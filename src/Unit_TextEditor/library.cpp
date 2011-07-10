#include "Unit_TextEditor/library.h"
#include "Unit_TextEditor/Unit_TextEditor.h"
#include "Unit_TextEditor/ILexerPlugin.h"
#include <QRegExp>
#include "Unit_TextEditor/SciSettings.h"

enum Modules
{
    MUnit_TextEditor
};

QVector<Module *> Unit_TextEditor_Library::ListModulesAndGetCore(
        ICoreFunctions *core)
{
    g_Core = core;
    settings = 0;
    QVector<Module *> arr;
    arr << new Module("TextEditorUnit", 1, "Unit_TextEditor", 1,
            MUnit_TextEditor);
    return arr;
}

QObject* Unit_TextEditor_Library::CreateModuleInstance(int id, QObject *parent)
{
    switch (id)
    {
    case MUnit_TextEditor:
        if (!settings)
            settings = new SciSettings(this, g_Core);
        Unit_TextEditor * e = new Unit_TextEditor(qobject_cast<QWidget *> (
                parent), settings, g_Core);
        return e;
    }
    return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Unit_TextEditor, Unit_TextEditor_Library)
QT_END_NAMESPACE
