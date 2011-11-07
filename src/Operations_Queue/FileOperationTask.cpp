#include "FileOperationTask.h"
#include "library.h"

#include <QEventLoop>

FileOperationTask::FileOperationTask( IFileOperation *fileOperation )
	: operation(fileOperation)
{
}

FileOperationTask::~FileOperationTask()
{
	g_Core->DebugWrite("OperationQueue", "File Operation task is complete. Deleting...");
//	delete operation.operation;
}

void FileOperationTask::run()
{	
	operation.Exec();
	QEventLoop loop;
	QObject::connect(operation.operation, SIGNAL(finished()), &loop, SLOT(quit()), Qt::QueuedConnection);
	loop.exec();
}

