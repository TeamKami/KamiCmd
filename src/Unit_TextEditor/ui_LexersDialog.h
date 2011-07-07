/********************************************************************************
** Form generated from reading UI file 'LexersDialog.ui'
**
** Created: Thu 7. Jul 17:48:04 2011
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEXERSDIALOG_H
#define UI_LEXERSDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_LexersDialog
{
public:
    QAction *actionEditShortcut;
    QAction *actionSelect;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QComboBox *lexers;
    QToolBar *toolBar;

    void setupUi(QMainWindow *LexersDialog)
    {
        if (LexersDialog->objectName().isEmpty())
            LexersDialog->setObjectName(QString::fromUtf8("LexersDialog"));
        LexersDialog->resize(358, 61);
        actionEditShortcut = new QAction(LexersDialog);
        actionEditShortcut->setObjectName(QString::fromUtf8("actionEditShortcut"));
        actionSelect = new QAction(LexersDialog);
        actionSelect->setObjectName(QString::fromUtf8("actionSelect"));
        centralwidget = new QWidget(LexersDialog);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        lexers = new QComboBox(centralwidget);
        lexers->setObjectName(QString::fromUtf8("lexers"));

        verticalLayout->addWidget(lexers);

        LexersDialog->setCentralWidget(centralwidget);
        toolBar = new QToolBar(LexersDialog);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        LexersDialog->addToolBar(Qt::BottomToolBarArea, toolBar);

        toolBar->addAction(actionSelect);

        retranslateUi(LexersDialog);

        QMetaObject::connectSlotsByName(LexersDialog);
    } // setupUi

    void retranslateUi(QMainWindow *LexersDialog)
    {
        LexersDialog->setWindowTitle(QApplication::translate("LexersDialog", "Lexer Manager", 0, QApplication::UnicodeUTF8));
        actionEditShortcut->setText(QApplication::translate("LexersDialog", "Edit Shortcut", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionEditShortcut->setToolTip(QApplication::translate("LexersDialog", "Change shortcuts for selected action", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionEditShortcut->setShortcut(QApplication::translate("LexersDialog", "F4", 0, QApplication::UnicodeUTF8));
        actionSelect->setText(QApplication::translate("LexersDialog", "Select", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_TOOLTIP
        actionSelect->setToolTip(QApplication::translate("LexersDialog", "Use selected lexer", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_TOOLTIP
        actionSelect->setShortcut(QApplication::translate("LexersDialog", "Enter", 0, QApplication::UnicodeUTF8));
        toolBar->setWindowTitle(QApplication::translate("LexersDialog", "toolBar", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class LexersDialog: public Ui_LexersDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEXERSDIALOG_H
