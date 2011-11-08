#include "ProgressDelegate.h"
#include "OperationQueueModel.h"
#include <QStyleOptionProgressBarV2>
#include <QPainter>
#include <QModelIndex>
#include <QApplication>

ProgressDelegate::ProgressDelegate(QObject *parent)
	: QStyledItemDelegate(parent)
{

}

ProgressDelegate::~ProgressDelegate()
{

}

void ProgressDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	if(index.column() != OperationQueueModel::Progress)
	{
		QStyledItemDelegate::paint(painter, option, index);
		return;
	}

	QStyleOptionProgressBar opt;
	opt.rect = option.rect;
	opt.minimum = 0;
	opt.maximum = 100;
	opt.progress = index.data().toInt();
	opt.text = QString("%1%").arg(opt.progress);
	opt.textVisible = true;

	QApplication::style()->drawControl(QStyle::CE_ProgressBar, &opt, painter, 0);
}
