#ifndef OPERATIONS_QUEUE
#define OPERATIONS_QUEUE

#include <QtCore/QList>
#include <QtCore/QThreadPool>
#include <QtCore/QSignalMapper>

#include "library.h"
#include "IFileOperation.h"

class FileOperationThread;

class OperationsQueue : public IOperationsQueue
{
	Q_OBJECT

public:
	OperationsQueue(QObject *parent = 0);
	~OperationsQueue();

	virtual void Add(IFileOperation *fileOperation, IFileOperation::OperationState state);
	virtual void Pause(IFileOperation *fileOperation);
	virtual void Cancel(IFileOperation *fileOperation);
	virtual int GetCount() const;	
	virtual bool IsValid( IFileOperation *operation ) const;
	IFileOperation *GetOperation(int index);

protected:
	virtual void Remove(IFileOperation *fileOperation);
	virtual void Resume(IFileOperation *fileOperation);
	void AddPriority(const IFileOperation *, int); 

signals:
	void operationRemoved(IFileOperation *);
	void operationAdded(IFileOperation *);
	void operationStateChanged(IFileOperation *operation, IFileOperation::OperationState newState);
	void operationFinished(IFileOperation *);
	void operationProgressChanged(IFileOperation *, int);

private:
	bool IsFileOperationInQueue( const IFileOperation *fileOperation ) const;
	void RemoveFileOperation(const IFileOperation *fileOperation);

	QList < IFileOperation *> operations;
	QList <FileOperationThread *> operationTasks;
	QThreadPool threadPool;
	QSignalMapper mapper;
};

#endif // OPERATIONS_QUEUE
