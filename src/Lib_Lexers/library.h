#ifndef LIB_LEXERS_LIBRARY_H
#define LIB_LEXERS_LIBRARY_H

#include <QtPlugin>
#include "ILibrary.h"

extern ICoreFunctions *g_Core;

/**
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \headerfile Lib_Lexers/library.h
 * \brief Lexers package for Unit_TextEditor
 */
class Lexers_Library : public QObject, public ILibrary
{
	Q_OBJECT
	Q_INTERFACES(ILibrary)

public:
	/**
	 * \brief Return list of availiable modules
	 * \param core Pointer to KamiCmd core
	 */
	QVector<Module *> ListModulesAndGetCore(ICoreFunctions *core);

	/**
	 * \brief Return instance of lexer. All lexers created when library loaded.
	 * \param id Lexer id
	 * \parent Not used, only for compatibility with ILibrary
	 */
	QObject* CreateModuleInstance(int id, QObject *parent);

private:
	ICoreFunctions *g_Core;
};

#endif // LIB_LEXERS_LIBRARY_H
