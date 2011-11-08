#ifndef FS_WINDOWS_LIBRARY_H
#define FS_WINDOWS_LIBRARY_H

#include <QtPlugin>
#include "../ILibrary.h"

extern ICoreFunctions *g_Core;
class OperationsQueue;
class OperationsQueueDialog;

class FC_Library : public QObject, public ILibrary
{
	Q_OBJECT
	Q_INTERFACES(ILibrary)

public:
	QVector<Module *> ListModulesAndGetCore(ICoreFunctions *core);
	QObject* CreateModuleInstance(int id, QObject *parent);

private:
	OperationsQueue *module;
	OperationsQueueDialog *dialog;
};

#endif // FS_WINDOWS_LIBRARY_H