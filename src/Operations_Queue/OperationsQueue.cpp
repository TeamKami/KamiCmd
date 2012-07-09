#include "OperationsQueue.h"
#include "FileOperationTask.h"

OperationsQueue::OperationsQueue( QObject *parent /*= 0*/ )
	: QObject(parent)
{
	threadPool.setMaxThreadCount(QThread::idealThreadCount());
}

OperationsQueue::~OperationsQueue()
{
 	foreach(IFileOperation *op, operations)
 		Internal::FileOperationProxy(op).Cancel();
	threadPool.waitForDone();
}

void OperationsQueue::Add( IFileOperation *fileOperation, IFileOperation::OperationState /*state*/ )
{
	operations.append(fileOperation);
	connect(dynamic_cast<QObject *>(fileOperation), SIGNAL(finished(IFileOperation *)), SIGNAL(operationFinished(IFileOperation *)));
	connect(dynamic_cast<QObject *>(fileOperation), SIGNAL(progressChanged(IFileOperation *, int)), SIGNAL(operationProgressChanged(IFileOperation *, int)));

	FileOperationTask *task = new FileOperationTask(fileOperation);
	task->setAutoDelete(true);
	threadPool.start(task);
	
	emit operationAdded(fileOperation);
}

void OperationsQueue::Pause( IFileOperation * fileOperation)
{
	if(IsFileOperationInQueue(fileOperation))
	{
		Internal::FileOperationProxy(fileOperation).Pause();
		emit operationStateChanged(fileOperation, IFileOperation::Paused);
	}
}

void OperationsQueue::Cancel( IFileOperation * fileOperation )
{
	if(IsFileOperationInQueue(fileOperation))
	{
		Internal::FileOperationProxy(fileOperation).Cancel();
		emit operationStateChanged(fileOperation, IFileOperation::Canceled);
	}
}

void OperationsQueue::Remove( IFileOperation * fileOperation )
{
	if(IsFileOperationInQueue(fileOperation))
	{
		Internal::FileOperationProxy(fileOperation).Cancel();
		RemoveFileOperation(fileOperation);
		emit operationRemoved(fileOperation);
	}
}
void OperationsQueue::Resume( IFileOperation * fileOperation )
{
	if(IsFileOperationInQueue(fileOperation))
	{
		Internal::FileOperationProxy(fileOperation).Resume();
		emit operationStateChanged(fileOperation, IFileOperation::Running);
	}
}

void OperationsQueue::AddPriority( const IFileOperation * fileOperation, int steps)
{
	int operationIndex = -1;
	for(int i = 0; i < operations.size(); i++)
		if(operations[i] == fileOperation)
		{
			operationIndex = i;
			break;
		}

	if(operationIndex != -1)
	{
		steps = (steps >= 0) ? qMin(operations.size() - operationIndex, steps) : qMin(operationIndex, -steps);
		qSwap(operations[operationIndex], operations[operationIndex + steps]);
	}
}

int OperationsQueue::GetCount() const
{
	return operations.size();
}

bool OperationsQueue::IsFileOperationInQueue( const IFileOperation *fileOperation ) const
{
	return qFind(operations, fileOperation) != operations.end();
}

bool OperationsQueue::IsValid( IFileOperation *operation ) const
{
	return IsFileOperationInQueue(operation);
}

void OperationsQueue::RemoveFileOperation( const IFileOperation *fileOperation )
{
	int ind = -1;
	for(int i = 0; i < operations.size(); i++)
		if(operations.at(i) == fileOperation)
		{
			ind = i;
			break;
		}

	if(ind != -1)
		operations.removeAt(ind);
}

IFileOperation * OperationsQueue::GetOperation( int index )
{
	return operations[index];
}
