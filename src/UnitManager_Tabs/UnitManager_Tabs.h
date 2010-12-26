#ifndef UNITMANAGER_TABS_H
#define UNITMANAGER_TABS_H

#ifdef UNITMANAGER_TABS_LIB
# define UNITMANAGER_TABS_EXPORT Q_DECL_EXPORT
#else
# define UNITMANAGER_TABS_EXPORT Q_DECL_IMPORT
#endif

#include "../IUnitManager.h"
#include "../IPanel.h"
#include "TabWidget.h"


class UnitManager_Tabs : public QMainWindow, public IUnitManager
{
	Q_OBJECT

	// Inherited
public:
	UnitManager_Tabs(QWidget *parent) : QMainWindow(parent) {};

	void Start();
	int  AddUnit(IUnit *unit);
	void AddBar(Qt::ToolBarArea area, QToolBar *bar);
	void Link(int indexA, int indexB);
	void Close(int index);

	IUnit *GetActiveUnit();

	void SaveState();
	void LoadState();

	// Local
	void closeEvent(QCloseEvent *event);
	void AddNew();

private:
	TabWidget *tabs;
	QVector<QAction *> F;
	QMenuBar *menuBar;

public slots:
	void currentChanged(int index);
	void TabMousePressed(int index, QMouseEvent *event);
	void TabMouseDoubleClicked(int index, QMouseEvent *event);
	void UnitTextChanged();
	void F1_Pressed();
	void F2_Pressed();
	void F3_Pressed();
	void F4_Pressed();
	void F11_Pressed();
};

#endif // UNITMANAGER_TABS_H
