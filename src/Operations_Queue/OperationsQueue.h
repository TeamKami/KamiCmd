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

protected:
	virtual void Remove(IFileOperation *fileOperation);
	virtual void Resume(IFileOperation *fileOperation);
	void AddPriority(const IFileOperation *, int); 

private:
	Operation *findFileOperation(const IFileOperation *fileOperation);

	QList < Operation > operations;
	QThreadPool threadPool;
};

#endif // OPERATIONS_QUEUE
