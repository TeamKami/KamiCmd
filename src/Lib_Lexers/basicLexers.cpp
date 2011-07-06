#include "../Unit_TextEditor/ILexerPlugin.h"

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

#include "ObjectWrapper.h"

std::vector<ILexer *> & getLexers()
{
	static std::vector<ILexer*> lexers;
	if (lexers.empty())
	{
		#define LEXER(cls, name, wildcard) LEXER_R(cls, name, extension() | wildcard)
		#define LEXER_N(cls, name) LEXER_R(cls, name, extension())
		#define LEXER_R(cls, name, wildcard) lexers.push_back(new LexerWrapper<cls>(#name, wildcard))

		LEXER(QsciLexerBash, Bash, "*.sh");
		LEXER(QsciLexerBatch, Batch, "*.bat");
		LEXER(QsciLexerCMake, CMake, "*.cmake" | "CMakeLists.txt");
		LEXER(QsciLexerCPP, CPP, "*.cpp" | "*.cxx" | "*.c" | "*.h" | "*.hpp");
		LEXER(QsciLexerCSharp, CSharp, "*.cs");
		LEXER(QsciLexerCSS, CSS, "*.css");
		LEXER(QsciLexerD, D, "*.d");
		LEXER(QsciLexerDiff, Diff, "*.diff" | "*.patch");
		LEXER_N(QsciLexerFortran, Fortran);
		LEXER_N(QsciLexerFortran77, Fortran77);
		LEXER(QsciLexerHTML, HTML, "*.html" | "*.htm" | "*.xhtml");
		LEXER_N(QsciLexerIDL, IDL);
		LEXER(QsciLexerJava, Java, "*.java");
		LEXER(QsciLexerJavaScript, JavaScript, "*.js");
		LEXER(QsciLexerLua, Lua, "*.lua");
		LEXER(QsciLexerMakefile, Makefile, "*.m4" | "Makefile");
		LEXER_N(QsciLexerMatlab, Matlab);
		LEXER_N(QsciLexerOctave, Octave);
		LEXER(QsciLexerPascal, Pascal, "*.pas");
		LEXER(QsciLexerPerl, Perl, "*.pl");
		LEXER(QsciLexerPostScript, PostScript, "*.ps");
		LEXER_N(QsciLexerPOV, POV);
		LEXER(QsciLexerPython, Python, "*.py");
		LEXER_N(QsciLexerRuby, Ruby);
		LEXER_N(QsciLexerSpice, Spice);
		LEXER(QsciLexerSQL, SQL, "*.sql");
		LEXER_N(QsciLexerTCL, TCL);
		LEXER(QsciLexerTeX, TeX, "*.tex");
		LEXER_N(QsciLexerVerilog, Verilog);
		LEXER_N(QsciLexerVHDL, VHDL);
		LEXER(QsciLexerXML, XML, "*.xml" | "*.xsl");
		LEXER_N(QsciLexerYAML, YAML);

		#undef LEXER
		#undef LEXER_N
		#undef LEXER_R
	}

	return lexers;
}
