#include "library.h"
#include "UnitManager_Tabs.h"
#include "LinkedUnit.h"
#include "ButtonsBar.h"
#include "ModulesDialog.h"
#include "Associations.h"

#include "../IFileCopy.h"


int UnitManager_Tabs::AddUnit( IUnit *unit, bool isNextToActive )
{
	int index;
	if (isNextToActive)
		index = tabs->insertTab(tabs->currentIndex() + 1, unit, unit->GetText());
	else
		index = tabs->addTab(unit, unit->GetText());
	connect(unit, SIGNAL(TextChanged()), this, SLOT(UnitTextChanged()));
	return index;
}

IUnit * UnitManager_Tabs::GetActiveUnit()
{
	return static_cast<IUnit *>(tabs->currentWidget());
}

void UnitManager_Tabs::LinkUnits( int indexA, int indexB )
{
	LinkedUnit *activeLinked = dynamic_cast<LinkedUnit *>(tabs->widget(indexA)),
		*passiveLinked = dynamic_cast<LinkedUnit *>(tabs->widget(indexB));
	IUnit *active = static_cast<IUnit *>(tabs->widget(indexA)),
		*passive = static_cast<IUnit *>(tabs->widget(indexB));

	if (activeLinked || passiveLinked)
	{
		if (activeLinked == passiveLinked || passiveLinked) // Break link when passive is link or active == passive
		{
			IUnit *left = passiveLinked->GetLeftUnit(),
				*right = passiveLinked->GetRightUnit(),
				*active = passiveLinked->GetActiveUnit();
			tabs->removeTab(indexB);
			tabs->insertTab(indexB, left, left->GetText());
			tabs->insertTab(indexB + 1, right, right->GetText());

			if (activeLinked == passiveLinked)
				tabs->setCurrentIndex(tabs->indexOf(active));
			delete passiveLinked;
		}
		else if (activeLinked) // Relink inactive unit in LinkedUnit with indexB
		{
			IUnit *left = activeLinked->GetLeftUnit(),
				*right = activeLinked->GetRightUnit(),
				*active = activeLinked->GetActiveUnit();
			IUnit *inactive = active == left ? right : left;

			if (LinkedUnit *link = dynamic_cast<LinkedUnit *>(g_Core->QueryModule("LinkedUnit", 1)))
			{
				link->Create(active, passive, active == left);
				tabs->setCurrentIndex(tabs->insertTab(indexA, link, link->GetText()));
				tabs->insertTab(indexB, inactive, inactive->GetText());
				connect(link, SIGNAL(TextChanged()), this, SLOT(UnitTextChanged()));
				active->setFocus();
				delete activeLinked;
			}
			else
				g_Core->DebugWrite("UnitManager", "LinkedUnit module not found", ICoreFunctions::Error);
		}
		else // Both units are linked, supposedly to swap their inactive units
			g_Core->DebugWrite("UnitManager", "Unsupported yet", ICoreFunctions::Error); // btw it never happens
	}
	else if (indexA != indexB) // Link 2 single units
	{
		if (LinkedUnit *link = dynamic_cast<LinkedUnit *>(g_Core->QueryModule("LinkedUnit", 1)))
		{
			link->Create(active, passive, indexA < indexB);
			tabs->setCurrentIndex(tabs->insertTab(indexA - (indexB < indexA), link, link->GetText()));
			connect(link, SIGNAL(TextChanged()), this, SLOT(UnitTextChanged()));
			active->setFocus();
		}
		else
			g_Core->DebugWrite("UnitManager", "LinkedUnit module not found", ICoreFunctions::Error);
	}
// 	else // Duplicate unit
// 		MBox("Unsupported yet");
}

void UnitManager_Tabs::CloseUnit( int index )
{
	if (tabs->count() != 1)
	{
		QWidget *widget = tabs->widget(index);
		tabs->removeTab(index);
		delete widget;
	}
}

