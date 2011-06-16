#ifndef UNITMANAGER_TABS_LIBRARY_H
#define UNITMANAGER_TABS_LIBRARY_H

#include <QtPlugin>
#include "../ILibrary.h"

extern ICoreFunctions *g_Core;

class UnitManager_Tabs_Library : public QObject, public ILibrary
{
	Q_OBJECT
	Q_INTERFACES(ILibrary)

public:
	QVector<Module *> ListModulesAndGetCore(ICoreFunctions *core);
	QObject* CreateModuleInstance(int id, QObject *parent);
};

#endif // UNITMANAGER_TABS_LIBRARY_H