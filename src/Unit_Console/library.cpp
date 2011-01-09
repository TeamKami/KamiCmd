#include "library.h"
#include "Unit_Console.h"

ICoreFunctions *g_Core;

enum Modules {MUnit_Console};

QVector<Module *> Unit_Console_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr << new Module("ConsoleUnit", 1, "Unit_Console", 1, MUnit_Console);
	return arr;
}

QObject* Unit_Console_Library::CreateModuleInstance( int id, QObject *parent )
{
	switch (id)
	{
	case MUnit_Console:
		return new Unit_Console(qobject_cast<QWidget *>(parent));
	}
	return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Unit_Console, Unit_Console_Library)
QT_END_NAMESPACE