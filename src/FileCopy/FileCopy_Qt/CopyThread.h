#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QtCore/QThread>

class FileCopy;

class CopyThread : public QThread
{
public:
	CopyThread(QObject *parent, FileCopy *fileCopy);
	~CopyThread();
	void run();
private:
	FileCopy *fileCopy;
};

#endif // COPYTHREAD_H
