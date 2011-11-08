#include "OperationQueueModel.h"
#include "OperationsQueue.h"

OperationQueueModel::OperationQueueModel( OperationsQueue *queue, QObject *parent )
	: QAbstractListModel(parent), queue(queue)
{
	headers << tr("Operation Type") << tr("Progress") << tr("State");

	for(int i = 0; i < queue->GetCount(); i++)
		AddOperation(queue->GetOperation(i));

	connect(queue, SIGNAL(operationAdded(IFileOperation *)), SLOT(AddOperation(IFileOperation *)));
	connect(queue, SIGNAL(operationRemoved(IFileOperation *)), SLOT(RemoveOperation(IFileOperation *)));
	connect(queue, SIGNAL(operationStateChanged(IFileOperation *, IFileOperation::OperationState )), SLOT(ChangeOperationState(IFileOperation *, IFileOperation::OperationState)));
	connect(queue, SIGNAL(operationFinished(IFileOperation *)), SLOT(FinishOperation(IFileOperation *)));
	connect(queue, SIGNAL(operationProgressChanged(IFileOperation *, int)), SLOT(ChangeOperationProgress(IFileOperation *, int)));
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
			const OperationsQueueModelItem & item = operations.at(index.row());
			switch(index.column())
			{
			case OperationType:
				return item.GetType();
			case Progress:
				return item.GetProgress();
			case State:
				return item.GetStateString();
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
	operations.append(OperationsQueueModelItem(operation));
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
	int index = FindOperationIndex(operation);
	if(index != -1)
		removeRow(index);
}

void OperationQueueModel::ChangeOperationState( IFileOperation *operation, IFileOperation::OperationState newState )
{
	int i = FindOperationIndex(operation);
	if(i != -1)
	{
		operations[i].SetState(newState);
		QModelIndex ind = index(i, State);
		emit dataChanged(ind, ind);
	}
}

int OperationQueueModel::FindOperationIndex( const IFileOperation *operation ) const
{
	int index = -1;
	for(int i = 0; i < operations.size(); i++)
		if(operations.at(i).GetOperation() == operation)
		{
			index = i;
			break;
		}
	return index;
}

void OperationQueueModel::FinishOperation( IFileOperation *operation )
{
	int i = FindOperationIndex(operation);
	if(i != -1)
	{
		operations[i].SetState(IFileOperation::Finished);
		operations[i].SetProgress(100);
		QModelIndex left = index(i, Progress);
		QModelIndex right = index(i, State);
		emit dataChanged(left, right);
	}
}

void OperationQueueModel::ChangeOperationProgress( IFileOperation *operation, int progress )
{
	int i = FindOperationIndex(operation);
	if(i != -1)
	{
		operations[i].SetProgress(progress);
		QModelIndex left = index(i, Progress);
		emit dataChanged(left, left);
	}
}


OperationsQueueModelItem::OperationsQueueModelItem( const IFileOperation *fileOperation )
	: fileOperation(fileOperation)
{
	type = fileOperation->GetType();
	progress = fileOperation->GetProgress();
	state = fileOperation->GetState();
}

QString OperationsQueueModelItem::GetType() const
{
	return type;
}

QString OperationsQueueModelItem::GetStateString() const
{
	const static char *states[] = {"Paused", "Running", "Forced Running", "Queued", "Error", "Finished", "Canceled"};
	if(state > 6 || state < 0) // fucking workaround. need to change it
		return "FUUUUU";
	return states[state];
}

void OperationsQueueModelItem::SetProgress(int newProgress)
{
	progress = newProgress;
}

void OperationsQueueModelItem::SetState( IFileOperation::OperationState newState )
{
	state = newState;
}

const IFileOperation * OperationsQueueModelItem::GetOperation() const
{
	return fileOperation;
}

int OperationsQueueModelItem::GetProgress() const
{
	return progress;
}
