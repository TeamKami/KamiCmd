#ifndef FILELISTVIEW_H
#define FILELISTVIEW_H

#include <QtGui>
#include "SortModel.h"
#include "FileListModel.h"

class FileListView : public QTreeView
{
	Q_OBJECT

public:
	FileListView();
	virtual void focusInEvent(QFocusEvent *event);
	virtual void focusOutEvent(QFocusEvent *event);
	virtual void currentChanged(const QModelIndex &current, const QModelIndex &previous);
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	
	FileListModel *Model();
	void SetModel(FileListModel *model);
	SortModel *Sort();
	void SetSortModel(SortModel *sort);

private:
	SortModel *sort_;
	FileListModel *model_;
	int currentSelectionAction;
	QModelIndex mouseMovePrevIndex, mouseMoveActionPrevIndex;

signals:
	void EnterSelected();
	void FocusIn();

public slots:
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	void keyboardSearchNullify();
	void SelectAll(int selectAction, bool excludeCurrent = false);
};

#endif // FILELISTVIEW_H