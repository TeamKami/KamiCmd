#ifndef KAMICMD_H
#define KAMICMD_H

#include <QtGui>
#include "../ILibrary.h"
#include "../IUnitManager.h"
#include "DebugLog.h"

class CoreFunctions : public QObject, public ICoreFunctions
{
	Q_OBJECT

public:

	virtual void DebugWrite(QString sender, QString message, DebugWriteImportance importance = Info);
	virtual QObject *QueryModule(QString type, int interfaceVersion, QString name = "", int moduleVersion = -1, QObject *parent = NULL);
	virtual QVector<const Module *> GetModulesInfo(QString type = "", int interfaceVersion = 0);
	virtual void ShowDebugOutput();
	
	bool LoadModules();

signals:
	void ShowDebug();
private:
	void RedirectDebug();
	static void OutputDebugMesage( QtMsgType type, const char *msg );

	QVector<Module *> modules;
	
	static DebugLog* log;
};

#endif // KAMICMD_H
