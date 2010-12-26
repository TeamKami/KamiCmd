#include "FileListView.h"

//#define DARK

void FileListView::keyPressEvent( QKeyEvent *event )
{
	int overridden_key = 0;
	QModelIndex wasCurrent = currentIndex();

	switch (event->key())
	{ 
	case Qt::Key_Shift:
		//if (currentKeyboardSelectAction == FileListModel::SA_None)
		{
			if (model->isSelected(sort->mapToSource(currentIndex())))
				currentSelectionAction = FileListModel::SA_Deselect;
			else
				currentSelectionAction = FileListModel::SA_Select;
		}
		break;

	case Qt::Key_Plus:
	case Qt::Key_Minus:
	case Qt::Key_Asterisk:
		SelectAll(event->key() == Qt::Key_Plus ? FileListModel::SA_Select :
			event->key() == Qt::Key_Minus ? FileListModel::SA_Deselect :
			FileListModel::SA_Toggle);
		break;

	case Qt::Key_Left:
		overridden_key = Qt::Key_PageUp;
		break;
	case Qt::Key_Right:
		overridden_key = Qt::Key_PageDown;
		break;

	case Qt::Key_Insert:
		model->Select(sort->mapToSource(currentIndex()));
		{
			if (currentIndex().row() + 1 < model->rowCount())
			{
				const QModelIndex newSel = sort->index(currentIndex().row() + 1, 0);
				selectionModel()->setCurrentIndex(newSel, QItemSelectionModel::NoUpdate);
				//scrollTo(newSel);
			}
			else
				for (int i = 0; i < model->columnCount(); i++)
					update( sort->index(currentIndex().row(), i) );
		}
		break;

	case Qt::Key_Enter:
	case Qt::Key_Return:
		emit EnterSelected();
		break;
	}

	if (overridden_key)
		QTreeView::keyPressEvent(&QKeyEvent(event->type(), overridden_key,
		event->modifiers(), event->text(), event->isAutoRepeat(), event->count()));
	else
		QTreeView::keyPressEvent(event);

	if (event->modifiers() & Qt::ShiftModifier)
	{
		QModelIndex current = currentIndex();
		switch (event->key())
		{ 
		case Qt::Key_Up:
		case Qt::Key_Down:
			model->Select(sort->mapToSource(wasCurrent), currentSelectionAction);
			if (wasCurrent.row() == model->rowCount() - 1 || !wasCurrent.row())
				for (int i = 0; i < model->columnCount(); i++)
					update( sort->index(wasCurrent.row(), i) );
			break;

		case Qt::Key_Left:
		case Qt::Key_PageUp:
		case Qt::Key_Right:
		case Qt::Key_PageDown:
		case Qt::Key_Home:
		case Qt::Key_End:
			if (current.row() < wasCurrent.row())
				qSwap(wasCurrent, current);

			QVector<QModelIndex> list;
			list.resize(current.row() - wasCurrent.row() + 1);

			for (int i = wasCurrent.row(); i <= current.row(); i++)
				list[i - wasCurrent.row()] = sort->mapToSource(sort->index(i, 0));
			model->SelectRange(list, currentSelectionAction);

			setDirtyRegion(QRegion(0, 0, width(), height())); // Kinda not the best solution. Should fix someday
			break;
		}
	}
}


void FileListView::keyReleaseEvent( QKeyEvent *event )
{
	if (event->key() == Qt::Key_Shift)
		currentSelectionAction = FileListModel::SA_None;

	QTreeView::keyReleaseEvent(event);
}

void FileListView::keyboardSearchNullify()
{
	QAbstractItemView::keyboardSearch(QString());
}

// void FileListView::drawRow( QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index ) const
// {
// 	QTreeView::drawRow(painter, option, index);
// }

void FileListView::focusInEvent( QFocusEvent *event )
{
	model->isFocused = true;
	for (int i = 0; i < model->columnCount(); i++)
		update( sort->index(currentIndex().row(), i) );
	emit FocusIn();
}

void FileListView::focusOutEvent( QFocusEvent *event )
{
	model->isFocused = false;
	for (int i = 0; i < model->columnCount(); i++)
		update( sort->index(currentIndex().row(), i) );
}

void FileListView::currentChanged(const QModelIndex &current, const QModelIndex &previous) // overload
{
	model->selection = sort->mapToSource(current);
	// QT 4.6 CRUTCH. REMOVE WHEN NOT NEEDED
	{
		QRect rect;
		rect = visualRect(current);
		rect.setLeft(0);
		rect.setRight(width());
		setDirtyRegion(rect);

		rect = visualRect(previous);
		rect.setLeft(0);
		rect.setRight(width());
		setDirtyRegion(rect);
	}

	QAbstractItemView::currentChanged(current, previous);
}

