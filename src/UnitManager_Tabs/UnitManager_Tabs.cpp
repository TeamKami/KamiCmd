#include "library.h"
#include "UnitManager_Tabs.h"
#include "LinkedUnit.h"
#include "ActionManager.h"

int UnitManager_Tabs::AddUnit( IUnit *unit, bool isNextToActive, bool doActivate /*= false*/ )
{
	int index;
	if (isNextToActive)
		index = tabs->insertTab(tabs->currentIndex() + 1, unit, unit->GetText());
	else
		index = tabs->addTab(unit, unit->GetText());
	connect(unit, SIGNAL(TextChanged()), this, SLOT(UnitTextChanged()));
	if (doActivate)
		tabs->setCurrentIndex(index);
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
	// Register resources from "Resources" subfolder
	QDir resourcesDir(QApplication::applicationDirPath());
	resourcesDir.cd("Resources");
	foreach (QString fileName, resourcesDir.entryList(QStringList("*.rcc"), QDir::Files))
		QResource::registerResource(resourcesDir.absoluteFilePath(fileName));

	// Init tabs
	tabs = new TabWidget(this);
	setCentralWidget(tabs);
	tabs->setFocusPolicy(Qt::NoFocus);
	tabs->setDocumentMode(true);
	tabs->setMovable(true);
	connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(CurrentUnitChanged(int)));
	connect(tabs->tabBar_, SIGNAL(TabMousePressed(int, QMouseEvent *)), this, SLOT(TabMousePressed(int, QMouseEvent *)));
	connect(tabs->tabBar_, SIGNAL(TabMouseDoubleClicked(int, QMouseEvent *)), this, SLOT(TabMouseDoubleClicked(int, QMouseEvent *)));
	connect(tabs->tabBar_, SIGNAL(tabCloseRequested(int)), this, SLOT(CloseUnit(int)));
	//tabs->setTabsClosable(true);

	// Initializing actions
	InitActions();

	// Loading FButtonsBar module
	if (QToolBar *buttons = qobject_cast<QToolBar *>(g_Core->QueryModule("FButtonsBar", 1)))
		AddBar(Qt::BottomToolBarArea, buttons);
	else
		g_Core->DebugWrite("UnitManager", "ButtonsBar module not found", ICoreFunctions::Error);

	// Moving window to the center of screen
	move(QApplication::desktop()->screenGeometry().center() - rect().center());

	// Loading setting
	LoadState();

	// DEMO: Adding console unit
	if (IUnit *console = dynamic_cast<IUnit *>(g_Core->QueryModule("ConsoleUnit", 1)))
	{
		AddUnit(console, true);
		console->Create(GetActiveUnit());
	}

	// Showing window
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

void UnitManager_Tabs::Close_Pressed()
{
	if (!dynamic_cast<LinkedUnit *>(GetActiveUnit()) && !dynamic_cast<IPanel *>(GetActiveUnit()))
	{
		CloseUnit(tabs->currentIndex());
	}
}

void UnitManager_Tabs::InitActions()
{
	// Loading ActionManager module
	ActionManager *am;
	if (!(am = dynamic_cast<ActionManager *>(g_Core->QueryModule("ActionManager", 1))))
		g_Core->DebugWrite("UnitManager", "ActionManager module not found", ICoreFunctions::Error);

	Actions << new QAction(QIcon(), tr("Close active tab"), this);
	Actions.last()->setShortcut(Qt::Key_Escape);
	Actions.last()->setShortcutContext(Qt::WindowShortcut);
	Actions.last()->setData("UnitManager_Tabs");
	connect(Actions.last(), SIGNAL(triggered()), this, SLOT(Close_Pressed()));

	// Empty main toolbar
	// 	QToolBar *toolBar = new QToolBar(this);
	// 	AddBar(Qt::TopToolBarArea, toolBar);
	// 	toolBar->setWindowTitle(tr("Main Toolbar"));

	am->RegisterActions(Actions);
	addActions(Actions);
}
