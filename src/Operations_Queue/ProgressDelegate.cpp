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

void ProgressDelegate::SetProgress( int progress )
{
	this->progress = progress;
}

void ProgressDelegate::paint( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
		QStyledItemDelegate::paint(painter, option, index);
}
