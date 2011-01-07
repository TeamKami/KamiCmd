#ifndef OPERATIONS_QUEUE
#define OPERATIONS_QUEUE

#include <QList>
#include <QtGui/QDialog>

#include "../IFileOperation.h"
#include "library.h"

const static char* g_moduleName = "Operations Queue";
class OperationsQueue : public QObject, public IOperationsQueue
{
	Q_OBJECT

public:
	OperationsQueue(QObject *parent = 0) : QObject(parent)
	{}
	~OperationsQueue();

	virtual void Add(IFileOperation&, IFileOperation::OperationState state);
	virtual IFileOperation* GetFileOperation(int index) const;
	virtual bool Remove(int index);
	virtual bool Pause(int index);
	virtual bool Resume(int index);

	virtual void ChangePriority(const IFileOperation&, int);  // changes fileOperation place in waiting queue 
	virtual int Count() const;

private:
	QList<IFileOperation *> operationsQueue_;
	int size_;
};

#endif // OPERATIONS_QUEUE
