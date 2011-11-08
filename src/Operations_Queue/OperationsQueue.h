#ifndef OPERATIONS_QUEUE
#define OPERATIONS_QUEUE

#include <QtCore/QList>
#include <QtCore/QThreadPool>


#include "library.h"
#include "IFileOperation.h"

typedef QPair< IFileOperation *, int> Operation;

class OperationsQueue : public QObject, public IOperationsQueue
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

private:
	bool IsFileOperationInQueue( const IFileOperation *fileOperation ) const;
	void RemoveFileOperation(const IFileOperation *fileOperation);

	QList < IFileOperation *> operations;
	QThreadPool threadPool;
};

#endif // OPERATIONS_QUEUE
