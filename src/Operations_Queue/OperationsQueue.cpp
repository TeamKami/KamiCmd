#include "OperationsQueue.h"

void OperationsQueue::Add( IFileOperation& fileOperation, IFileOperation::OperationState state )
{
	if(state == IFileOperation::Running)
		if(!fileOperation.Exec())
		{
			g_Core->DebugWrite(g_moduleName, "Can't start file operation", ICoreFunctions::Info);
			return;
		}
	operationsQueue_.push_back(&fileOperation);
}


bool OperationsQueue::Remove( int index )
{
	IFileOperation &operation = *operationsQueue_[index];
	operationsQueue_.removeAt(index);
	operation.Cancel();
	return true;
}

bool OperationsQueue::Pause( int index )
{
	operationsQueue_[index]->Pause();
	return true;
}


bool OperationsQueue::Resume( int index )
{
	operationsQueue_[index]->Resume();
	return true;
}

void OperationsQueue::ChangePriority( const IFileOperation&, int )
{

}

int OperationsQueue::Count() const
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

IFileOperation * OperationsQueue::GetFileOperation( int index ) const
{
	return operationsQueue_[index];
}
