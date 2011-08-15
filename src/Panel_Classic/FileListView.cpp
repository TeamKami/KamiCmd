#include "FileListView.h"
#include "FileListHeader.h"

FileListView::FileListView()
	: isSearchMode(false), currentSelectionAction(QItemSelectionModel::NoUpdate)
{
	searchEdit = new SearchLineEdit(this);
	searchEdit->setVisible(isSearchMode);
	searchEdit->setFocusPolicy(Qt::ClickFocus);
	setDragDropMode(QAbstractItemView::DragDrop);
	//setDropIndicatorShown(true);
	//setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	//viewport()->setStyleSheet("background-color: white;");

	//setStyleSheet("QTreeView::item { padding: 6px 14px 0px 14px; }");

	// 	QPalette pal = palette();
	// 	pal.setColor(QPalette::AlternateBase, QColor(248, 248, 248));
	// 	setPalette(pal);

	setFrameStyle(QFrame::Plain);
	setHeader(new FileListHeader(Qt::Horizontal, this));


#ifdef DARK
	QPalette pal = palette();
	pal.setColor(QPalette::Base, QColor(33, 38, 38));
	setPalette(pal);
#endif
}

void FileListView::keyPressEvent( QKeyEvent *event )
{
	int overridden_key = 0;
	QModelIndex wasCurrent = currentIndex();

	bool isControlEnter = (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && (event->modifiers() & Qt::ControlModifier);
	switch (event->key())
	{ 
	case Qt::Key_Shift:
		currentSelectionAction = selectionModel()->isSelected(currentIndex()) ? QItemSelectionModel::Deselect : QItemSelectionModel::Select;
		break;

	case Qt::Key_Plus:
	case Qt::Key_Minus:
	case Qt::Key_Asterisk:
		SelectAll(event->key() == Qt::Key_Plus ? QItemSelectionModel::Select :
			event->key() == Qt::Key_Minus ? QItemSelectionModel::Deselect :
			QItemSelectionModel::Toggle);
		break;

	case Qt::Key_Left:
		overridden_key = Qt::Key_PageUp;
		break;
	case Qt::Key_Right:
		overridden_key = Qt::Key_PageDown;
		break;

	case Qt::Key_Insert:
		overridden_key = Qt::Key_Down;
		selectionModel()->select(currentIndex(), QItemSelectionModel::Toggle);
		break;

	case Qt::Key_Enter:
	case Qt::Key_Return:
		if (!isControlEnter)
		{
			if (event->modifiers() & Qt::ShiftModifier)
				emit OpenSelected();
			else 
				emit EnterSelected();
		}
		break;
	}

	if (isSearchMode && !isControlEnter)
		switch (event->key())
		{
		case Qt::Key_Return:
		case Qt::Key_Enter:
		case Qt::Key_Escape:
		case Qt::Key_Left:
		case Qt::Key_Right:
		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_PageUp:
		case Qt::Key_PageDown:
		case Qt::Key_Home:
		case Qt::Key_End:
			isSearchMode = false;
			searchEdit->setText("");
			searchEdit->setVisible(false);
			emit QuickSearch(searchEdit->text());
			break;
		}

	bool modified = (event->modifiers() & (Qt::ControlModifier | Qt::AltModifier | Qt::MetaModifier));
        if ((isControlEnter && isSearchMode) || (!event->text().isEmpty() && event->text() != "\x0d" && !modified && !edit(currentIndex(), AnyKeyPressed, event)))
	{
		if (!isSearchMode)
		{
			isSearchMode = true;
			searchEdit->setVisible(true);
		}
		if (!isControlEnter)
		{
			searchEdit->keyPressEvent(event);
			emit QuickSearch(searchEdit->text());
		}

		bool isFound = false;
		if (event->modifiers() & Qt::ShiftModifier)
		{ // Is quick search order reversed?
			for (int i = currentIndex().row() - (isControlEnter ? 1 : 0); i >= 0; i--)
				if (model()->index(i, 0).data(Qt::DisplayRole).toString().indexOf(searchEdit->text(), 0, Qt::CaseInsensitive) != -1)
				{
					setCurrentIndex(model()->index(i, 0));
					isFound = true;
					break;
				}
			if (!isFound)
			{
				for (int i = model()->rowCount() - 1; i > currentIndex().row(); i--)
					if (model()->index(i, 0).data(Qt::DisplayRole).toString().indexOf(searchEdit->text(), 0, Qt::CaseInsensitive) != -1)
					{
						setCurrentIndex(model()->index(i, 0));
						isFound = true;
						break;
					}
			}
		}
		else
		{
			for (int i = currentIndex().row() + (isControlEnter ? 1 : 0); i < model()->rowCount(); i++)
				if (model()->index(i, 0).data(Qt::DisplayRole).toString().indexOf(searchEdit->text(), 0, Qt::CaseInsensitive) != -1)
				{
					setCurrentIndex(model()->index(i, 0));
					isFound = true;
					break;
				}
			if (!isFound)
			{
				for (int i = 0; i < currentIndex().row(); i++)
					if (model()->index(i, 0).data(Qt::DisplayRole).toString().indexOf(searchEdit->text(), 0, Qt::CaseInsensitive) != -1)
					{
						setCurrentIndex(model()->index(i, 0));
						isFound = true;
						break;
					}
			}
		}

		if (!isControlEnter)
		{
			static bool wasFound = true;
			if (wasFound != isFound)
			{
				wasFound = isFound;

				QPalette pal = searchEdit->palette();
				pal.setColor(QPalette::Base, QColor(255, 200, 200));
				if (isFound)
					pal = palette();
				searchEdit->setPalette(pal);
			}
		}

		setDirtyRegion(QRegion(0, 0, width(), height()));
		//keyboardSearch(event->text());
		event->accept();
	}
	else
	{
		event->ignore();

		if (overridden_key)
		{
			QKeyEvent newEvent(event->type(), overridden_key, event->modifiers(), event->text(), event->isAutoRepeat(), event->count());
			QTreeView::keyPressEvent(&newEvent);
		}
		else
			QTreeView::keyPressEvent(event);
	}

	if (event->modifiers() & Qt::ShiftModifier)
	{
		QModelIndex current = currentIndex();
		switch (event->key())
		{ 
		case Qt::Key_Up:
		case Qt::Key_Down:
			selectionModel()->select(wasCurrent, currentSelectionAction);
			break;

		case Qt::Key_Left:
		case Qt::Key_PageUp:
		case Qt::Key_Right:
		case Qt::Key_PageDown:
		case Qt::Key_Home:
		case Qt::Key_End:
			if (current.row() < wasCurrent.row())
				qSwap(wasCurrent, current);

			QItemSelection toSelect;
			toSelect << QItemSelectionRange(model()->index(wasCurrent.row(), 0), model()->index(current.row(), 1));
			selectionModel()->select(toSelect, currentSelectionAction);
			break;
		}
	}
}


void FileListView::keyReleaseEvent( QKeyEvent *event )
{
	if (event->key() == Qt::Key_Shift)
		currentSelectionAction = QItemSelectionModel::NoUpdate;

	QTreeView::keyReleaseEvent(event);
}

void FileListView::KeyboardSearchNullify()
{
	QAbstractItemView::keyboardSearch(QString());
}

void FileListView::focusInEvent( QFocusEvent * /*event*/ )
{
	for (int i = 0; i < model()->columnCount(); i++)
		update( model()->index(currentIndex().row(), i) );
	emit FocusIn();
}

void FileListView::focusOutEvent( QFocusEvent * /*event*/ )
{
	for (int i = 0; i < model()->columnCount(); i++)
		update( model()->index(currentIndex().row(), i) );
}

void FileListView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
	emit CurrentRowChanged(current.row());

	for (int i = 0; i < model()->columnCount(); i++)
		update( model()->index(previous.row(), i) );
	for (int i = 0; i < model()->columnCount(); i++)
		update( model()->index(current.row(), i) );

	QAbstractItemView::currentChanged(current, previous);
}

