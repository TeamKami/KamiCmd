#ifndef UNITMANAGER_TABS_H
#define UNITMANAGER_TABS_H

#include "../IUnitManager.h"
#include "../IPanel.h"
#include "TabWidget.h"


class UnitManager_Tabs : public QMainWindow, public IUnitManager
{
	Q_OBJECT

public:
	UnitManager_Tabs(QWidget *parent) : QMainWindow(parent) {};
	virtual int  AddUnit(IUnit *unit, bool isNextToActive);
	virtual IUnit *GetActiveUnit();
	virtual void LinkUnits(int indexA, int indexB);
	virtual void CloseUnit(int index);
	virtual void AddBar(Qt::ToolBarArea area, QToolBar *bar);
	virtual void Start();
	virtual void SaveState();
	virtual void LoadState();

	virtual void closeEvent(QCloseEvent *event);
	void AddNewPanels();

private:
	TabWidget *tabs;
	QVector<QAction *> F;
	QMenuBar *menuBar;

public slots:
	void CurrentUnitChanged(int index);
	void TabMousePressed(int index, QMouseEvent *event);
	void TabMouseDoubleClicked(int index, QMouseEvent *event);
	void UnitTextChanged();
	void F1_Pressed();
	void F2_Pressed();
	void F3_Pressed();
	void F4_Pressed();
	void F5_Pressed();
	void F11_Pressed();
	void Close_Pressed();
};

#endif // UNITMANAGER_TABS_H
