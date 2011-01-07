#include "library.h"
#include "FS_Qt.h"

ICoreFunctions *g_Core;

QVector<Module *> FS_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr += new Module("FS", 1, "FS_Qt", 0, 0);
	return arr;
}

QObject* FS_Library::CreateModuleInstance( int /*id*/, QObject *parent )
{
	return new FS_Qt(parent);
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(FS_Qt, FS_Library)
QT_END_NAMESPACE