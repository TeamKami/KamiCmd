#include "library.h"
#include "Unit_WebKit.h"

ICoreFunctions *g_Core;

enum Modules {MUnit_WebKit};

QVector<Module *> Unit_WebKit_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr << new Module("WebKitUnit", 1, "Unit_WebKit", 1, MUnit_WebKit);
	return arr;
}

QObject* Unit_WebKit_Library::CreateModuleInstance( int id, QObject *parent )
{
	switch (id)
	{
	case MUnit_WebKit:
		return new Unit_WebKit(qobject_cast<QWidget *>(parent));
	}
	return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Unit_WebKit, Unit_WebKit_Library)
QT_END_NAMESPACE