#include "library.h"
#include "FileOperationDummy.h"

#include <QObject>
ICoreFunctions *g_Core;

QVector<Module *> Dummy_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr += new Module("DummyFileOperation", 1, "DummyFileOperation_Qt", 0, 0);
	return arr;
}

QObject* Dummy_Library::CreateModuleInstance( int id, QObject *parent )
{
	switch(id)
	{
	case 0:
		return new FileOperationDummy(parent);
	}
	return 0;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Dummy_Library, Dummy_Library)
QT_END_NAMESPACE