#ifndef FILEOPERATIONTASK_H
#define FILEOPERATIONTASK_H

#include <QtCore/QRunnable>
#include "IFileOperation.h"

class FileOperationTask : public QRunnable
{
public:
	FileOperationTask(IFileOperation *fileOperation);
	~FileOperationTask();
	
	void run();

private:
	Internal::FileOperationProxy operation;
};

#endif // FILEOPERATIONTASK_H
