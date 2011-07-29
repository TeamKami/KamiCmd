#ifndef FILESDELEGATE_H
#define FILESDELEGATE_H

#include <QStyledItemDelegate>

class FilesDelegate : public QStyledItemDelegate
{
	Q_OBJECT

public:
	FilesDelegate(QObject *parent = 0);
	virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void QCommonStylePrivate_viewItemDrawText(QStyle *s, QPainter *p, const QStyleOptionViewItemV4 *option, const QRect &rect, bool isSplitExtension = false) const;

private:
	QString quickSearch;

public slots:
	void PaletteChanged();
	void QuickSearchChanged(QString search);
};

#endif // FILESDELEGATE_H
