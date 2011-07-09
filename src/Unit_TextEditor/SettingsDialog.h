#ifndef LEXERSSDIALOG_H
#define LEXERSDIALOG_H

#include <QDialog>
#include "ui_SettingsDialog.h"
#include <map>

class QsciScintilla;
class SciSettings;

/**
 * \headerfile Unit_TextEditor/SettingsDialog.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Settings dialog
 */
class SettingsDialog: public QDialog
{
    Q_OBJECT

public:
    /**
     * \param parent passed to QWidget(QWidget*)
     * \param set poiter to corresponding instance of SciSettings
     */
    SettingsDialog(QWidget * parent = 0, SciSettings * set = 0);

    ~SettingsDialog();

    /**
     * \brief show dialog
     */
    void show();

private:
    Ui::SettingsDialog ui;
    bool created;
    SciSettings * settings;

public slots:
    void onDialogButton(QAbstractButton * button);
    void onPageSelected(QTreeWidgetItem * item, int column);signals:
    void saveSettings();
    void settingsChanged();
};

#endif // LEXERSDIALOG_H
