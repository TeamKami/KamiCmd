#include "FileListHeader.h"

#define VIEWPORT_MARGIN_TOP 4/*6*/

FileListHeader::FileListHeader( Qt::Orientation orientation, QWidget *parent /*= NULL*/ )
	: QHeaderView(orientation, parent)
{
	setMovable(true);
	setStretchLastSection(true);
	setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	setStyleSheet(QString("border-top: 0px solid #5d6c78; border-bottom: %1px solid white;").arg(VIEWPORT_MARGIN_TOP));
	connect(this, SIGNAL(sectionDoubleClicked(int)), SLOT(sectionDoubleClickedSlot(int)));
}

FileListHeader::~FileListHeader()
{

}

QSize FileListHeader::sizeHint() const
{
	QSize size = QHeaderView::sizeHint();
	size.setHeight(size.height() + VIEWPORT_MARGIN_TOP);
	return size;
}

void FileListHeader::sectionDoubleClickedSlot(int logicalIndex)
{
	setSortIndicator(logicalIndex, sortIndicatorOrder() == Qt::AscendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
}
