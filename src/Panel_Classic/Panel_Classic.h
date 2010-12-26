#ifndef PANEL_CLASSIC_H
#define PANEL_CLASSIC_H

#include "library.h"
#include "../IPanel.h"
#include "FileListModel.h"
#include "SortModel.h"
#include "FileListView.h"

#ifdef PANEL_CLASSIC_LIB
# define PANEL_CLASSIC_EXPORT Q_DECL_EXPORT
#else
# define PANEL_CLASSIC_EXPORT Q_DECL_IMPORT
#endif

class Panel_Classic : public IPanel
{
	Q_OBJECT

public:
	Panel_Classic(QWidget *parent);
	void Create(IUnit *createdFrom); //overload
	void Link(IUnit *withUnit); //overload
	QString GetText(); // overload
	void SaveState(QSettings &set); // overload
	void LoadState(QSettings &set); // overload

	QString GetPath(); //overload
	const FileInfo *const GetCurrentFile(); //overload
	QVector<const FileInfo*> GetSelectedFiles(); //overload
	bool SetCurrentIndex(int index); //overload
	int  GetCurrentIndex(); //overload

private:
	QLineEdit *pathEdit;
	FileListView *view;
	QVBoxLayout *layout;
	IUnit *inactivePanel;

public slots:
	void SetPath(QString path); // overload
	void EnterSelected();

	void pathEditReturnPressed();

signals:
	void TextChanged();
	void FocusIn();
};

#endif // PANEL_CLASSIC_H
