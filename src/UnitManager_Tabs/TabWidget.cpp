#include "TabWidget.h"

void TabBar::mousePressEvent( QMouseEvent *event )
{
	int index = -1;
	for (int i = 0; i < count(); i++)
		if (tabRect(i).contains(event->pos()))
		{
			index = i;
			break;
		}
	emit TabMousePressed(index, event);

	mousePressCurrentIndex = currentIndex();
	mousePressIndex = index;

	if (event->button() == Qt::LeftButton && event->modifiers() & Qt::ControlModifier)
		return;
	else
		QTabBar::mousePressEvent(event);
}

void TabBar::mouseReleaseEvent( QMouseEvent *event )
{
	int index = -1;
	for (int i = 0; i < count(); i++)
		if (tabRect(i).contains(event->pos()))
		{
			index = i;
			break;
		}
	if (event->button() == Qt::LeftButton && event->modifiers() & Qt::ControlModifier)
		return;
	else if (index == mousePressCurrentIndex && mousePressIndex != -1)
		ActivateLastActive();
	QTabBar::mouseReleaseEvent(event);
}


void TabBar::mouseDoubleClickEvent( QMouseEvent *event )
{
	int index = -1;
	for (int i = 0; i < count(); i++)
		if (tabRect(i).contains(event->pos()))
		{
			index = i;
			break;
		}
	emit TabMouseDoubleClicked(index, event);

 	if (index != currentIndex())
	 	mousePressCurrentIndex = -1;

	if (mousePressIndex != -1)
		QTabBar::mouseDoubleClickEvent(event);
	setCurrentIndex(index);
}

void TabBar::TabMoved( int from, int to )
{
	qSwap(tabsFocus[from], tabsFocus[to]);
}

TabBar::TabBar( QWidget *parent )
	: QTabBar(parent)
{
	connect(this, SIGNAL(tabMoved(int, int)), SLOT(TabMoved(int, int)));
	connect(this, SIGNAL(currentChanged(int)), SLOT(CurrentChanged(int)));
}

void TabBar::tabRemoved( int index )
{
	for(int i = 0; i < tabsFocus.size(); i++)
		if (tabsFocus[i] > tabsFocus[index])
			tabsFocus[i]--;
	tabsFocus.removeAt(index);
}

void TabBar::tabInserted( int index )
{
	for(int i = 0; i < tabsFocus.size(); i++)
		tabsFocus[i]++;
	tabsFocus.insert(index, 0);
}

void TabBar::CurrentChanged( int index )
{
	if (index == -1)
		return;

	for (int i = 0; i < tabsFocus.size(); i++)
		if (tabsFocus[i] > tabsFocus[index])
			tabsFocus[i]--;
	if (index < tabsFocus.size())
		tabsFocus[index] = tabsFocus.size() - 1;
}

void TabBar::ActivateLastActive()
{
	int lastActive = currentIndex() - 1;
	for (int i = 0; i < tabsFocus.size(); i++)
		if (tabsFocus[i] == tabsFocus[currentIndex()] - 1)
		{
			lastActive = i;
			break;
		}

	for (int i = 0; i < tabsFocus.size(); i++)
		tabsFocus[i]++;
	tabsFocus[currentIndex()] = 0;
	
	disconnect(this, SLOT(CurrentChanged(int)));
//	bool isSignalsBlocked = blockSignals(true);
	QTabBar::setCurrentIndex(lastActive);
	connect(this, SIGNAL(currentChanged(int)), SLOT(CurrentChanged(int)));
//	blockSignals(isSignalsBlocked);
}
