#ifndef OPERATIONQUEUEMODEL_H
#define OPERATIONQUEUEMODEL_H

#include <QAbstractListModel>
#include <QStringList>

class OperationsQueue;
class IFileOperation;

class OperationQueueModel : public QAbstractListModel 
{
	Q_OBJECT

public:
	enum Columns {
		OperationType,
		Progress,
		State
	};

	OperationQueueModel(OperationsQueue *queue, QObject *parent);
	~OperationQueueModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	int columnCount( const QModelIndex &parent = QModelIndex() ) const;

	bool addRow(IFileOperation *operation);
	bool removeRow(int position);

public slots:
	void AddOperation(IFileOperation *operation);
	void RemoveOperation(IFileOperation *operation);

private:

	OperationsQueue *queue;
	QList<IFileOperation *> operations;
	QStringList headers;
};

class OperationsQueueModelItem
{
	OperationsQueueModelItem();
	~OperationsQueueModelItem();
};

#endif // OPERATIONQUEUEMODEL_H