void UnitManager_Tabs::AddNewPanels()
{
	IPanel *cur = dynamic_cast<IPanel *>(tabs->widget(tabs->currentIndex()));
	LinkedUnit *curLink = dynamic_cast<LinkedUnit *>(tabs->widget(tabs->currentIndex()));
	QString pathA, pathB = QApplication::applicationDirPath();

	if (cur)
		pathA = cur->GetPath();
	else if (curLink)
	{
		IPanel *curLinkLeft = dynamic_cast<IPanel *>(curLink->GetLeftUnit());
		IPanel *curLinkRight = dynamic_cast<IPanel *>(curLink->GetRightUnit());
		pathA = curLinkLeft->GetPath();
		pathB = curLinkRight->GetPath();
	}

	if (IPanel *panel = dynamic_cast<IPanel *>(g_Core->QueryModule("Panel", 1)))
	{
		int index1 = AddUnit(panel, false);
		panel->SetPath(pathA);

		IPanel *panel2;
		int index2 = AddUnit(panel2 = dynamic_cast<IPanel *>(g_Core->QueryModule("Panel", 1)), false);
		panel2->SetPath(pathB);

		LinkUnits(index1, index2);
	}
	else
		g_Core->DebugWrite("UnitManager_Tabs", "Panel module not found", ICoreFunctions::Error);
}

void UnitManager_Tabs::AddBar( Qt::ToolBarArea area, QToolBar *bar )
{
	addToolBar(area, bar);
	//bar->addWidget(tabs->tabBar_);
}


void UnitManager_Tabs::Start()
{
	QDir resourcesDir(QApplication::applicationDirPath());
	resourcesDir.cd("Resources");

	foreach (QString fileName, resourcesDir.entryList(QStringList("*.rcc"), QDir::Files))
		QResource::registerResource(resourcesDir.absoluteFilePath(fileName));

	F.resize(13);
	F[1] = new QAction(QIcon(":/Images/F1.png"), tr("F1 Help"), this);
	F[2] = new QAction(QIcon(":/Images/F2.png"), tr("F2 Assc"), this);
	F[3] = new QAction(QIcon(":/Images/F3.png"), tr("F3 View"), this);
	F[4] = new QAction(QIcon(":/Images/F4.png"), tr("F4 Edit"), this);
	F[5] = new QAction(QIcon(":/Images/F5.png"), tr("F5 Copy"), this);
	F[6] = new QAction(QIcon(":/Images/F6.png"), tr("F6 Move"), this);
	F[7] = new QAction(QIcon(":/Images/F7.png"), tr("F7 MkDir"), this);
	F[8] = new QAction(QIcon(":/Images/F8.png"), tr("F8 Delete"), this);
	F[9] = new QAction(/*QIcon(":/Images/F9.png"), */tr("F9 ?"), this);
	F[10] = new QAction(/*QIcon(":/Images/F10.png"), */tr("F10 Exit"), this);
	F[11] = new QAction(/*QIcon(":/Images/F11.png"), */tr("F11 Modules"), this);
	F[12] = new QAction(/*QIcon(":/Images/F12.png"), */tr("F12 Tabs"), this);
	QAction *actClose = new QAction(this);
	actClose->setShortcut(Qt::Key_Escape);
	actClose->setShortcutContext(Qt::WindowShortcut);
	connect(actClose, SIGNAL(triggered()), this, SLOT(Close_Pressed()));
	addAction(actClose);

	for (int i = 1; i < F.size(); i++)
		F[i]->setEnabled(false);

	//menuBar = new QMenuBar

	tabs = new TabWidget(this);
	setCentralWidget(tabs);
	tabs->setFocusPolicy(Qt::NoFocus);
	tabs->setDocumentMode(true);
	tabs->setMovable(true);
	//tabs->setTabsClosable(true);

	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(CurrentUnitChanged(int)));
	connect(tabs->tabBar_, SIGNAL(TabMousePressed(int, QMouseEvent *)), this, SLOT(TabMousePressed(int, QMouseEvent *)));
	connect(tabs->tabBar_, SIGNAL(TabMouseDoubleClicked(int, QMouseEvent *)), this, SLOT(TabMouseDoubleClicked(int, QMouseEvent *)));

	connect(F[1], SIGNAL(triggered()), this, SLOT(F1_Pressed()));
	F[1]->setEnabled(true);
	connect(F[2], SIGNAL(triggered()), this, SLOT(F2_Pressed()));
	F[2]->setEnabled(true);
	connect(F[3], SIGNAL(triggered()), this, SLOT(F3_Pressed()));
	F[3]->setEnabled(true);
	connect(F[4], SIGNAL(triggered()), this, SLOT(F4_Pressed()));
