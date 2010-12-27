#include "library.h"
#include "Copy.h"


ICoreFunctions *g_Core;

QVector<Module *> FC_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr += new Module("FileCopy", 1, "FileCopy_Windows", 1, 0);
	return arr;
}

QObject* FC_Library::CreateModuleInstance( int id, QObject *parent )
{
	return new FileCopy(parent);//	return new OQWidget(qobject_cast<QWidget *>(parent));//OperationsQueue(parent);
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(FC_Library, FC_Library)
QT_END_NAMESPACE