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

	if (event->button() == Qt::LeftButton && event->modifiers() & Qt::ControlModifier)
		return;
	QTabBar::mousePressEvent(event);
}

void TabBar::mouseDoubleClickEvent( QMouseEvent *event )
{
	for (int i = 0; i < count(); i++)
		if (tabRect(i).contains(event->pos()))
		{
			emit TabMouseDoubleClicked(i, event);
			return;
		}
	emit TabMouseDoubleClicked(-1, event);

	QTabBar::mouseDoubleClickEvent(event);
}