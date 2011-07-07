#ifndef UNIT_PICTUREVIEW_LIBRARY_H
#define UNIT_PICTUREVIEW_LIBRARY_H

#include <QtPlugin>
#include "../ILibrary.h"
#include "Unit_TextEditor.h"

class SciSettings;

class Unit_TextEditor_Library : public QObject, public ILibrary
{
	Q_OBJECT
	Q_INTERFACES(ILibrary)

public:
	QVector<Module *> ListModulesAndGetCore(ICoreFunctions *core);
	QObject* CreateModuleInstance(int id, QObject *parent);

private:
	SciSettings * settings;
	ICoreFunctions *g_Core;
};

#endif // UNIT_PICTUREVIEW_LIBRARY_H
