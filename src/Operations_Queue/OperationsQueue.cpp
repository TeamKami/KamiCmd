#include "OperationsQueue.h"

void OperationsQueue::add( IFileOperation& fileOperation, IFileOperation::OperationState state )
{
	if(state == IFileOperation::Running)
		if(!fileOperation.exec())
		{
			g_Core->DebugWrite(g_moduleName, "Can't start file operation", ICoreFunctions::Info);
			return;
		}
	operationsQueue_.push_back(&fileOperation);
}


bool OperationsQueue::remove( int index )
{
	IFileOperation &operation = *operationsQueue_[index];
	operationsQueue_.removeAt(index);
	return operation.cancel();
}

bool OperationsQueue::pause( int index )
{
	return operationsQueue_[index]->pause();
}


bool OperationsQueue::resume( int index )
{
	return operationsQueue_[index]->resume();
}

void OperationsQueue::changePriority( const IFileOperation&, int )
{

}

int OperationsQueue::size() const
{
	return operationsQueue_.size();
}

OperationsQueue::~OperationsQueue()
{
	for(int i = 0; i < operationsQueue_.size(); i++)
	{
//		if(!operationsQueue_[i]->cancel())
	//		g_Core->DebugWrite(g_moduleName, "Error when cancelling file operation!", ICoreFunctions::Info);
	}
}

IFileOperation * OperationsQueue::getFileOperation( int index ) const
{
	return operationsQueue_[index];
}