void FileListView::mousePressEvent( QMouseEvent *event )
{
	if (event->button() == Qt::RightButton || event->button() == Qt::LeftButton && event->modifiers() & Qt::ControlModifier)
	{
		QModelIndex index = indexAt(event->pos());
		if (!index.isValid())
		{
			index = model()->index(model()->rowCount() - 1, 0);
			setCurrentIndex(index);
		}

		if (selectionModel()->isSelected(index))
			currentSelectionAction = QItemSelectionModel::Deselect;
		else
			currentSelectionAction = QItemSelectionModel::Select;

		mouseMovePrevIndex = index;

		selectionModel()->select(index, currentSelectionAction);
	}
	else if (event->button() == Qt::LeftButton && event->modifiers() & Qt::ShiftModifier)
	{
		QModelIndex indexPrev = currentIndex(), index = indexAt(event->pos());
		if (!index.isValid())
		{
			index = model()->index(model()->rowCount() - 1, 0);
			setCurrentIndex(index);
		}

		if (index.row() < indexPrev.row())
			qSwap(index, indexPrev);

		QItemSelection toSelect;
		toSelect << QItemSelectionRange(model()->index(indexPrev.row(), 0), model()->index(index.row(), 1));
		selectionModel()->select(toSelect, currentSelectionAction);
	}
	else if (event->button() == Qt::MidButton)
	{
		EnterSelected();
		return;
	}

	QAbstractItemView::mousePressEvent(event);
}

