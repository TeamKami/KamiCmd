#ifndef UNIT_PICTUREVIEW_LIBRARY_H
#define UNIT_PICTUREVIEW_LIBRARY_H

#include <QtPlugin>
#include "../ILibrary.h"

extern ICoreFunctions *g_Core;

class Lexers_Library : public QObject, public ILibrary
{
	Q_OBJECT
	Q_INTERFACES(ILibrary)

public:
	QVector<Module *> ListModulesAndGetCore(ICoreFunctions *core);
	QObject* CreateModuleInstance(int id, QObject *parent);
};

#endif // UNIT_PICTUREVIEW_LIBRARY_H
