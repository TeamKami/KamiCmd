#ifndef UNITMANAGER_INTERFACE_H
#define UNITMANAGER_INTERFACE_H

#include <QtGui>

class IUnit : public QMainWindow//QWidget
{
public:
	IUnit(QWidget *parent) : QMainWindow(parent) { setLayout(new QVBoxLayout(this)); }
	virtual void Create(IUnit *createdFrom = NULL) = 0;
	virtual void Link(IUnit *withUnit = NULL) = 0;
	virtual void Close() {}
	virtual QString GetText() = 0;
	virtual QIcon GetIcon() = 0;
	virtual void SaveState(QSettings &set) = 0;
	virtual void LoadState(QSettings &set) = 0;

signals:
	void TextChanged();
	void IconChanged();
	void FocusIn();
};

class ILinkedUnit : public IUnit
{
public:
	ILinkedUnit(QWidget *parent) : IUnit(parent) {}
	virtual void Create(IUnit *active, IUnit *inactive, bool isActiveLeft) = 0;
	virtual IUnit *GetActiveUnit() = 0;
	virtual IUnit *GetInactiveUnit() = 0;
	virtual IUnit *GetLeftUnit() = 0;
	virtual IUnit *GetRightUnit() = 0;
};

class IUnitManager
{
public:
	virtual int  AddUnit(IUnit *unit, bool isNextToActive, bool doActivate = false) = 0;
	virtual IUnit *GetActiveUnit() = 0;
	virtual void LinkUnits(int indexA, int indexB) = 0;
	virtual void CloseUnit(int index) = 0;

	virtual void AddBar(Qt::ToolBarArea area, QToolBar *bar) = 0;

	virtual void Start() = 0;
	virtual void SaveState() = 0;
	virtual void LoadState() = 0;

	virtual QMainWindow *GetMainWindow() = 0;
};

class IActionManager
{
public:
    virtual void RegisterActions(QList<QAction *> actions) = 0;
    virtual void UnregisterActions(QList<QAction *> actions) = 0;
};



#endif // UNITMANAGER_INTERFACE_H
