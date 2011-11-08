#include "library.h"
#include "OperationsQueue.h"
#include "OperationQueueDialog.h"

ICoreFunctions *g_Core;

QVector<Module *> FC_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;
	module = 0;
	dialog = 0;

	QVector<Module *> arr;
	arr += new Module("OperationsQueue", CURRENT_FILE_OPERATION_INTERFACE_VERSION, "OperationsQueueClassic", 1, 0);
	arr += new Module("OperationsQueueDialog", CURRENT_FILE_OPERATION_INTERFACE_VERSION, "OQ_Widget", 1, 1);
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
	{
		if(!dialog)
			dialog = new OperationsQueueDialog(qobject_cast<QWidget *>(parent));
		else
			dialog->setParent(qobject_cast<QWidget *>(parent));

		return dialog;
	}
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(FC_Library, FC_Library)
QT_END_NAMESPACE