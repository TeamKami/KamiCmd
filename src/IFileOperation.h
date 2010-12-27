#pragma once
#include <QtCore>
#include <QString>

class IOperationsQueue;

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

	virtual OperationState getState() const = 0;
	virtual QString getType() const = 0; // i.e. copying, etc...
	virtual int getProgress() const = 0; // percentage

	virtual bool exec() = 0;
	virtual bool pause() = 0;
	virtual bool resume() = 0;
	virtual bool cancel() = 0;

//	friend class IOperationsQueue;
};

class IOperationsQueue 
{

public:
    virtual void add(IFileOperation&, IFileOperation::OperationState state) = 0;
	
	virtual IFileOperation *getFileOperation(int index) const = 0;
	virtual bool remove(int index) = 0;
	virtual bool pause(int index) = 0;
	virtual bool resume(int index) = 0;

	virtual void changePriority(const IFileOperation&, int) = 0;  // changes fileOperation place in waiting queue 
	virtual int size() const = 0;
};