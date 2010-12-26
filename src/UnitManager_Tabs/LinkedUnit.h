#ifndef LINKEDUNIT_H
#define LINKEDUNIT_H

#include <QtGui>
#include "../IUnitManager.h"

class SplitterHandle : public QSplitterHandle
{
	Q_OBJECT

public:
	SplitterHandle(Qt::Orientation orientation, QSplitter * parent) : QSplitterHandle(orientation, parent) {}; // overload
	void mouseDoubleClickEvent(QMouseEvent *event); // overload

signals:
	void MouseDoubleClicked();
};


class Splitter : public QSplitter
{
	Q_OBJECT

public:
	Splitter(Qt::Orientation orientation, QWidget *parent = 0) : QSplitter(orientation, parent) {}; // overload

protected:
	QSplitterHandle *createHandle();

public slots:
	void MouseDoubleClicked();
};


class LinkedUnit : public IUnit
{
	Q_OBJECT

public:
	LinkedUnit(QWidget *parent);
	void Create(IUnit * /*createdFrom*/) {}; // overload
	void Link(IUnit * /*withUnit*/) {}; // overload
	QString GetText(); // overload
	void SaveState(QSettings &set); // overload
	void LoadState(QSettings &set); // overload

	void Create(IUnit *active, IUnit *passive, bool isActiveLeft);

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