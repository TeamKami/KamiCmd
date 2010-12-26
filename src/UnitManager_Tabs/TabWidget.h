#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtGui>

class TabBar : public QTabBar
{
	Q_OBJECT

	void mousePressEvent(QMouseEvent *event); // overload
	void mouseDoubleClickEvent(QMouseEvent *event); // overload

public:
	TabBar(QWidget *parent): QTabBar(parent) {};

signals:
	void TabMousePressed(int index, QMouseEvent *event);
	void TabMouseDoubleClicked(int index, QMouseEvent *event);
};


class TabWidget : public QTabWidget
{
public:
	TabBar *tabBar_;

	TabWidget(QWidget *parent) : QTabWidget(parent)
	{
		tabBar_ = new TabBar(this);
		setTabBar(tabBar_);
	};

};

#endif // TABWIDGET_H
