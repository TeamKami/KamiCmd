#ifndef LEXERSDELEGATE_H
#define LEXERSDELEGATE_H

#include <QStyledItemDelegate>

class LexersDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	LexersDelegate(QObject * parent = 0);
	virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif //LEXERSDELEGATE_H