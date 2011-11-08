#ifndef PROGRESSDELEGATE_H
#define PROGRESSDELEGATE_H

#include <QStyledItemDelegate>

class ProgressDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	ProgressDelegate(QObject *parent = 0);
	~ProgressDelegate();

	virtual void paint ( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const;
};

#endif // PROGRESSDELEGATE_H
