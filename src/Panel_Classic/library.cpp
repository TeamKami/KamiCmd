#include "library.h"
#include "Panel_Classic.h"

ICoreFunctions *g_Core;
enum Modules {MPanel_Classic};

QVector<Module *> Panel_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr << new Module("Panel", 1, "Panel_Classic", 1, MPanel_Classic);
	return arr;
}

QObject* Panel_Library::CreateModuleInstance( int id, QObject *parent )
{
	switch (id)
	{
	case MPanel_Classic:
		return new Panel_Classic(qobject_cast<QWidget *>(parent));
	}
	return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Panel_Classic, Panel_Library)
QT_END_NAMESPACE