void FileListView::mouseMoveEvent( QMouseEvent *event )
{
	if (event->buttons() & Qt::RightButton)
	{
		QModelIndex index = indexAt(QPoint(0, event->pos().y()));
		if (!index.isValid())
			index = model()->index(event->pos().y() < 0 ? 0 : model()->rowCount() - 1, 0);

		if (index != mouseMoveActionPrevIndex)
		{
			mouseMoveActionPrevIndex = index;
			setCurrentIndex(index);

			if (index.row() != mouseMovePrevIndex.row())
			{
				int start = qMin(index.row(), mouseMovePrevIndex.row()),
					end = qMax(index.row(), mouseMovePrevIndex.row());

				QItemSelection toSelect;
				toSelect << QItemSelectionRange(model()->index(start, 0), model()->index(end, 1));
				selectionModel()->select(toSelect, currentSelectionAction);
			}
			else
				selectionModel()->select(index, currentSelectionAction);
		}
	}
	else
		QAbstractItemView::mouseMoveEvent(event);
}

void FileListView::mouseReleaseEvent( QMouseEvent *event )
{
	if (event->button() == Qt::RightButton)
	{
		currentSelectionAction = QItemSelectionModel::NoUpdate;
		event->setAccepted(true);
	}
	else
		QAbstractItemView::mouseReleaseEvent(event);
}

void FileListView::mouseDoubleClickEvent(QMouseEvent *event)
{
	switch (event->button())
	{
	case Qt::LeftButton:
		if (!event->modifiers())
		{
			emit EnterSelected();
			break;
		}
		else if (event->modifiers() != Qt::ControlModifier)
			break;
	case Qt::RightButton:
		{
			QModelIndex index = indexAt(QPoint(0, event->pos().y()));
			if (!index.isValid())
				index = model()->index(event->pos().y() < 0 ? 0 : model()->rowCount() - 1, 0);

			SelectAll(QItemSelectionModel::Toggle, true);
		}
		break;

	case Qt::MidButton:
		EnterSelected();
		break;

	default:
		QTreeView::mouseDoubleClickEvent(event);
	}
}

void FileListView::SelectAll( QItemSelectionModel::SelectionFlag selectAction, bool excludeCurrent /*= false*/ )
{
	QItemSelection toSelect;
	if (excludeCurrent)
	{
		toSelect << QItemSelectionRange(model()->index(0, 0), model()->index(currentIndex().row() - 1, 1));
		if (currentIndex().row() < model()->rowCount() - 1)
			toSelect << QItemSelectionRange(model()->index(currentIndex().row() + 1, 0), model()->index(model()->rowCount() - 1, 1));
	}
	else
		toSelect << QItemSelectionRange(model()->index(0, 0), model()->index(model()->rowCount(), 1));

	selectionModel()->select(toSelect, selectAction);
}

void FileListView::wheelEvent( QWheelEvent *event )
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

// 	if (abs(numSteps) > 1)
// 	{
// 		__asm int 3;
// 	
// 	}
// 	int newRow = qMin(model()->rowCount() - 1, qMax(0, currentIndex().row() - numSteps));
// 	for (int i = currentIndex().row(); ; numSteps > 0 ? i-- : i++)
// 	{
// 		setCurrentIndex(model()->index(i , 0));
// 		if (i == newRow)
// 			break;
// 	}

	setCurrentIndex(model()->index( qMin(model()->rowCount() - 1, qMax(0, currentIndex().row() - numSteps)) , 0));
	event->accept();
	return;
	QTreeView::wheelEvent(event);
}

void FileListView::resizeEvent( QResizeEvent *event )
{
	setColumnWidth(0, event->size().width() - 66);
	setColumnWidth(1, 66);
	QTreeView::resizeEvent(event);
}

void FileListView::keyboardSearch( const QString &search )
{
	searchEdit->insert(search);
}

void FileListView::changeEvent( QEvent *event )
{
	if (event->type() == QEvent::PaletteChange)
		emit PaletteChanged();
	return QTreeView::changeEvent(event);
}