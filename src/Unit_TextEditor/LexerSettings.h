#ifndef LEXERSSDIALOG_H
#define LEXERSDIALOG_H

#include <QWidget>
#include "ui_LexerSettings.h"
#include <QThread>

class QsciLexer;

/**
 * \headerfile Unit_TextEditor/LexersSettings.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Lexer settings page widget
 */
class LexerSettings: public QWidget
{
    Q_OBJECT

public:
    /**
     * \param parent Parent passed to QWidget(QWidget*)
     */
    LexerSettings(QWidget *parent = 0);

    ~LexerSettings();

protected:
    void showEvent(QShowEvent * event);

private:
    Ui::LexerSettings ui;

public slots:
    void save();
signals:
    void updateLexers();
};

#endif // LEXERSDIALOG_H
