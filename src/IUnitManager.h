#ifndef UNITMANAGER_INTERFACE_H
#define UNITMANAGER_INTERFACE_H

#include <QtGui>

class IUnit : public QWidget
{
public:
	IUnit(QWidget *parent) : QWidget(parent) {};
	virtual void Create(IUnit *createdFrom) = 0;
	virtual void Link(IUnit *withUnit) = 0;
	virtual void Unlink() {};
	virtual void Close() {};

	virtual void Activated() {};
	virtual void Deactivated() {};

	virtual QString GetText() = 0;

	virtual void SaveState(QSettings &set) = 0;
	virtual void LoadState(QSettings &set) = 0;

signals:
	void TextChanged();
	void FocusIn();
};

class ILinkedUnit : public IUnit
{
public:
	ILinkedUnit(QWidget *parent) : IUnit(parent) {};

	virtual void Create(IUnit *active, IUnit *passive, bool isActiveLeft) = 0;
	virtual IUnit *GetActiveUnit() = 0;
	virtual IUnit *GetPassiveUnit() = 0;

	virtual IUnit *GetLeftUnit() = 0;
	virtual IUnit *GetRightUnit() = 0;

};


class IUnitManager
{
public:
	virtual void Start() = 0;
	virtual int  AddUnit(IUnit *unit) = 0;
	virtual void AddBar(Qt::ToolBarArea area, QToolBar *bar) = 0;
	virtual void Link(int indexA, int indexB) = 0;
	virtual void Close(int index) = 0;
	
	virtual IUnit *GetActiveUnit() = 0;

	virtual void SaveState() = 0;
	virtual void LoadState() = 0;
};

#endif // UNITMANAGER_INTERFACE_H
