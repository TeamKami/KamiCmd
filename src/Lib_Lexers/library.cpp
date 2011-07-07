#include "Lib_Lexers/library.h"
#include "IUnitManager.h"
#include "Unit_TextEditor/ILexerPlugin.h"
#include "Lib_Lexers/getLexers.h"

class Unit_Lexers : public IUnit
{
    Q_OBJECT
};

QVector<Module *> Lexers_Library::ListModulesAndGetCore( ICoreFunctions *core )
{
	g_Core = core;

	QVector<Module *> arr;

	size_t id = 0;

	foreach(ILexer * lexer, getLexers())
	{
		arr << new Module("LexerModule", 1, lexer->getName(), 1, id++);
	}

	return arr;
}

QObject* Lexers_Library::CreateModuleInstance(int id, QObject *parent)
{
	if (id < getLexers().size())
	{
		return dynamic_cast<QObject*>(getLexers()[id]);
	}
	return NULL;
}

QT_BEGIN_NAMESPACE
Q_EXPORT_PLUGIN2(Lib_Lexers, Lexers_Library)
QT_END_NAMESPACE