//	F[4]->setEnabled(true);
	connect(F[5], SIGNAL(triggered()), this, SLOT(F5_Pressed()));
	F[5]->setEnabled(true);

	connect(F[10], SIGNAL(triggered()), this, SLOT(close()));
	F[10]->setEnabled(true);
	connect(F[11], SIGNAL(triggered()), this, SLOT(F11_Pressed()));
	F[11]->setEnabled(true);
	//connect(tabs->tabBar_, SIGNAL(tabCloseRequested(int)), this, SLOT(TabClose(int)));

	QToolBar *toolBar = new QToolBar(this);
	AddBar(Qt::TopToolBarArea, toolBar);
	toolBar->setWindowTitle(tr("Main Toolbar"));
	if (ButtonsBar *buttons = dynamic_cast<ButtonsBar *>(g_Core->QueryModule("ButtonsBar", 1)))
	{
		buttons->setWindowTitle(tr("Command Buttons Bar"));
		buttons->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		for (int i = 1; i < F.size(); i++)
		{
			buttons->addAction(F[i]);
			F[i]->setShortcut(Qt::Key_F1 + i - 1);
			//if (i == 2 || i == 4 || i == 8)
			if (i != F.size() - 1)
				buttons->addSeparator();
		}
		AddBar(Qt::BottomToolBarArea, buttons);
	}
	else
		g_Core->DebugWrite("UnitManager", "ButtonsBar module not found", ICoreFunctions::Error);


	setGeometry((int)(QApplication::desktop()->width() - (QApplication::desktop()->width() - (QApplication::desktop()->width() / 2)) * 1.5) / 2, (int)(QApplication::desktop()->height() - (QApplication::desktop()->height() - (QApplication::desktop()->height() / 2)) * 1.5) / 2, (int)((QApplication::desktop()->width() - (QApplication::desktop()->width() / 2)) * 1.5), (int)((QApplication::desktop()->height() - (QApplication::desktop()->height() / 2)) * 1.5));

	LoadState();

	if (IUnit *console = dynamic_cast<IUnit *>(g_Core->QueryModule("ConsoleUnit", 1)))
	{
		AddUnit(console, true);
		console->Create(GetActiveUnit());
	}

	show();
}

void UnitManager_Tabs::CurrentUnitChanged( int index )
{
	tabs->setCurrentIndex(index);
}

void UnitManager_Tabs::TabMousePressed( int index, QMouseEvent *event )
{
	if (index != -1 && event->button() == Qt::LeftButton && event->modifiers() & Qt::ControlModifier)
		LinkUnits(tabs->currentIndex(), index);
	else if (event->button() == Qt::MidButton)
	{
		if (index == -1) // Click outside of tabs
			AddNewPanels();
		else // Click on tabs
			CloseUnit(index);
	}
}

void UnitManager_Tabs::TabMouseDoubleClicked( int index, QMouseEvent *event )
{
	if (event->button() == Qt::LeftButton)
	{
		if (index == -1) // DoubleClick outside of tabs
			AddNewPanels();
// 		else // DoubleClick on tabs to close
// 			CloseUnit(index);
	}
}

void UnitManager_Tabs::UnitTextChanged()
{
	if (IUnit *unit = dynamic_cast<IUnit *>(sender()))
		tabs->setTabText(tabs->indexOf(unit), unit->GetText());
}

void UnitManager_Tabs::closeEvent( QCloseEvent *event )
{
	SaveState();
	QMainWindow::closeEvent(event);
}

void UnitManager_Tabs::SaveState()
{
	QSettings set;
	set.setIniCodec("UTF-8");
	set.beginGroup("UnitManager");
	set.remove("");

	set.beginWriteArray("Units");
	for (int i = 0; i < tabs->count(); i++)
	{
		set.setArrayIndex(i);
		set.setValue("type", dynamic_cast<LinkedUnit *>(tabs->widget(i)) ? "LinkedUnit" : 
			dynamic_cast<IPanel *>(tabs->widget(i)) ? "Panel" : "Unknown");
		static_cast<IUnit *>(tabs->widget(i))->SaveState(set);
	}
	set.endArray();
	set.setValue("ActiveUnit", tabs->currentIndex());
	set.endGroup();

	set.beginGroup("MainWindow");
	set.setValue("geometry", saveGeometry());
	set.setValue("state", saveState());
	set.endGroup();
}

