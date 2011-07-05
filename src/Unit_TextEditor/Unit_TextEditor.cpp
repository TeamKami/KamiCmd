#include "Unit_TextEditor.h"
#include "library.h"

#include <QRegExp>
#include <vector>

#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerbash.h>
#include <Qsci/qscilexerbatch.h>
#include <Qsci/qscilexercmake.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexercsharp.h>
#include <Qsci/qscilexercss.h>
#include <Qsci/qscilexerd.h>
#include <Qsci/qscilexerdiff.h>
#include <Qsci/qscilexerfortran.h>
#include <Qsci/qscilexerfortran77.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qscilexeridl.h>
#include <Qsci/qscilexerjava.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qscilexerlua.h>
#include <Qsci/qscilexermakefile.h>
#include <Qsci/qscilexermatlab.h>
#include <Qsci/qscilexeroctave.h>
#include <Qsci/qscilexerpascal.h>
#include <Qsci/qscilexerperl.h>
#include <Qsci/qscilexerpostscript.h>
#include <Qsci/qscilexerpov.h>
#include <Qsci/qscilexerpython.h>
#include <Qsci/qscilexerruby.h>
#include <Qsci/qscilexerspice.h>
#include <Qsci/qscilexersql.h>
#include <Qsci/qscilexertcl.h>
#include <Qsci/qscilexertex.h>
#include <Qsci/qscilexerverilog.h>
#include <Qsci/qscilexervhdl.h>
#include <Qsci/qscilexerxml.h>
#include <Qsci/qscilexeryaml.h>

QsciLexer * getLexer(QString const & name)
{
	static std::map<QString, QsciLexer*> lexers;
	static bool loaded = false;

	if (!loaded)
	{
		lexers["bash"] = new QsciLexerBash;
		lexers["batch"] = new QsciLexerBatch;
		lexers["cmake"] = new QsciLexerCMake;
		lexers["cpp"] = new QsciLexerCPP;
		lexers["csharp"] = new QsciLexerCSharp;
		lexers["css"] = new QsciLexerCSS;
		lexers["d"] = new QsciLexerD;
		lexers["diff"] = new QsciLexerDiff;
		lexers["fortran"] = new QsciLexerFortran;
		lexers["fortran77"] = new QsciLexerFortran77;
		lexers["html"] = new QsciLexerHTML;
		lexers["idl"] = new QsciLexerIDL;
		lexers["java"] = new QsciLexerJava;
		lexers["javascript"] = new QsciLexerJavaScript;
		lexers["lua"] = new QsciLexerLua;
		lexers["makefile"] = new QsciLexerMakefile;
		lexers["matlab"] = new QsciLexerMatlab;
		lexers["octave"] = new QsciLexerOctave;
		lexers["pascal"] = new QsciLexerPascal;
		lexers["perl"] = new QsciLexerPerl;
		lexers["postscript"] = new QsciLexerPostScript;
		lexers["pov"] = new QsciLexerPOV;
		lexers["python"] = new QsciLexerPython;
		lexers["ruby"] = new QsciLexerRuby;
		lexers["spice"] = new QsciLexerSpice;
		lexers["sql"] = new QsciLexerSQL;
		lexers["tcl"] = new QsciLexerTCL;
		lexers["tex"] = new QsciLexerTeX;
		lexers["verilog"] = new QsciLexerVerilog;
		lexers["vhdl"] = new QsciLexerVHDL;
		lexers["xml"] = new QsciLexerXML;
		lexers["yaml"] = new QsciLexerYAML;
		loaded = true;
	}

	if (lexers.count(name))
	{
		return lexers[name];
	}
	
	return 0;
}

QString const & extToLexer(QString const & name)
{
	static std::vector<std::pair<QRegExp, QString> > assoc;
	static bool loaded = false;
	static QString empty;

	if (!loaded)
	{
		#define R(expr, lex) assoc.push_back(std::make_pair(QRegExp(expr, Qt::CaseInsensitive, QRegExp::Wildcard), lex));
		R("*.sh", "bash");
		R("*.bat", "batch");
		R("CMakeLists.txt", "cmake");
		R("*.cmake", "cmake");
		R("*.cpp", "cpp");
		R("*.h", "cpp");
		R("*.cxx", "cpp");
		R("*.hpp", "cpp");
		R("*.c", "cpp");
		R("*.cs", "cpp");
		R("*.css", "css");
		R("*.d", "d");
		R("*.diff", "diff");
		R("*.patch", "diff");
		R("*.html", "html");
		R("*.htm", "html");
		R("*.xhtml", "html");
		R("*.java", "java");
		R("*.js", "javascript");
		R("*.lua", "lua");
		R("makefile", "makefile");
		R("*.mk", "makefile"); 
		R("*.pas", "pascal");
		R("*.pl", "perl");
		R("*.ps", "postscript");
		R("*.py", "python");
		R("*.sql", "sql");
		R("*.tex", "tex");
		R("*.xml", "xml");
		R("*.xsl", "xml");
		#undef R
		loaded = true;
	}

	for (std::vector<std::pair<QRegExp, QString> >::iterator it = 
		assoc.begin(); it != assoc.end(); ++it)
	{
		if (it->first.exactMatch(name))
		{
			return it->second;
		}
	}
	
	return empty;
}


Unit_TextEditor::Unit_TextEditor(QWidget *parent)
	: IUnit(parent)
{
	setCentralWidget(editor = new QsciScintilla(this));	
}

QString Unit_TextEditor::GetText()
{
	return text;
}

void Unit_TextEditor::SaveState( QSettings &/*set*/ )
{
}

void Unit_TextEditor::LoadState( QSettings &/*set*/ )
{
}

void Unit_TextEditor::Create( IUnit *createdFrom )
{
	if (hostPanel = dynamic_cast<IPanel *>(createdFrom))
	{
		const FileInfo *const info = hostPanel->GetCurrentFile();
		text = info->name;
		path = info->path;
		emit TextChanged();
		
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

