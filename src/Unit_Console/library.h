#ifndef UNIT_CONSOLE_LIBRARY_H
#define UNIT_CONSOLE_LIBRARY_H

#include <QtPlugin>
#include "../ILibrary.h"
#include "Unit_Console.h"

extern ICoreFunctions *g_Core;

class Unit_Console_Library : public QObject, public ILibrary
{
	Q_OBJECT
	Q_INTERFACES(ILibrary)

public:
	QVector<Module *> ListModulesAndGetCore(ICoreFunctions *core);
	QObject* CreateModuleInstance(int id, QObject *parent);
};

#endif // UNIT_CONSOLE_LIBRARY_H