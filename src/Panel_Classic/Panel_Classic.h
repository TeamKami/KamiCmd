#ifndef PANEL_CLASSIC_H
#define PANEL_CLASSIC_H

#include "library.h"
#include "../IPanel.h"
#include "FileListModel.h"
#include "SortModel.h"
#include "FileListView.h"

class Panel_Classic : public IPanel
{
	Q_OBJECT

public:
	Panel_Classic(QWidget *parent);
	virtual void Create(IUnit *createdFrom = NULL);
	virtual void Link(IUnit *withUnit = NULL);
	virtual QString GetText();
	virtual QIcon GetIcon();
	virtual void SaveState(QSettings &set);
	virtual void LoadState(QSettings &set);

	virtual QString GetPath();
	virtual const FileInfo * GetCurrentFile();
	virtual const FileInfo * SetCurrentFileToPrev();
	virtual const FileInfo * SetCurrentFileToNext();
	virtual QVector<const FileInfo*> GetSelectedFiles();
	virtual bool SetCurrentIndex(int index);
	virtual int  GetCurrentIndex();

private:
	QLineEdit *pathEdit;
	FileListView *view;
	QVBoxLayout *layout;
	IUnit *inactivePanel;

public slots:
	virtual void SetPath(QString path);

	void EnterSelected();
	void pathEditReturnPressed();

signals:
	void TextChanged();
	void FocusIn();
};

#endif // PANEL_CLASSIC_H