void FileListView::mousePressEvent( QMouseEvent *event )
{
	if (event->button() == Qt::RightButton)
	{
		QModelIndex index = indexAt(event->pos());
		if (!index.isValid())
		{
			index = sort->index(model->rowCount() - 1, 0);
			setCurrentIndex(index);
		}

		if ( model->isSelected(sort->mapToSource(index)) )
			currentSelectionAction = FileListModel::SA_Deselect;
		else
			currentSelectionAction = FileListModel::SA_Select;

		mouseMovePrevIndex = index;

		model->Select(sort->mapToSource(index), currentSelectionAction);
		for (int i = 0; i < model->columnCount(); i++)
			update( sort->index(index.row(), i) );
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
			index = sort->index(event->pos().y() < 0 ? 0 : model->rowCount() - 1, 0);

		if (index != mouseMoveActionPrevIndex)
		{
			mouseMoveActionPrevIndex = index;
			setCurrentIndex(index);

			if (index.row() != mouseMovePrevIndex.row())
			{
				int start = qMin(index.row(), mouseMovePrevIndex.row()),
					end = qMax(index.row(), mouseMovePrevIndex.row()) + 1;

				QVector<QModelIndex> list;
				list.resize(end - start);

				for (int i = 0; i < list.size(); i++)
					list[i] = sort->mapToSource(sort->index(i + start, 0));

				model->SelectRange(list, currentSelectionAction);

				for (int i = start; i < end; i++)
					for (int j = 0; j < model->columnCount(); j++)
						update( sort->index(i, j) );
			}
			else
				model->Select(sort->mapToSource(index), currentSelectionAction);
		}
	}
	else
		QAbstractItemView::mouseMoveEvent(event);
}

void FileListView::mouseReleaseEvent( QMouseEvent *event )
{
	if (event->button() == Qt::RightButton)
	{
		currentSelectionAction = FileListModel::SA_None;
		event->setAccepted(true);
	}
	else
		QAbstractItemView::mouseReleaseEvent(event);
}

void FileListView::mouseDoubleClickEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		emit EnterSelected();
	else if (event->button() == Qt::RightButton)
	{
		QModelIndex index = indexAt(QPoint(0, event->pos().y()));
		if (!index.isValid())
			index = sort->index(event->pos().y() < 0 ? 0 : model->rowCount() - 1, 0);
		currentSelectionAction = model->GetFileInfo(sort->mapToSource(index).row())->selected ? FileListModel::SA_Select : FileListModel::SA_Deselect;

		SelectAll(FileListModel::SA_Toggle, true);
	}
	else if (event->button() == Qt::MidButton)
		EnterSelected();
	else
		QTreeView::mouseDoubleClickEvent(event);
}

void FileListView::SelectAll( int selectAction /*= FileListModel::SA_Select*/, bool excludeCurrent /*= false*/ )
{
	QVector<QModelIndex> list;
	list.resize(model->rowCount() - excludeCurrent);

	int currentRow = sort->mapToSource(currentIndex()).row();

	for (int i = 0, shift = 0; i < model->rowCount(); i++)
	{
		if (shift)
			list[i - shift] = model->index(i);

		if (excludeCurrent && currentRow == model->index(i).row())
			shift++;
		if (!shift)
			list[i] = model->index(i);
	}
	model->SelectRange(list, selectAction);

	setDirtyRegion(QRegion(0, 0, width(), height())); // Kinda not the best solution. Should fix someday
}

void FileListView::wheelEvent( QWheelEvent *event )
{
	if (!model->isFocused)
		return;

	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	setCurrentIndex(sort->index( qMin(model->rowCount() - 1, qMax(0, currentIndex().row() - numSteps)) , 0));

	return;
	QTreeView::wheelEvent(event);
}

void FileListView::resizeEvent( QResizeEvent *event )
{
	setColumnWidth(0, event->size().width() - 66);
	setColumnWidth(1, 66);
	QTreeView::resizeEvent(event);
}

FileListView::FileListView()
	:currentSelectionAction(FileListModel::SA_None)
{
#ifdef DARK
	QPalette pal = palette();
	pal.setColor(QPalette::Base, QColor(33, 38, 38));
	setPalette(pal);
#endif
}