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
	QItemSelectionModel::select(index.sibling(index.row(), 0), command);
	QItemSelectionModel::select(index.sibling(index.row(), 1), command);
}
