#ifndef FILE_COPY_H
#define FILE_COPY_H

#include <QList>
#include <QtGui/QDialog>

#include "../IFileCopy.h"
#include "library.h"

const static char* g_moduleName = "FileCopy Windows";
class FileCopy : public QObject, public IFileCopy
{
	Q_OBJECT

public:
	FileCopy(QObject *parent = 0);
	~FileCopy();

	bool copy(const QString & from, const QString & to);
	bool copy(const QStringList & files, const QString & to, const QString & includeMask, const QString & excludeMask);

	bool exec();
	bool pause();
	bool resume();
	bool cancel();
	OperationState getState() const;
	QString getType() const;
	int getProgress() const; 
private:
	OperationState state_;
	QStringList from_;
	QString to_;
	QString includeMask_;
	QString excludeMask_;

	quint64 bytesToCopy;
};

#endif // OPERATIONS_QUEUE
