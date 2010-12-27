#include "library.h"
#include "OperationsQueue.h"
#include "oqwidget.h"

ICoreFunctions *g_Core;

QVector<Module *> FC_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;
	module = 0;
	QVector<Module *> arr;
	arr += new Module("OperationsQueue", 1, "OQ", 1, 0);
	arr += new Module("OperationsQueue", 1, "OQ_Widget", 1, 1);
	return arr;
}

QObject* FC_Library::CreateModuleInstance( int id, QObject *parent )
{
	if(!id) 
	{
		if(!module)
			module = new OperationsQueue(parent);
		return module;
	}
	else
		return new OQWidget(qobject_cast<QWidget *>(parent));

//	return new OQWidget(qobject_cast<QWidget *>(parent));//OperationsQueue(parent);
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(FC_Library, FC_Library)
QT_END_NAMESPACE