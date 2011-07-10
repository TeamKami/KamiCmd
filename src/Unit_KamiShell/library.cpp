#include "library.h"
#include "Unit_KamiShell/Unit_KamiShell.h"

enum Modules {MUnit_KamiShell};

QVector<Module *> Unit_Console_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr << new Module("ConsoleUnit", 1, "Unit_KamiShell", 1, MUnit_KamiShell);
	return arr;
}

QObject* Unit_Console_Library::CreateModuleInstance( int id, QObject *parent )
{
	switch (id)
	{
	case MUnit_KamiShell:
		return new Unit_KamiShell(qobject_cast<QWidget *>(parent));
	}
	return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Unit_Console, Unit_Console_Library)
QT_END_NAMESPACE
