#include "CopyThread.h"
#include "FileCopy.h"

CopyThread::CopyThread(QObject *parent, FileCopy *fileCopy)
	: QThread(parent), fileCopy(fileCopy)
{

}

CopyThread::~CopyThread()
{
//	delete fileCopy;
}

void CopyThread::run()
{
	fileCopy->Exec();
}
