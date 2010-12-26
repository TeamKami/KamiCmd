#include "library.h"
#include "UnitManager_Tabs.h"
#include "LinkedUnit.h"
#include "ButtonsBar.h"
#include "ModulesDialog.h"
#include "Associations.h"

ICoreFunctions *g_Core;

enum Modules {MUnitManager, MLinkedUnit, MButtonsBar, MModulesDialog, MAssociations};

QVector<Module *> UnitManager_Tabs_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr << new Module("UnitManager", 1, "UnitManager_Tabs", 1, MUnitManager)
		<< new Module("LinkedUnit", 1, "UnitManager_LinkedUnit", 1, MLinkedUnit)
		<< new Module("ButtonsBar", 1, "UnitManager_ButtonsBar", 1, MButtonsBar)
		<< new Module("ModulesDialog", 1, "UnitManager_ModulesDialog", 1, MModulesDialog)
		<< new Module("Associations", 1, "UnitManager_Associations", 1, MAssociations);
	return arr;
}

QObject* UnitManager_Tabs_Library::CreateModuleInstance( int id, QObject *parent )
{
	switch (id)
	{
	case MUnitManager:
		if (!module1)
			module1 = new UnitManager_Tabs(qobject_cast<QWidget *>(parent));
		return module1;

	case MLinkedUnit:
		return new LinkedUnit(qobject_cast<QWidget *>(parent));

	case MButtonsBar:
		return new ButtonsBar(qobject_cast<QWidget *>(parent));

	case MModulesDialog:
		return new ModulesDialog(qobject_cast<QWidget *>(parent));

	case MAssociations:
		if (!moduleAscs)
			moduleAscs = new Associations(parent);
		return moduleAscs;
	}
	return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(UnitManager_Tabs, UnitManager_Tabs_Library)
QT_END_NAMESPACE