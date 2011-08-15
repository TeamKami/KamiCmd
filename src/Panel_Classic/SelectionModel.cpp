#include "SelectionModel.h"

SelectionModel::SelectionModel(QAbstractItemModel *model, QObject *parent)
	: QItemSelectionModel(model, parent)
{

}

SelectionModel::~SelectionModel()
{

}

void SelectionModel::select(const QModelIndex &index, QItemSelectionModel::SelectionFlags command)
{
	QItemSelection toSelect;
	toSelect << QItemSelectionRange(index.sibling(index.row(), 0), index.sibling(index.row(), 1));
	QItemSelectionModel::select(toSelect, command);

	//QItemSelectionModel::select(index, command);
	//QItemSelectionModel::select(index.sibling(index.row(), 0), command);
	//QItemSelectionModel::select(index.sibling(index.row(), 1), command);
}
