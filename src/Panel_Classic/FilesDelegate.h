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
	int currentRow;

public slots:
	void PaletteChanged();
	void QuickSearchChanged(QString search);
	void CurrentRowChanged(int newCurrentRow);
};

#endif // FILESDELEGATE_H
