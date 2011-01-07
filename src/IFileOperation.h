#pragma once
#include <QtCore>
#include <QString>

class IOperationsQueue;

#define CURRENT_FILE_OPERATION_INTERFACE_VERSION 1

class IFileOperation
{	
public:
    enum OperationState
    {
        Paused,
        Running,
        ForcedRunning, 
        Queued,
        Error,
		Finished
    };

	virtual OperationState GetState() const = 0;
	virtual QString GetType() const = 0; // i.e. copying, etc...
	virtual int GetProgress() const = 0; // percentage

	virtual bool Exec() = 0;
	virtual bool Pause() = 0;
	virtual bool Resume() = 0;
	virtual bool Cancel() = 0;

	virtual ~IFileOperation() 
	{

	}
//	friend class IOperationsQueue;
};

class IOperationsQueue 
{

public:
    virtual void Add(IFileOperation&, IFileOperation::OperationState state) = 0;
	
	virtual IFileOperation *GetFileOperation(int index) const = 0;
	virtual bool Remove(int index) = 0;
	virtual bool Pause(int index) = 0;
	virtual bool Resume(int index) = 0;

	virtual void ChangePriority(const IFileOperation&, int) = 0;  // changes fileOperation place in waiting queue 
	virtual int Count() const = 0;
	virtual ~IOperationsQueue() 
	{};
};