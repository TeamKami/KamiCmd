#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QtWidgets>

class TabBar : public QTabBar
{
	Q_OBJECT

public:
	TabBar(QWidget *parent);

private:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	QList<int> tabsFocus;
	int mousePressCurrentIndex;
	int mousePressIndex;

protected:
	virtual void tabRemoved(int index);
	virtual void tabInserted(int index);

signals:
	void TabMousePressed(int index, QMouseEvent *event);
	void TabMouseDoubleClicked(int index, QMouseEvent *event);

private slots:
	void TabMoved(int from, int to);

public slots:
	void CurrentChanged(int index);
	void ActivateLastActive();
	void ActivateNext();
	void ActivatePrev();
};


class TabWidget : public QTabWidget
{
public:
	TabBar *tabBar_;

	TabWidget(QWidget *parent) : QTabWidget(parent)
	{
		tabBar_ = new TabBar(this);
		//tabBar_->setSelectionBehaviorOnRemove(QTabBar::SelectPreviousTab);
		setTabBar(tabBar_);
	};

};

#endif // TABWIDGET_H
