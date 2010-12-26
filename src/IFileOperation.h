#pragma once
#include <QtCore>


class IFileOperation
{
public:

    enum OperationState
    {
        Paused,
        Running,
        ForcedRunning, 
        Waiting,
        Error,
    };

	virtual bool exec() = 0;
	virtual bool pause() = 0;
	virtual bool resume() = 0;
	virtual bool cancel() = 0;
	virtual int getStatus() const = 0;
};

class IOperationsQueue 
{
public:
    virtual void add(IFileOperation&, IFileOperation::OperationState state) = 0;
	virtual bool remove(const IFileOperation& ) = 0;
	virtual bool pause(const IFileOperation&) = 0;
	virtual bool resume(const IFileOperation&) = 0;	

	virtual bool remove(int index) = 0;
	virtual bool pause(int index) = 0;
	virtual bool resume(int index) = 0;

	virtual void changePriority(const IFileOperation&, int) = 0;  // changes fileOperation place in waiting queue 
	virtual void incPriority(const IFileOperation&) = 0;
	virtual void incPriority(int index) = 0;

	virtual void decPriority(const IFileOperation&) = 0;
	virtual void decPriority(int index) = 0;
	virtual int size() const = 0;
};