#ifndef LEXERSSDIALOG_H
#define LEXERSDIALOG_H

#include <QMainWindow>
#include "ui_LexersDialog.h"
#include <QVector>

class QsciLexer;
class ILexer;

/**
 * \headerfile Unit_TextEditor/LexersDialog.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Lexer selection dialog
 */
class LexersDialog: public QMainWindow
{
    Q_OBJECT

public:
    /**
     * \param parent passed to QWidget(QWidget*)
     */
    LexersDialog(QWidget *parent = 0);

    ~LexersDialog();

    /**
     * \brief open dialog
     * \param lexers Array of availiable lexers
     * \param lexer Pointer to current lexer
     */
    void show(QVector<ILexer*> const & lexers, QsciLexer * lexer);

private:
    Ui::LexersDialog ui;

public slots:
    void on_actionSelect_triggered();
signals:
    void setLexer(QsciLexer * l);
};

#endif // LEXERSDIALOG_H
