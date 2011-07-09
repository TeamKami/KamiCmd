#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include <QWidget>
#include "ui_EditorSettings.h"

class SciSettings;

/**
 * \headerfile Unit_TextEditor/EditorSettings.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Editor settings page widget
 */
class EditorSettings: public QWidget
{
    Q_OBJECT

public:
    /**
     * \param parent Parent passed to QWidget(QWidget*)
     * \sci Pointer to corresponding SciSettings object
     */
    EditorSettings(QWidget *parent = 0, SciSettings * sci = 0);
    ~EditorSettings();

private:
    Ui::EditorSettings ui;
    SciSettings * sci_;

public slots:
    void save();
};

#endif // EDITORSETTINGS_H
