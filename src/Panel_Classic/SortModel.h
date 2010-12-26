#ifndef PANEL_SORTMODEL_H
#define PANEL_SORTMODEL_H

#include <QSortFilterProxyModel>


class SortModel : public QSortFilterProxyModel
{
	Q_OBJECT

private:
	int column_;
	Qt::SortOrder order_;

public:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);
	bool lessThan(const QModelIndex &upIndex, const QModelIndex &downIndex) const;
};

#endif // PANEL_SORTMODEL_H
