#include "library.h"
#include "CopyWidget.h"


ICoreFunctions *g_Core;

QVector<Module *> FC_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr += new Module("FileCopy", 1, "FileCopy_Widget", 1, 1);
	return arr;
}

QObject* FC_Library::CreateModuleInstance( int id, QObject *parent )
{
	return new CopyWidget(qobject_cast<QDialog *>(parent));
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(FC_Library, FC_Library)
QT_END_NAMESPACE