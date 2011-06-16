#ifndef FBUTTONSBAR_H
#define FBUTTONSBAR_H

#include <QToolBar>
#include <QAction>
#include "../IUnitManager.h"
#include "ActionManager.h"

class FButtonsBar : public QToolBar
{
	Q_OBJECT

public:
	FButtonsBar(QWidget *parent);
	~FButtonsBar();

private:
	QList<QAction *> Actions;
	IUnitManager *UnitManager;
	ActionManager *AM;

public slots:
	void F1_Pressed();
	void F2_Pressed();
	void F3_Pressed();
	void F4_Pressed();
	void F5_Pressed();
	void F9_Pressed();
	void F11_Pressed();
};

#endif // FBUTTONSBAR_H
