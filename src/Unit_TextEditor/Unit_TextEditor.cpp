#include "Unit_TextEditor.h"
#include "library.h"
#include "../UnitManager_Tabs/ActionManager.h"
#include <QRegExp>
#include <vector>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>

#include "ILexerPlugin.h"
std::map<QString, QsciLexer*> & lexers()
{
	static std::map<QString, QsciLexer*> lexers_;
	return lexers_;
}

std::vector<std::pair<QRegExp, QString> > & assoc()
{
	static std::vector<std::pair<QRegExp, QString> > assoc_;
	return assoc_;
}

QsciLexer * getLexer(QString const & name)
{
	if (lexers().count(name))
	{
		return lexers()[name];
	}
	
	return 0;
}

QString const & extToLexer(QString const & name)
{
	static QString empty;

	for (std::vector<std::pair<QRegExp, QString> >::iterator it = 
		assoc().begin(); it != assoc().end(); ++it)
	{
		if (it->first.exactMatch(name))
		{
			return it->second;
		}
	}
	
	return empty;
}

bool & lexersLoaded()
{
	static bool loaded = false;
	return loaded;
}

void Unit_TextEditor::loadModules()
{
	if (lexersLoaded())
		return;

	QVector<const Module *> lexerUnits = g_Core->GetModulesInfo("LexerUnit", 1);
		
	foreach(const Module * lexerUnit, lexerUnits)
	{
		ILexer * lexer = dynamic_cast<ILexer*>(g_Core->QueryModule("LexerModule", 1, lexerUnit->name));
		if (lexer)
		{
			lexers()[lexer->getName()] = lexer->getLexer();
			for (std::vector<QString>::const_iterator it = lexer->getMask().ext.begin(); it != lexer->getMask().ext.end(); ++it)
			{
				assoc().push_back(std::make_pair(QRegExp(*it, Qt::CaseInsensitive, QRegExp::Wildcard), lexer->getName()));
			}
		}
	}

	lexersLoaded() = true;
}

Unit_TextEditor::Unit_TextEditor(QWidget *parent)
	: IUnit(parent)
{
	setCentralWidget(editor = new QsciScintilla(this));	
	connect(editor, SIGNAL(textChanged()), this, SLOT(onEdit()));
	loadModules();

	ld = new LexersDialog(this);
	sd = new SettingsDialog(this);
	connect(ld, SIGNAL(setLexer(QsciLexer*)), this, SLOT(setLexer(QsciLexer*)));

	ActionManager *am;
	if (!(am = dynamic_cast<ActionManager *>(g_Core->QueryModule("ActionManager", 1))))
	{
		g_Core->DebugWrite("TextEditor", "ActionManager module not found", ICoreFunctions::Error);
		QMessageBox::warning(this, "TextEditor", "ActionManager module not found");
	}

	Actions << new QAction(QIcon(), tr("Select Lexer"), this);
	Actions.last()->setShortcut(Qt::CTRL + Qt::Key_L);
	Actions.last()->setShortcutContext(Qt::WindowShortcut);
	Actions.last()->setData("TextEditor");
	connect(Actions.last(), SIGNAL(triggered()), SLOT(selectLexer()));
	Actions << new QAction(QIcon(), tr("Editor settings"), this);
	Actions.last()->setShortcut(Qt::CTRL + Qt::Key_E);
	Actions.last()->setShortcutContext(Qt::WindowShortcut);
	Actions.last()->setData("TextEditor");
	connect(Actions.last(), SIGNAL(triggered()), SLOT(settings()));
	am->RegisterActions(Actions);
	addActions(Actions);

	editor->setFolding(QsciScintilla::PlainFoldStyle);
	editor->setAutoCompletionThreshold(2);
	editor->setAutoCompletionSource(QsciScintilla::AcsAll);
}

QString Unit_TextEditor::GetText()
{
	return text;
}

