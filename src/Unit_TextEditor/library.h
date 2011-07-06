#ifndef UNIT_PICTUREVIEW_LIBRARY_H
#define UNIT_PICTUREVIEW_LIBRARY_H

#include <QtPlugin>
#include "../ILibrary.h"
#include "Unit_TextEditor.h"

extern ICoreFunctions *g_Core;

class Unit_TextEditor_Library : public QObject, public ILibrary
{
	Q_OBJECT
	Q_INTERFACES(ILibrary)

public:
	QVector<Module *> ListModulesAndGetCore(ICoreFunctions *core);
	QObject* CreateModuleInstance(int id, QObject *parent);

private slots:
	void updateSettings();

signals:
	void settingsChanged();
};

#endif // UNIT_PICTUREVIEW_LIBRARY_H
