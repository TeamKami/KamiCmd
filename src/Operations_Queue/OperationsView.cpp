#include "OperationsView.h"

#include <QtGui/QPainter>
#include <QtWidgets/QHeaderView>

OperationsView::OperationsView(QWidget *parent)
	: QTreeView(parent)
{
	
}

OperationsView::~OperationsView()
{

}

void OperationsView::paintEvent( QPaintEvent* event )
{
	QTreeView::paintEvent(event);
	QPainter painter(viewport());
	for (int i = 0; i < model()->columnCount(); ++i)
	{
//		 draw only visible sections starting from second column
 		if (header()->isSectionHidden(i) || header()->visualIndex(i) <= 0)
 			continue;
		// position mapped to viewport

		int pos = header()->sectionViewportPosition(i) - 1;
		if (pos > 0)
		{
			painter.setPen(QStyle::SH_Table_GridLineColor);
			painter.drawLine(QPoint(pos, 0), QPoint(pos, height()));
		}
	}
}

void OperationsView::scrollContentsBy( int dx, int dy )
{
	QTreeView::scrollContentsBy(dx, dy);
	// make sure lines get updated even if the view is empty
	viewport()->update();
}
