#ifndef UNIT_CONSOLE_LIBRARY_H
#define UNIT_CONSOLE_LIBRARY_H

#include <QtPlugin>
#include "ILibrary.h"
#include "Unit_KamiShell/Unit_KamiShell.h"

class Unit_Console_Library : public QObject, public ILibrary
{
	Q_OBJECT
	Q_INTERFACES(ILibrary)

public:
	QVector<Module *> ListModulesAndGetCore(ICoreFunctions *core);
	QObject* CreateModuleInstance(int id, QObject *parent);
private:
	ICoreFunctions *g_Core;
};

#endif // UNIT_CONSOLE_LIBRARY_H
