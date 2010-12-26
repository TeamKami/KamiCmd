#ifndef ARCHIVER_INTERFACE_H
#define ARCHIVER_INTERFACE_H

#include "IFileSystem.h"

class IArchiver : public IFileSystem
{
public:
	virtual bool OpenFile(QFile *file) = 0;
};

#endif // ARCHIVER_INTERFACE_H
