#include "library.h"
#include "FileCopy.h"
#include "CopyDialog.h"


ICoreFunctions *g_Core;

QVector<Module *> FC_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr += new Module("FileCopyOperation", 1, "FileCopyOperation_Qt", 1, 0);
	arr += new Module("FileCopyDialog", 1, "FileCopyQt_Dialog", 1, 1);
	return arr;
}

QObject* FC_Library::CreateModuleInstance( int id, QObject *parent )
{
	switch(id)
	{
	case 0:
		return new FileCopy(parent);
	case 1:
		return new CopyDialog(qobject_cast<QWidget *>(parent));
	}
	return 0;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(FC_Library, FC_Library)
QT_END_NAMESPACE