void UnitManager_Tabs::LoadState()
{
	QSettings set;
	set.setIniCodec("UTF-8");

	int n = set.beginReadArray("UnitManager/Units");
	for (int i = 0; i < n; i++)
	{
		set.setArrayIndex(i);
		QString type = set.value("type").toString();
		if (type == "Panel")
		{
			if (IPanel *panel = dynamic_cast<IPanel *>(g_Core->QueryModule("Panel", 1)))
			{
				AddUnit(panel, false);
				panel->LoadState(set);
			}
			else
				g_Core->DebugWrite("UnitManager_Tabs", "Panel module not found", ICoreFunctions::Error);
		}
		else if (type == "LinkedUnit")
		{
			if (LinkedUnit *link = dynamic_cast<LinkedUnit *>(g_Core->QueryModule("LinkedUnit", 1)))
			{
				AddUnit(link, false);
				link->LoadState(set);
			}
			else
				g_Core->DebugWrite("UnitManager", "LinkedUnit module not found", ICoreFunctions::Error);
		}
		//else
		//	g_Core->DebugWrite("UnitManager_Tabs", "Unknown unit found", ICoreFunctions::Warning);
	}
	set.endArray();
	if (!n)
	{
		if (IPanel *panelA = dynamic_cast<IPanel *>(g_Core->QueryModule("Panel", 1)))
		{
			IPanel *panelB = dynamic_cast<IPanel *>(g_Core->QueryModule("Panel", 1));
			panelA->SetPath(QApplication::applicationDirPath());
			panelB->SetPath(QApplication::applicationDirPath());
			LinkUnits(AddUnit(panelA, false), AddUnit(panelB, false));
		}
		else
			g_Core->DebugWrite("UnitManager_Tabs", "Panel module not found", ICoreFunctions::Error);
	}
	tabs->setCurrentIndex(set.value("UnitManager/ActiveUnit", 0).toInt());
	tabs->widget(tabs->currentIndex())->setFocus();

	set.beginGroup("MainWindow");
	restoreGeometry(set.value("geometry").toByteArray());
	restoreState(set.value("state").toByteArray());
	set.endGroup();
}






void UnitManager_Tabs::F11_Pressed()
{
	if (ModulesDialog *modWin = dynamic_cast<ModulesDialog *>(g_Core->QueryModule("ModulesDialog", 1, "", -1, this)))
	{
		modWin->exec();
		delete modWin;
	}
	else
		g_Core->DebugWrite("UnitManager_Tabs", "ModulesDialog module not found", ICoreFunctions::Error);
}

void UnitManager_Tabs::F2_Pressed()
{
	if (Associations *assc = dynamic_cast<Associations *>(g_Core->QueryModule("Associations", 1)))
		assc->ShowDialog(this);
	else
		g_Core->DebugWrite("UnitManager_Tabs", "Associations module not found", ICoreFunctions::Error);
}

void UnitManager_Tabs::F3_Pressed()
{
	if (IUnit *viewer = dynamic_cast<IUnit *>(g_Core->QueryModule("WebViewUnit", 1)))
	{
		LinkedUnit *link = dynamic_cast<LinkedUnit *>(GetActiveUnit());
		if (link)
			viewer->Create(link->GetActiveUnit());
		else
			viewer->Create(GetActiveUnit());
		tabs->setCurrentIndex(AddUnit(viewer, true));
	}
}

void UnitManager_Tabs::F4_Pressed()
{
	if (IUnit *editor = dynamic_cast<IUnit *>(g_Core->QueryModule("TextEditorUnit", 1)))
	{
		LinkedUnit *link = dynamic_cast<LinkedUnit *>(GetActiveUnit());
		if (link)
			editor->Create(link->GetActiveUnit());
		else
			editor->Create(GetActiveUnit());
		tabs->setCurrentIndex(AddUnit(editor, true));
	}
}

void UnitManager_Tabs::F1_Pressed()
{
	if (QDialog *oq = qobject_cast<QDialog *>(g_Core->QueryModule("OperationsQueue", 1, "OQ_Widget")))
	{
		oq->exec();
		delete oq;
	}
	else
		g_Core->DebugWrite("UnitManager_Tabs", "Check sums module not found", ICoreFunctions::Error);
}

void UnitManager_Tabs::F5_Pressed()
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

void UnitManager_Tabs::Close_Pressed()
{
	if (!dynamic_cast<LinkedUnit *>(GetActiveUnit()) && !dynamic_cast<IPanel *>(GetActiveUnit()))
	{
		CloseUnit(tabs->currentIndex());
	}
}
