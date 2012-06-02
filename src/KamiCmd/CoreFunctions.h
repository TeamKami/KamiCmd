#ifndef KAMICMD_H
#define KAMICMD_H

#include <QtWidgets>
#include "../ILibrary.h"
#include "../IUnitManager.h"

class DebugLog;


class CoreFunctions : public ICoreFunctions
{
	Q_OBJECT

public:
	CoreFunctions();
	virtual void DebugWrite(QString sender, QString message, DebugWriteImportance importance = Info);
	virtual QObject *QueryModule(QString type, int interfaceVersion, QString name = "", int moduleVersion = -1, QObject *parent = NULL);
	virtual QVector<const Module *> GetModulesInfo(QString type = "", int interfaceVersion = 0);
	virtual QList<QPair<ICoreFunctions::DebugWriteImportance, QString> > GetDebugWriteLog();
	virtual bool LoadModules();
	void RedirectDebug();

signals:
	void DebugMessageReceived(ICoreFunctions::DebugWriteImportance importance, QString message);

private:
	static DebugLog debugLog;
	QVector<Module *> modules;
};

class DebugLogEmitter : public QObject
{
	Q_OBJECT

public:
	void EmitMessage(ICoreFunctions::DebugWriteImportance importance, QString message);

signals:
	void DebugMessageReceived(ICoreFunctions::DebugWriteImportance importance, QString message);
};

class DebugLog : public QObject
{
	Q_OBJECT

public:
	static void OutputDebugMessage(QtMsgType type, const char *msg);

	static QMutex mutex;	
	static QList<QPair<ICoreFunctions::DebugWriteImportance, QString> > log;
	static bool isDebugRedirect;
	static DebugLogEmitter emitter;
};

#endif // KAMICMD_H
