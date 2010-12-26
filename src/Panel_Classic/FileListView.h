#ifndef FILELISTVIEW_H
#define FILELISTVIEW_H

#include <QtGui>
#include "SortModel.h"
#include "FileListModel.h"

class FileListView : public QTreeView
{
	Q_OBJECT

	int currentSelectionAction;
	QModelIndex mouseMovePrevIndex, mouseMoveActionPrevIndex;

public:
	SortModel *sort;
	FileListModel *model;

	FileListView();

//	void drawRow(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const; //overload
	void focusInEvent(QFocusEvent *event); // overload
	void focusOutEvent(QFocusEvent *event); // overload
	void currentChanged(const QModelIndex &current, const QModelIndex &previous); // overload
	void mousePressEvent(QMouseEvent *event); // overload
	void mouseMoveEvent(QMouseEvent *event); // overload
	void mouseReleaseEvent(QMouseEvent *event); // overload
	void wheelEvent(QWheelEvent *event); // overload
	void resizeEvent(QResizeEvent *event); // overload

signals:
	void EnterSelected();
	void FocusIn();

public slots:
	void keyPressEvent(QKeyEvent *event); // overload
	void keyReleaseEvent(QKeyEvent *event); // overload
	void mouseDoubleClickEvent(QMouseEvent *event); // overload
	void keyboardSearchNullify();
	void SelectAll(int selectAction, bool excludeCurrent = false);
};

#endif // FILELISTVIEW_H