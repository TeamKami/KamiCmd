#ifndef SELECTIONMODEL_H
#define SELECTIONMODEL_H

#include <QItemSelectionModel>

class SelectionModel : public QItemSelectionModel
{
	Q_OBJECT

public:
	SelectionModel(QAbstractItemModel *model, QObject *parent);
	~SelectionModel();

public slots:
	virtual void select(const QModelIndex &index, QItemSelectionModel::SelectionFlags command);
};

#endif // SELECTIONMODEL_H
