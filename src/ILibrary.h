#ifndef LIBRARIES_INTERFACE_H
#define LIBRARIES_INTERFACE_H

#include <QVector>

struct Module;

class ICoreFunctions
{
public:
	enum DebugWriteImportance {Error, Warning, Info, ReportMe};

	virtual void DebugWrite(QString sender, QString message, DebugWriteImportance importance = Info) = 0;
	virtual QObject *QueryModule(QString type, int interfaceVersion, QString name = "", int moduleVersion = -1, QObject *parent = NULL) = 0;
	virtual QVector<const Module *> GetModulesInfo(QString type = "", int interfaceVersion = 0) = 0;
};

struct Module
{
	Module(QString inType, int inInterfaceVersion, QString inName, int inModuleVersion, int inModuleId):
	  type(inType), interfaceVersion(inInterfaceVersion), name(inName), moduleVersion(inModuleVersion), moduleId(inModuleId), typeDependentInfo(0) {};
	QString type;
	int interfaceVersion;
	QString name;
	int moduleVersion;

	int moduleId;
	quintptr typeDependentInfo;
	quintptr parentLib;
	QString parentLibName;
};


class ILibrary
{
public:
	virtual QVector<Module *> ListModulesAndGetCore(ICoreFunctions *core) = 0;
	virtual QObject* CreateModuleInstance(int id, QObject *parent) = 0;
};

QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(ILibrary, "KamiCmd_Library/1.0")
QT_END_NAMESPACE

#endif