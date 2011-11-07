#ifndef FILEOPERATIONDUMMY
#define FILEOPERATIONDUMMY

#include "IFileOperation.h"
#include <QtCore/QTimer>

class DummyProgressDialog;

class FileOperationDummy : public IFileOperation
{
	Q_OBJECT
public:
	FileOperationDummy(QObject *parent = 0);
	virtual ~FileOperationDummy();

	virtual int GetProgress() const;

	virtual bool Exec();

	virtual void Resume();

	virtual void Pause();

	virtual void Cancel();

	virtual OperationState GetState() const;

	virtual QString GetType() const;

	virtual void ShowProgressDialog(QWidget *parent = 0);

private slots:
	void update();

signals:
	void cancelOperation();

private:
	enum {Interval = 100};

	QTimer *timer;

	DummyProgressDialog *progressDialog;
	
	int duration;
	int elapsed;
	OperationState state;
	QMutex m;
};

#endif