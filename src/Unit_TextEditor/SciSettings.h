#ifndef SCISETTINGS_H
#define SCISETTINGS_H

#include <QObject>
#include <QVector>
#include <QSettings>
#include <QList>
#include "ui_EditorSettings.h"

class QsciScintilla;
class SettingsDialog;
class ICoreFunctions;

/**
 * \headerfile Unit_TextEditor/SciSettings.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Single setting of editor, used only in SciSettings
 */
class SciSetting
{
public:
    virtual void apply(QsciScintilla * editor) = 0;
    virtual void load(QsciScintilla * editor) = 0;
    virtual void read(QSettings & set) = 0;
    virtual void write(QSettings & set) = 0;
    virtual void setUi(Ui::EditorSettings & ui) = 0;
    virtual void getUi(Ui::EditorSettings & ui) = 0;
};

/**
 * \headerfile Unit_TextEditor/SciSettings.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief
 */
class SciSettings: public QObject
{
    Q_OBJECT
public:
    /**
     * \param parent passed to QObject(QObject*)
     * \param core pointer to KamiCmd core
     */
    SciSettings(QObject * parent = 0, ICoreFunctions *core = 0);

    ~SciSettings();

    /**
     * \param editor pointer to instance of QsciScintilla
     * \brief set settings of editor
     */
    void applySettings(QsciScintilla * editor);

    /**
     * \param ui reference to EditorSettings ui
     * \brief update ui state based on settings
     */
    void setUi(Ui::EditorSettings & ui);

    /**
     * \param ui reference to EditorSettings ui
     * \brief update settings based on ui state
     */
    void getUi(Ui::EditorSettings & ui);

signals:
    void settingsChanged();
public slots:
    void showSettings();

private:
    void loadSettings();
    void saveSettings();
    QVector<SciSetting*> settings;
    SettingsDialog * sd;
    QList<QAction *> Actions;
    ICoreFunctions *g_Core;
private slots:
    void settingsUpdated();
};

#endif //SCISETTINGS_H