void Unit_TextEditor::SaveState(QSettings & set)
{
	set.setIniCodec("UTF-8");
	set.beginGroup("TextEditor");
	
	set.setValue("autoIndent", QVariant::fromValue(editor->autoIndent()));
	set.setValue("backspaceUnindents", QVariant::fromValue(editor->backspaceUnindents()));
	set.setValue("indentationGuides", QVariant::fromValue(editor->indentationGuides()));
	set.setValue("useTabs", QVariant::fromValue(editor->indentationsUseTabs()));
	set.setValue("tabIndents", QVariant::fromValue(editor->tabIndents()));
	set.setValue("indentationWidth", QVariant::fromValue(editor->indentationWidth()));
	set.setValue("tabWidth", QVariant::fromValue(editor->tabWidth()));

	set.setValue("foldingStyle", QVariant::fromValue(static_cast<int>(editor->folding())));

	set.setValue("edgeColumn", QVariant::fromValue(editor->edgeColumn()));
	set.setValue("edgeType", QVariant::fromValue(static_cast<int>(editor->edgeMode())));

	set.setValue("autocompletionCaseSensitive", QVariant::fromValue(editor->autoCompletionCaseSensitivity()));
	set.setValue("autocompletionReplaceWord", QVariant::fromValue(editor->autoCompletionReplaceWord()));
	set.setValue("autocompletionShowSingle", QVariant::fromValue(editor->autoCompletionShowSingle()));
	set.setValue("autocompletionFillups", QVariant::fromValue(editor->autoCompletionFillupsEnabled()));
	set.setValue("autocompletionTreshold", QVariant::fromValue(editor->autoCompletionThreshold()));
	set.setValue("autocompletionSource", QVariant::fromValue(static_cast<int>(editor->autoCompletionSource())));
	set.setValue("autocompletionUseSingle", QVariant::fromValue(static_cast<int>(editor->autoCompletionUseSingle())));

	set.setValue("wrapMode", QVariant::fromValue(static_cast<int>(editor->wrapMode())));
	set.setValue("wrapIndentation", QVariant::fromValue(static_cast<int>(editor->wrapIndentMode())));

	for (std::map<QString, QsciLexer*>::iterator it = lexers().begin(); it != lexers().end(); ++it)
	{
		set.beginGroup("Lexer_" + it->first);
		it->second->writeSettings(set);
		set.endGroup();
	}

	set.endGroup();
}

void Unit_TextEditor::LoadState(QSettings & set)
{
	set.setIniCodec("UTF-8");
	set.beginGroup("TextEditor");
	
	editor->setAutoIndent(set.value("autoIndent").value<bool>());
	editor->setBackspaceUnindents(set.value("backspaceUnindents").value<bool>());
	editor->setIndentationGuides(set.value("indentationGuides").value<bool>());
	editor->setIndentationsUseTabs(set.value("useTabs").value<bool>());
	editor->setTabIndents(set.value("tabIndents").value<bool>());
	editor->setIndentationWidth(set.value("indentationWidth").value<int>());
	editor->setTabWidth(set.value("tabWidth").value<int>());

	editor->setFolding(static_cast<QsciScintilla::FoldStyle>(set.value("foldingStyle").value<int>()));

	editor->setEdgeColumn(set.value("edgeColumn").value<int>());
	editor->setEdgeMode(static_cast<QsciScintilla::EdgeMode>(set.value("edgeType").value<int>()));

	editor->setAutoCompletionCaseSensitivity(set.value("autocompletionCaseSensitive").value<bool>());
	editor->setAutoCompletionReplaceWord(set.value("autocompletionReplaceWord").value<bool>());
	editor->setAutoCompletionShowSingle(set.value("autocompletionShowSingle").value<bool>());
	editor->setAutoCompletionFillupsEnabled(set.value("autocompletionFillups").value<bool>());
	editor->setAutoCompletionThreshold(set.value("autocompletionTreshold").value<bool>());
	editor->setAutoCompletionSource(static_cast<QsciScintilla::AutoCompletionSource>(set.value("autocompletionSource").value<int>()));
	editor->setAutoCompletionUseSingle(static_cast<QsciScintilla::AutoCompletionUseSingle>(set.value("autocompletionUseSingle").value<int>()));

	editor->setWrapMode(static_cast<QsciScintilla::WrapMode>(set.value("wrapMode").value<int>()));
	editor->setWrapIndentMode(static_cast<QsciScintilla::WrapIndentMode>(set.value("wrapIndentation").value<int>()));

	for (std::map<QString, QsciLexer*>::iterator it = lexers().begin(); it != lexers().end(); ++it)
	{
		set.beginGroup("Lexer_" + it->first);
		it->second->readSettings(set);
		set.endGroup();
	}


	set.endGroup();
}

void Unit_TextEditor::Create( IUnit *createdFrom )
{
	if (hostPanel = dynamic_cast<IPanel *>(createdFrom))
	{
		const FileInfo *const info = hostPanel->GetCurrentFile();
	
		QFile file(info->path + info->name);


		if (!file.open(QFile::ReadOnly)) {
			QMessageBox::warning(this, tr("Application"),
								 tr("Cannot read file %1:\n%2.")
								 .arg(info->path + info->name)
								 .arg(file.errorString()));
			return;
		}

		QTextStream in(&file);
		QApplication::setOverrideCursor(Qt::WaitCursor);

		editor->setText(in.readAll());

		text = info->name;
		path = info->path;
		edited = false;
		emit TextChanged();

		QFileInfo qinfo(info->path + info->name);

		QString lex = extToLexer(info->name);

		if (lex != "")
		{
			editor->setLexer(getLexer(lex));
		}
		
		QApplication::restoreOverrideCursor();
	}
}

QIcon Unit_TextEditor::GetIcon()
{
	return QIcon();
}

void Unit_TextEditor::onEdit()
{
	if (edited)
		return;

	edited = true;
	text = text + "*";
	emit TextChanged();
}

void Unit_TextEditor::selectLexer()
{
	ld->show(lexers(), editor->lexer());
}

void Unit_TextEditor::settings()
{
	sd->show(editor);
}

void Unit_TextEditor::setLexer(QsciLexer * l)
{
	editor->setLexer(l);
}