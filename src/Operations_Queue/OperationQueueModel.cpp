#include "OperationQueueModel.h"
#include "OperationsQueue.h"

OperationQueueModel::OperationQueueModel( OperationsQueue *queue, QObject *parent )
	: QAbstractListModel(parent), queue(queue)
{
	headers << tr("Operation Type") << tr("Progress") << tr("State");
	for(int i = 0; i < queue->GetCount(); i++)
		operations << queue->GetOperation(i);
	connect(queue, SIGNAL(operationAdded(IFileOperation *)), SLOT(AddOperation(IFileOperation *)));
	connect(queue, SIGNAL(operationRemoved(IFileOperation *)), SLOT(RemoveOperation(IFileOperation *)));
}

OperationQueueModel::~OperationQueueModel()
{

}

int OperationQueueModel::columnCount( const QModelIndex & ) const
{
	return headers.size();
}

int OperationQueueModel::rowCount( const QModelIndex & ) const
{
	return operations.size();
}

QVariant OperationQueueModel::data( const QModelIndex &index, int role ) const
{
	if(role == Qt::DisplayRole)
		if(index.row() < rowCount())
		{
			IFileOperation *op = operations[index.row()];
			switch(index.column())
			{
			case OperationType:
				return op->GetType();
			case Progress:
				return op->GetProgress();
			case State:
				return op->GetState();
			default:
				return QVariant();
			}
		}
	return QVariant();
}

QVariant OperationQueueModel::headerData( int section, Qt::Orientation /*orientation*/, int role /*= Qt::DisplayRole*/ ) const
{
	if(role == Qt::DisplayRole && section < headers.size())
		return headers.at(section);
	return QVariant();
}

bool OperationQueueModel::addRow( IFileOperation *operation)
{
	beginInsertRows(QModelIndex(), operations.size(), operations.size() + 1);
	operations.append(operation);
	endInsertRows();
	return true;
}

bool OperationQueueModel::removeRow( int position )
{
	beginRemoveRows(QModelIndex(), position, position + 1);
	operations.removeAt(position);
	endRemoveRows();
	return true;
}

void OperationQueueModel::AddOperation( IFileOperation *operation )
{
	addRow(operation);
}

void OperationQueueModel::RemoveOperation( IFileOperation *operation )
{
	int ind = -1;
	for(int i = 0; i < operations.size(); i++)
		if(operations.at(i) == operation)
		{
			ind = i;
			break;
		}
	if(ind != -1)
		removeRow(ind);
}

