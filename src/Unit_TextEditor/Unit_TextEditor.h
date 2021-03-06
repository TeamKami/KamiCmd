#ifndef UNIT_PICTUREVIEW_H
#define UNIT_PICTUREVIEW_H

#include <QtGui>
#include "IUnitManager.h"
#include "IPanel.h"
#include <QTextEdit>
#include <QScrollBar>

#include "Unit_TextEditor/LexersDialog.h"

class QsciLexer;
class ICoreFunctions;
class QsciScintilla;
class Action;
class ActionManager;
class SciSettings;
struct FileInfo;

/**
 * \headerfile Unit_TextEditor/Unit_TextEditor.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief QScintilla based text editor unit with code highlighting/autocompleteion
 */
class Unit_TextEditor: public IUnit
{
    Q_OBJECT

public:
    Unit_TextEditor(QWidget *parent = 0, SciSettings * settings = 0,
            ICoreFunctions * core = 0);
    ~Unit_TextEditor();
    virtual void Create(IUnit *createdFrom);
    virtual void Link(IUnit * /*withUnit*/)
    {
    }

    virtual QString GetText();
    virtual QIcon GetIcon();

    virtual void SaveState(QSettings &set);
    virtual void LoadState(QSettings &set);

private:
    void loadModules();

    QString text;
    QString path;
    bool edited;
    QsciScintilla * editor;
    IPanel *hostPanel;
    LexersDialog * ld;
    QList<QAction *> Actions;
    IActionManager *am;
    SciSettings * settings_;
    ICoreFunctions * g_Core;
    const FileInfo * info;

private slots:
    void onEdit();
    void selectLexer();
    void save();
    void setLexer(QsciLexer * l);
    void updateSettings();

signals:
    void TextChanged();
};

#endif // UNIT_PICTUREVIEW_H
