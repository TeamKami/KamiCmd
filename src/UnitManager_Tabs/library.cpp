#include "library.h"
#include "UnitManager_Tabs.h"
#include "LinkedUnit.h"
#include "FButtonsBar.h"
#include "ModulesDialog.h"
#include "Associations.h"
#include "ActionManager.h"
#include "DebugDialog.h"

ICoreFunctions *g_Core;

enum Modules {MUnitManager, MLinkedUnit, MFButtonsBar, MModulesDialog, MAssociations, MActionManager, MDebugDialog};

QVector<Module *> UnitManager_Tabs_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr << new Module("UnitManager", 1, "UnitManager_Tabs", 1, MUnitManager)
		<< new Module("LinkedUnit", 1, "UnitManager_LinkedUnit", 1, MLinkedUnit)
		<< new Module("FButtonsBar", 1, "UnitManager_FButtonsBar", 1, MFButtonsBar)
		<< new Module("ModulesDialog", 1, "UnitManager_ModulesDialog", 1, MModulesDialog)
		<< new Module("Associations", 1, "UnitManager_Associations", 1, MAssociations)
		<< new Module("ActionManager", 1, "UnitManager_ActionManager", 1, MActionManager)
		<< new Module("DebugDialog", 1, "UnitManager_DebugDialog", 1, MDebugDialog);
	return arr;
}

QObject* UnitManager_Tabs_Library::CreateModuleInstance( int id, QObject *parent )
{
	static UnitManager_Tabs *moduleUnitManager = NULL;
	static Associations *moduleAssociations = NULL;
	static ActionManager *moduleActionManager = NULL;
	static DebugDialog *debugDialog = NULL;
	
	switch (id)
	{
	case MUnitManager:
		if (!moduleUnitManager)
			moduleUnitManager = new UnitManager_Tabs(qobject_cast<QWidget *>(parent));
		return moduleUnitManager;

	case MLinkedUnit:
		return new LinkedUnit(qobject_cast<QWidget *>(parent));

	case MFButtonsBar:
		return new FButtonsBar(qobject_cast<QWidget *>(parent));

	case MModulesDialog:
		return new ModulesDialog(qobject_cast<QWidget *>(parent));

	case MAssociations:
		if (!moduleAssociations)
			moduleAssociations = new Associations(parent);
		return moduleAssociations;

	case MActionManager:
		if (!moduleActionManager)
			moduleActionManager = new ActionManager(parent);
		return moduleActionManager;

	case MDebugDialog:
		if (!debugDialog)
			debugDialog = new DebugDialog();
		return debugDialog;
	}
	return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(UnitManager_Tabs, UnitManager_Tabs_Library)
QT_END_NAMESPACE