#ifndef OPERATIONQUEUEMODEL_H
#define OPERATIONQUEUEMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include "IFileOperation.h"

class OperationsQueue;

class OperationsQueueModelItem
{
public:
	OperationsQueueModelItem( const IFileOperation *fileOperation );
	QString GetType() const;
	QString GetStateString() const;
	int GetProgress() const;

	void SetProgress(int newProgress);
	void SetState(IFileOperation::OperationState newState);

	const IFileOperation *GetOperation() const;

private:
	QString type;
	QString operation;
	IFileOperation::OperationState state;
	int progress;

	const IFileOperation *fileOperation;
};

class OperationQueueModel : public QAbstractListModel 
{
	Q_OBJECT

public:
	enum Columns
	{
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
	void ChangeOperationState(IFileOperation *operation, IFileOperation::OperationState newState);
	void FinishOperation(IFileOperation *operation);
	void ChangeOperationProgress(IFileOperation *operation, int progress);
private:
	int FindOperationIndex(const IFileOperation *operation) const;

	OperationsQueue *queue;
	QList<OperationsQueueModelItem> operations;
	QStringList headers;
};

#endif // OPERATIONQUEUEMODEL_H
