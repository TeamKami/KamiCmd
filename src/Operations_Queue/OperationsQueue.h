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

	void add(IFileOperation&, IFileOperation::OperationState state);
	IFileOperation *getFileOperation(int index) const;
	bool remove(int index);
	bool pause(int index);
	bool resume(int index);

	void changePriority(const IFileOperation&, int);  // changes fileOperation place in waiting queue 

	int size() const;

private:
	QList<IFileOperation *> operationsQueue_;
	int size_;
};

#endif // OPERATIONS_QUEUE
