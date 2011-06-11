#include "library.h"
#include "Unit_PictureView.h"

ICoreFunctions *g_Core;

enum Modules {MUnit_PictureView};

QVector<Module *> Unit_PictureView_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr << new Module("PictureViewUnit", 1, "Unit_PictureView", 1, MUnit_PictureView);
	return arr;
}

QObject* Unit_PictureView_Library::CreateModuleInstance( int id, QObject *parent )
{
	switch (id)
	{
	case MUnit_PictureView:
		return new Unit_PictureView(qobject_cast<QWidget *>(parent));
	}
	return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Unit_PictureView, Unit_PictureView_Library)
QT_END_NAMESPACE