#ifndef PANEL_CLASSIC_LIBRARY_H
#define PANEL_CLASSIC_LIBRARY_H

#include <QtPlugin>
#include "../ILibrary.h"

extern ICoreFunctions *g_Core;

class Panel_Library : public QObject, public ILibrary
{
	Q_OBJECT
	Q_INTERFACES(ILibrary)

public:
	Panel_Library(){}
	QVector<Module *> ListModulesAndGetCore(ICoreFunctions *core);
	QObject* CreateModuleInstance(int id, QObject *parent);
};

#endif // PANEL_CLASSIC_H