#include "library.h"
#include "Unit_TextEditor.h"
#include "ILexerPlugin.h"
#include <QRegExp>

ICoreFunctions *g_Core;

enum Modules {MUnit_TextEditor};

std::map<QString, QsciLexer*> & lexers();
std::vector<std::pair<QRegExp, QString> > & assoc();

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
		Unit_TextEditor * e = new Unit_TextEditor(qobject_cast<QWidget *>(parent));
		connect(this, SIGNAL(settingsChanged(QSettings &)), e, SLOT(LoadSettings(QSettings &)));
		connect(e, SIGNAL(settingsChanged()), this, SLOT(updateSettings()));
		return e;
	}
	return NULL;
}

void Unit_TextEditor_Library::updateSettings()
{
	QSettings set;
	emit settingsChanged(set);
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Unit_TextEditor, Unit_TextEditor_Library)
QT_END_NAMESPACE