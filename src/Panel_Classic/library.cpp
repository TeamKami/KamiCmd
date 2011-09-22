#include "library.h"
#include "Panel_Classic.h"

ICoreFunctions *g_Core;
enum Modules {MPanel_Classic, MQuickSearchWidget};

QVector<Module *> Panel_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr << new Module("Panel", 1, "Panel_Classic", 1, MPanel_Classic)
		<< new Module("QuickSearchWidget", 1, "QuickSearchWidget_PanelClassic", 1, MQuickSearchWidget);
	return arr;
}

QObject* Panel_Library::CreateModuleInstance( int id, QObject *parent )
{
	switch (id)
	{
	case MPanel_Classic:
		return new Panel_Classic(qobject_cast<QWidget *>(parent));
	case MQuickSearchWidget:
		return new Panel_Classic(qobject_cast<QWidget *>(parent));
	}
	return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Panel_Classic, Panel_Library)
QT_END_NAMESPACE