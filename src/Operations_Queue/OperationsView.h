#ifndef OPERATIONSVIEW_H
#define OPERATIONSVIEW_H

#include <QTreeView>

class OperationsView : public QTreeView
{
	Q_OBJECT

public:
	OperationsView(QWidget *parent);
	~OperationsView();

protected:
	virtual void paintEvent(QPaintEvent* event);
	virtual void scrollContentsBy(int dx, int dy);

private:
	
};

#endif // OPERATIONSVIEW_H
