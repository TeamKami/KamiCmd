#ifndef LINKEDUNIT_H
#define LINKEDUNIT_H

#include <QtGui>
#include "IUnitManager.h"


class SplitterHandle : public QSplitterHandle
{
	Q_OBJECT

public:
	SplitterHandle(Qt::Orientation orientation, QSplitter * parent) : QSplitterHandle(orientation, parent) {};
	virtual void mouseDoubleClickEvent(QMouseEvent *event);

signals:
	void MouseDoubleClicked();
};


class Splitter : public QSplitter
{
	Q_OBJECT

public:
	Splitter(Qt::Orientation orientation, QWidget *parent = 0) : QSplitter(orientation, parent) {};

protected:
	QSplitterHandle *createHandle();

public slots:
	void MouseDoubleClicked();
};


class LinkedUnit : public ILinkedUnit
{
	Q_OBJECT

public:
	LinkedUnit(QWidget *parent);
	virtual void Create(IUnit * /*createdFrom*/) {};
	virtual void Link(IUnit * /*withUnit*/) {};
	virtual QString GetText();
	virtual QIcon GetIcon();
	virtual void SaveState(QSettings &set);
	virtual void LoadState(QSettings &set);

	virtual void Create(IUnit *active, IUnit *inactive, bool isActiveLeft);
	virtual IUnit *GetActiveUnit();
	virtual IUnit *GetInactiveUnit();
	virtual IUnit *GetLeftUnit();
	virtual IUnit *GetRightUnit();

private:
	IUnit *left, *right, *active;

private:
	Splitter *splitter;
	QVBoxLayout *layout;


public slots:
	void LinkedUnitFocusIn();

signals:
	void TextChanged();
};

#endif // LINKEDUNIT_H
