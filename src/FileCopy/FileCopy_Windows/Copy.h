#ifndef FILE_COPY_H
#define FILE_COPY_H

#include <QList>
#include <QtGui/QDialog>

#include "IFileCopy.h"
#include "library.h"

const static char* g_moduleName = "FileCopy_Windows";
class FileCopy : public QObject, public IFileCopy
{
	Q_OBJECT

public:
	FileCopy(QObject *parent = 0);
	~FileCopy();

	virtual bool prepareForCopy(const QString & from, const QString & to);
	virtual bool prepareForCopy(const QStringList & files, const QString & to, const QString & includeMask, const QString & excludeMask);
	virtual bool prepareForCopy(const QList<FileInfo *> & files, const QString & to, const QString & includeMask, const QString & excludeMask);

	virtual bool Exec();
	virtual bool Pause();
	virtual bool Resume();
	virtual bool Cancel();
	virtual OperationState GetState() const;
	virtual QString GetType() const;
	virtual int GetProgress() const; 
private:
	OperationState state_;
	QStringList from_;
	QString to_;
	QString includeMask_;
	QString excludeMask_;

	quint64 bytesToCopy;
};

#endif // OPERATIONS_QUEUE
