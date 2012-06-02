#ifndef PANEL_CLASSIC_INTERFACE_H
#define PANEL_CLASSIC_INTERFACE_H

#include <QtPlugin>
#include <QtCore/qglobal.h>
#include <QWidget>
#include "IUnitManager.h"
#include "IFileSystem.h"

class IPanel : public IUnit
{
public:
	IPanel(QWidget *parent) : IUnit(parent) {};
	virtual QString GetPath() = 0;
        virtual const FileInfo * GetCurrentFile() = 0;
        virtual const FileInfo * SetCurrentFileToPrev() = 0;
        virtual const FileInfo * SetCurrentFileToNext() = 0;
	virtual QVector<FileInfo> GetSelectedFiles() = 0;
	virtual bool SetCurrentIndex(int index) = 0;
	virtual int  GetCurrentIndex() = 0;

public slots:
	virtual void SetPath(QString path) = 0;
};

#endif // PANEL_CLASSIC_INTERFACE_H
