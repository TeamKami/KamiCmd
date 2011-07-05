#include "FButtonsBar.h"
#include "library.h"
#include "LinkedUnit.h"

#include "ModulesDialog.h"
#include "Associations.h"
#include "../IFileCopy.h"

FButtonsBar::FButtonsBar(QWidget *parent)
	: QToolBar(parent)
{
	if (!(UnitManager = dynamic_cast<IUnitManager *>(g_Core->QueryModule("UnitManager", 1, "", -1, this))))
		g_Core->DebugWrite("FButtonsBar", "ModulesDialog module not found", ICoreFunctions::Error);

	if (!(AM = dynamic_cast<ActionManager *>(g_Core->QueryModule("ActionManager", 1))))
		g_Core->DebugWrite("UnitManager", "ActionManager module not found", ICoreFunctions::Error);

	// Default init of F-keys
	Actions.reserve(12);
	Actions << new QAction(QIcon(":/Images/F1.png"), tr("F1 Help"), this);
	Actions << new QAction(QIcon(":/Images/F2.png"), tr("F2 Assc"), this);
	Actions << new QAction(QIcon(":/Images/F3.png"), tr("F3 View"), this);
	Actions << new QAction(QIcon(":/Images/F4.png"), tr("F4 Edit"), this);
	Actions << new QAction(QIcon(":/Images/F5.png"), tr("F5 Copy"), this);
	Actions << new QAction(QIcon(":/Images/F6.png"), tr("F6 Move"), this);
	Actions << new QAction(QIcon(":/Images/F7.png"), tr("F7 MkDir"), this);
	Actions << new QAction(QIcon(":/Images/F8.png"), tr("F8 Delete"), this);
	Actions << new QAction(/*QIcon(":/Images/F9.png"), */tr("F9 Actions"), this);
	Actions << new QAction(/*QIcon(":/Images/F10.png"), */tr("F10 Exit"), this);
	Actions << new QAction(/*QIcon(":/Images/F11.png"), */tr("F11 Modules"), this);
	Actions << new QAction(/*QIcon(":/Images/F12.png"), */tr("F12 Tabs"), this);
	
	for (int i = 0; i < Actions.size(); i++)
		Actions[i]->setEnabled(false);


	connect(Actions[1 -1], SIGNAL(triggered()), SLOT(F1_Pressed()));
	Actions[1 -1]->setEnabled(true);
	connect(Actions[2 -1], SIGNAL(triggered()), SLOT(F2_Pressed()));
	Actions[2 -1]->setEnabled(true);
	connect(Actions[3 -1], SIGNAL(triggered()), SLOT(F3_Pressed()));
	Actions[3 -1]->setEnabled(true);
	
	connect(Actions[4 -1], SIGNAL(triggered()), SLOT(F4_Pressed()));
	Actions[4 -1]->setEnabled(true);

	connect(Actions[5 -1], SIGNAL(triggered()), SLOT(F5_Pressed()));
	Actions[5 -1]->setEnabled(true);

	connect(Actions[9 -1], SIGNAL(triggered()), SLOT(F9_Pressed()));
	Actions[9 -1]->setEnabled(true);
	connect(Actions[10 -1], SIGNAL(triggered()), SLOT(close()));
	Actions[10 -1]->setEnabled(true);
	connect(Actions[11 -1], SIGNAL(triggered()), SLOT(F11_Pressed()));
	Actions[11 -1]->setEnabled(true);

	
	
	setWindowTitle(tr("Command Buttons Bar"));
	setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	for (int i = 0; i < Actions.size(); i++)
	{
		Actions[i]->setShortcut(Qt::Key_F1 + i);
		Actions[i]->setData("FButtonsBar");
		addAction(Actions[i]);
		//if (i == 2 || i == 4 || i == 8)
		if (i != Actions.size() - 1)
			addSeparator();
	}

	AM->RegisterActions(Actions);
}

FButtonsBar::~FButtonsBar()
{

}

void FButtonsBar::F11_Pressed()
{
	if (ModulesDialog *modWin = dynamic_cast<ModulesDialog *>(g_Core->QueryModule("ModulesDialog", 1, "", -1, this)))
	{
		modWin->exec();
		delete modWin;
	}
	else
		g_Core->DebugWrite("UnitManager_Tabs", "ModulesDialog module not found", ICoreFunctions::Error);
}

void FButtonsBar::F2_Pressed()
{
	if (Associations *assc = dynamic_cast<Associations *>(g_Core->QueryModule("Associations", 1)))
		assc->ShowDialog(this);
	else
		g_Core->DebugWrite("UnitManager_Tabs", "Associations module not found", ICoreFunctions::Error);
}

void FButtonsBar::F3_Pressed()
{
	if (IUnit *viewer = dynamic_cast<IUnit *>(g_Core->QueryModule("PictureViewUnit", 1)))
	{
		LinkedUnit *link = dynamic_cast<LinkedUnit *>(UnitManager->GetActiveUnit());
		if (link)
			viewer->Create(link->GetActiveUnit());
		else
			viewer->Create(UnitManager->GetActiveUnit());
		UnitManager->AddUnit(viewer, true, true);
	}
}

void FButtonsBar::F4_Pressed()
{
	if (IUnit *editor = dynamic_cast<IUnit *>(g_Core->QueryModule("TextEditorUnit", 1)))
	{
		LinkedUnit *link = dynamic_cast<LinkedUnit *>(UnitManager->GetActiveUnit());
		if (link)
			editor->Create(link->GetActiveUnit());
		else
			editor->Create(UnitManager->GetActiveUnit());
		UnitManager->AddUnit(editor, true, true);
	}
}

void FButtonsBar::F1_Pressed()
{
	if (QDialog *oq = qobject_cast<QDialog *>(g_Core->QueryModule("OperationsQueue", 1, "OQ_Widget")))
	{
		oq->exec();
		delete oq;
	}
	else
		g_Core->DebugWrite("UnitManager_Tabs", "Check sums module not found", ICoreFunctions::Error);
}

void FButtonsBar::F5_Pressed()
{
	if (QDialog *copyDialog = dynamic_cast<QDialog *>(g_Core->QueryModule("FileCopy", 1, "FileCopy_Widget", -1, this)))
	{
		copyDialog->show();
		//	delete copyDialog;
		// 		IOperationsQueue *operationsQueue_;
		// 		if (!(operationsQueue_ = dynamic_cast<IOperationsQueue *>(g_Core->QueryModule("OperationsQueue", 1, "OQ"))))
		// 		{
		// 			g_Core->DebugWrite("UnitManager_Tabs", "Operations Queue not found", ICoreFunctions::Error);
		// 			return;
		// 		}
		// 		operationsQueue_->Add(*oq, oq->GetState());
	}
	else
		g_Core->DebugWrite("UnitManager_Tabs", "File Copy not found", ICoreFunctions::Error);
}

void FButtonsBar::F9_Pressed()
{
	AM->ShowDialog(this);
}
