#ifndef FILELISTVIEW_H
#define FILELISTVIEW_H

#include <QtGui>
#include "SortModel.h"
#include "FileListModel.h"
#include <QLineEdit>

class SearchLineEdit : public QLineEdit
{
public:
	virtual void keyPressEvent(QKeyEvent *event) { QLineEdit::keyPressEvent(event); };
	SearchLineEdit(QWidget *parent) : QLineEdit(parent) { };
};

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
	virtual void keyboardSearch(const QString &search);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void changeEvent(QEvent *event);
	
	QWidget *QuickSearchBar() { return searchEdit; };

private:
	QItemSelectionModel::SelectionFlag currentSelectionAction;
	QModelIndex mouseMovePrevIndex, mouseMoveActionPrevIndex;
	
	bool isSearchMode;
	SearchLineEdit *searchEdit;

signals:
	void EnterSelected();
	void FocusIn();
	void PaletteChanged();
	void QuickSearch(QString search);
	void CurrentRowChanged(int newCurrentRow);
	
public slots:
	void KeyboardSearchNullify();
	void SelectAll(QItemSelectionModel::SelectionFlag selectAction, bool excludeCurrent = false);
};

#endif // FILELISTVIEW_H