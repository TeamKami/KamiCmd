#include "CoreFunctions.h"
#include <QtCore/QDebug>

DebugLog CoreFunctions::debugLog;

QMutex DebugLog::mutex;	
QList<QPair<ICoreFunctions::DebugWriteImportance, QString> > DebugLog::log;
bool DebugLog::isDebugRedirect = false;
DebugLogEmitter DebugLog::emitter;


CoreFunctions::CoreFunctions()
{

}

bool moduleLessThan( Module *first, Module *second )
{
	if (first->type == second->type)
	{
                if (first->interfaceVersion > second->interfaceVersion ||
                        (first->interfaceVersion == second->interfaceVersion && first->moduleVersion > second->moduleVersion))
			return true;
		return false;
	}
	return first->type < second->type;
}

bool CoreFunctions::LoadModules()
{
	int num = 0;
	QDir modulesDir(QApplication::applicationDirPath());
	modulesDir.cd("Modules");

#if defined(Q_OS_WIN)
	foreach (QString fileName, modulesDir.entryList(QStringList("*.dll"), QDir::Files))
#elif defined(Q_OS_LINUX)
	foreach (QString fileName, modulesDir.entryList(QStringList("*.so"), QDir::Files))
#elif defined(Q_OS_MAC)
	foreach (QString fileName, modulesDir.entryList(QStringList("*.dylib"), QDir::Files))
#else
	foreach (QString fileName, modulesDir.entryList(QDir::Files))
#endif // Q_OS_WIN
	{
        QPluginLoader loader(modulesDir.absoluteFilePath(fileName));
	    QObject *libObject = loader.instance();
		qDebug() << loader.errorString();
		if (ILibrary *lib = qobject_cast<ILibrary *>(libObject))
		{
			libObject->setParent(this);
			int j = modules.size();
			modules += lib->ListModulesAndGetCore(this);
			for (; j < modules.size(); j++)
			{
				modules[j]->parentLib = (quintptr)(quint64)lib;
				modules[j]->parentLibName = fileName;
			}

			num++;
			//DebugWrite(modules.last()->name, "");
		}
		else
			DebugWrite(("Core"), QString("Failed to load %1\n%2").arg(fileName).arg(loader.errorString()));
	}
	if (!num)
		DebugWrite("Core", QString("Modules not found\nSearched in: \"%1\"").arg(modulesDir.absolutePath()), Error);

	qSort(modules.begin(), modules.end(), moduleLessThan);

	return num != 0;
}

QObject * CoreFunctions::QueryModule( QString type, int interfaceVersion, QString name /*= ""*/, int moduleVersion /*= -1*/, QObject *parent /*= 0*/ )
{
	if (!parent)
		parent = this;

	if (!type.isEmpty())
		for (int i = 0; i < modules.size(); i++)
			if (modules[i]->type == type && (name == modules[i]->name || name.isEmpty()) && interfaceVersion == modules[i]->interfaceVersion && (moduleVersion == modules[i]->moduleVersion || moduleVersion == -1))
				return ((ILibrary *)((quint64)modules[i]->parentLib))->CreateModuleInstance(modules[i]->moduleId, parent);
	return NULL;
}

QVector<const Module *> CoreFunctions::GetModulesInfo(QString type /*= ""*/, int interfaceVersion /*= 0*/)
{
	QVector<const Module *> arr;

	for (int i = 0; i < modules.size(); i++)
                if (type.isEmpty() || (modules[i]->type == type && interfaceVersion == modules[i]->interfaceVersion))
			arr.append(modules[i]);

	return arr;
}

void CoreFunctions::DebugWrite( QString sender, QString message, DebugWriteImportance importance /*= Info*/ )
{
	message = QString("Module: %1\t Message: %2").arg(sender, message);

	debugLog.mutex.lock();

	if (debugLog.isDebugRedirect)
	{
		debugLog.log.append(qMakePair(importance, message));
		debugLog.mutex.unlock();
		emit DebugMessageReceived(importance, message);
		return;
	}
	debugLog.mutex.unlock();

	switch (importance)
	{
	case Error:
		qCritical() << message;
		break;
	case Warning:
		qWarning() << message;
		break;
	case Info:
		qDebug() << message;
		break;
	case ReportMe:
        qFatal(static_cast<char *>(message.toLocal8Bit().data()));
		break;
	default:
		break;
	}
}


void CoreFunctions::RedirectDebug()
{
	QMutexLocker lock(&debugLog.mutex);
	connect(&debugLog.emitter, SIGNAL(DebugMessageReceived(ICoreFunctions::DebugWriteImportance, QString)), SIGNAL(DebugMessageReceived(ICoreFunctions::DebugWriteImportance, QString)));
	qInstallMsgHandler(&DebugLog::OutputDebugMessage);
	debugLog.isDebugRedirect = true;
}

QList<QPair<ICoreFunctions::DebugWriteImportance, QString> > CoreFunctions::GetDebugWriteLog()
{
	return debugLog.log;
}

void DebugLog::OutputDebugMessage( QtMsgType type, const char *msg )
{
	qRegisterMetaType<ICoreFunctions::DebugWriteImportance>("ICoreFunctions::DebugWriteImportance");

	QMutexLocker lock(&mutex);
    QString debugMsg = QString::fromLocal8Bit(msg); // FIXME: Is it really local 8bit?

#ifdef Q_WS_MAC
    static bool isTabletDeviceUnknownMsgShown = false;
    if (debugMsg.startsWith("QCocoaView handleTabletEvent: This tablet device is unknown"))
    {
        if (isTabletDeviceUnknownMsgShown)
            return;
        isTabletDeviceUnknownMsgShown = true;
        debugMsg.append(" This message is posted hundred times, so KamiCmd only shows it once");
    }
#endif

    log.append(qMakePair((ICoreFunctions::DebugWriteImportance)type, debugMsg));
	emitter.EmitMessage(log.last().first, log.last().second);
}

void DebugLogEmitter::EmitMessage( ICoreFunctions::DebugWriteImportance importance, QString message )
{
	emit DebugMessageReceived((ICoreFunctions::DebugWriteImportance)importance, message);
}
