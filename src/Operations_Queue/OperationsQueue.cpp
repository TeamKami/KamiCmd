#include "OperationsQueue.h"
#include "FileOperationTask.h"

OperationsQueue::OperationsQueue( QObject *parent /*= 0*/ )
	: QObject(parent)
{
	threadPool.setMaxThreadCount(8);

}

OperationsQueue::~OperationsQueue()
{

}

void OperationsQueue::Add( IFileOperation *fileOperation, IFileOperation::OperationState state )
{
	operations.append(qMakePair(fileOperation, (int) state));
	FileOperationTask *task = new FileOperationTask(fileOperation);
	task->setAutoDelete(false);
	threadPool.start(task);
}

void OperationsQueue::Pause( IFileOperation * fileOperation)
{
	if(Operation *operation = findFileOperation(fileOperation))
		Internal::FileOperationProxy(operation->first).Pause();
}

void OperationsQueue::Cancel( IFileOperation * fileOperation )
{
	if(Operation *operation = findFileOperation(fileOperation))
		Internal::FileOperationProxy(operation->first).Cancel();
}

void OperationsQueue::Remove( IFileOperation * fileOperation )
{
	if(Operation *operation = findFileOperation(fileOperation))
		Internal::FileOperationProxy(operation->first).Cancel();
	// TODO implemet operation removal

}
void OperationsQueue::Resume( IFileOperation * fileOperation )
{
	if(Operation *operation = findFileOperation(fileOperation))
		Internal::FileOperationProxy(operation->first).Resume();
}

void OperationsQueue::AddPriority( const IFileOperation * fileOperation, int steps)
{
	int operationIndex = -1;
	for(int i = 0; i < operations.size(); i++)
		if(operations[i].first == fileOperation)
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

Operation* OperationsQueue::findFileOperation( const IFileOperation *fileOperation )
{
	for(int i = 0; i < operations.size(); i++)
		if(operations.at(i).first == fileOperation)
			return &operations[i];
	return NULL;
}
