#pragma once
#include <QtCore>
#include <QString>

#define CURRENT_FILE_OPERATION_INTERFACE_VERSION 1

class IFileOperation : public QObject
{
	Q_OBJECT	
public:
	enum OperationState
	{
		Paused,
		Running,
		ForcedRunning, 
		Queued,
		Error,
		Finished,
		Canceled
	};

	IFileOperation(QObject *parent = 0) : QObject(parent) {}
	virtual ~IFileOperation() {}
	virtual OperationState GetState() const = 0;
	virtual QString GetType() const = 0; // i.e. copying, etc...
	virtual int GetProgress() const = 0; // percentage

	virtual void ShowProgressDialog(QWidget *parent = 0) = 0;

	virtual bool Exec() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void Cancel() = 0;

	friend class FileOperationProxy;

signals:
	void finished();
	void finished(IFileOperation *); // workaround - allows operations queue to know which operation finished. need to change it to better solution
	void progressChanged(IFileOperation *, int);
};


class IOperationsQueue
{
public:
	virtual ~IOperationsQueue() {}
	virtual void Add(IFileOperation *, IFileOperation::OperationState state) = 0;
	virtual void Pause(IFileOperation *) = 0;
	virtual void Cancel(IFileOperation *) = 0;

	virtual int GetCount() const = 0;

	virtual bool IsValid(IFileOperation *) const = 0;

	virtual void Remove(IFileOperation* ) = 0;
	virtual void Resume(IFileOperation *) = 0;
//	virtual void ChangePriority(const IFileOperation&, int) = 0;  // changes fileOperation place in waiting queue 
};

namespace Internal
{
	class FileOperationProxy
	{
	public:
		FileOperationProxy(IFileOperation *fileOperation)
			: operation(fileOperation)
		{}
		bool Exec() { return operation->Exec(); }
		void Pause() { operation->Pause(); }
		void Resume() { operation->Resume();	}
		void Cancel() { operation->Cancel(); }

		IFileOperation *operation;
	};

}
