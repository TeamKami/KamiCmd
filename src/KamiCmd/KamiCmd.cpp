#include "KamiCmd.h"

#include <QtCore/QDebug>

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

DebugLog *CoreFunctions::log = 0;

bool CoreFunctions::LoadModules()
{
	int num = 0;
	QDir modulesDir(QApplication::applicationDirPath());
	modulesDir.cd("Modules");

#ifdef Q_WS_WIN
 	foreach (QString fileName, modulesDir.entryList(QStringList("*.dll"), QDir::Files))
#else
	foreach (QString fileName, modulesDir.entryList(QDir::Files))
#endif // Q_WS_WIN
	{
        QPluginLoader loader(modulesDir.absoluteFilePath(fileName));
	    QObject *libObject = loader.instance();
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
		{
			QMessageBox::warning(0, tr("Application"), tr("Failed to load %1\n%2")
					.arg(fileName)
					.arg(loader.errorString())
					);
		}
	}
	if (!num)
		DebugWrite("Core", QString("Modules not found\nSearched in: \"%1\"").arg(modulesDir.absolutePath()), Error);

	qSort(modules.begin(), modules.end(), moduleLessThan);

	return num != 0;
}


void CoreFunctions::DebugWrite( QString sender, QString message, DebugWriteImportance importance /*= Info*/ )
{
	message = QString("Module: %1\t Message: %2").arg(sender, message);
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
	}
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

void CoreFunctions::ShowDebugOutput()
{
	RedirectDebug();
	emit ShowDebug();
}

void CoreFunctions::OutputDebugMesage( QtMsgType type, const char *msg )
{
	if(log)
		log->Write(type, msg);
}

void CoreFunctions::RedirectDebug()
{
	if(log)
		return;
	log = new DebugLog(this);
	connect(this, SIGNAL(ShowDebug()), log, SLOT(ShowDebugDialog()));
	qInstallMsgHandler(OutputDebugMesage);
}
