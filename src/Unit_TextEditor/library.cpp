#include "library.h"
#include "Unit_TextEditor.h"

ICoreFunctions *g_Core;

enum Modules {MUnit_TextEditor};

QVector<Module *> Unit_TextEditor_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;
	arr << new Module("TextEditorUnit", 1, "Unit_TextEditor", 1, MUnit_TextEditor);
	return arr;
}

QObject* Unit_TextEditor_Library::CreateModuleInstance( int id, QObject *parent )
{
	switch (id)
	{
	case MUnit_TextEditor:
		return new Unit_TextEditor(qobject_cast<QWidget *>(parent));
	}
	return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Unit_TextEditor, Unit_TextEditor_Library)
QT_END_NAMESPACE