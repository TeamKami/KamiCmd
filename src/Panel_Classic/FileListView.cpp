#include "FileListView.h"

//#define DARK

void FileListView::keyPressEvent( QKeyEvent *event )
{
	int overridden_key = 0, overridden_modifiers = 0;
	QModelIndex wasCurrent = currentIndex();

	bool isControlEnter = (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) && (event->modifiers() & Qt::ControlModifier);
	switch (event->key())
	{ 
	case Qt::Key_Shift:
		//if (currentKeyboardSelectAction == FileListModel::SA_None)
		{
			if (model_->isSelected(sort_->mapToSource(currentIndex())))
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
		overridden_key = Qt::Key_Down;
		overridden_modifiers = Qt::ShiftModifier;
		break;

	case Qt::Key_Enter:
	case Qt::Key_Return:
	case Qt::Key_Escape:
			if (!isControlEnter)
				emit EnterSelected();
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
			setDirtyRegion(QRegion(0, 0, width(), height()));
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
				if (sort_->index(i, 0).data(Qt::DisplayRole).toString().indexOf(searchEdit->text(), 0, Qt::CaseInsensitive) != -1)
				{
					setCurrentIndex(sort_->index(i, 0));
					isFound = true;
					break;
				}
			if (!isFound)
			{
				for (int i = sort_->rowCount() - 1; i > currentIndex().row(); i--)
					if (sort_->index(i, 0).data(Qt::DisplayRole).toString().indexOf(searchEdit->text(), 0, Qt::CaseInsensitive) != -1)
					{
						setCurrentIndex(sort_->index(i, 0));
						isFound = true;
						break;
					}
			}
		}
		else
		{
			for (int i = currentIndex().row() + (isControlEnter ? 1 : 0); i < sort_->rowCount(); i++)
				if (sort_->index(i, 0).data(Qt::DisplayRole).toString().indexOf(searchEdit->text(), 0, Qt::CaseInsensitive) != -1)
				{
					setCurrentIndex(sort_->index(i, 0));
					isFound = true;
					break;
				}
			if (!isFound)
			{
				for (int i = 0; i < currentIndex().row(); i++)
					if (sort_->index(i, 0).data(Qt::DisplayRole).toString().indexOf(searchEdit->text(), 0, Qt::CaseInsensitive) != -1)
					{
						setCurrentIndex(sort_->index(i, 0));
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
                        QTreeView::keyPressEvent(&QKeyEvent(event->type(), overridden_key, //FIXME: Warning:Taking address of temporary.
			event->modifiers() & overridden_modifiers, event->text(), event->isAutoRepeat(), event->count()));
		else
			QTreeView::keyPressEvent(event);
	}

	if (event->modifiers() & Qt::ShiftModifier || overridden_modifiers & Qt::ShiftModifier)
	{
		QModelIndex current = currentIndex();
		switch (event->key())
		{ 
		case Qt::Key_Up:
		case Qt::Key_Down:
		case Qt::Key_Insert:
			model_->Select(sort_->mapToSource(wasCurrent), currentSelectionAction);
			if (wasCurrent.row() == model_->rowCount() - 1 || !wasCurrent.row())
				for (int i = 0; i < model_->columnCount(); i++)
					update( sort_->index(wasCurrent.row(), i) );
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
				list[i - wasCurrent.row()] = sort_->mapToSource(sort_->index(i, 0));
			model_->SelectRange(list, currentSelectionAction);

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

void FileListView::focusInEvent( QFocusEvent * /*event*/ )
{
	model_->isFocused = true;
	for (int i = 0; i < model_->columnCount(); i++)
		update( sort_->index(currentIndex().row(), i) );
	emit FocusIn();
}

void FileListView::focusOutEvent( QFocusEvent * /*event*/ )
{
	model_->isFocused = false;
	for (int i = 0; i < model_->columnCount(); i++)
		update( sort_->index(currentIndex().row(), i) );
}

void FileListView::currentChanged(const QModelIndex &current, const QModelIndex &previous) // overload
{
	model_->selection = sort_->mapToSource(current);
	// QT 4.6 CRUTCH. REMOVE WHEN NOT NEEDED
// 	{
// 		QRect rect;
// 		rect = visualRect(current);
// 		rect.setLeft(0);
// 		rect.setRight(width());
// 		setDirtyRegion(rect);
// 
// 		rect = visualRect(previous);
// 		rect.setLeft(0);
// 		rect.setRight(width());
// 		setDirtyRegion(rect);
// 	}

	QAbstractItemView::currentChanged(current, previous);
}

void FileListView::mousePressEvent( QMouseEvent *event )
{
	if (event->button() == Qt::RightButton)
	{
		QModelIndex index = indexAt(event->pos());
		if (!index.isValid())
		{
			index = sort_->index(model_->rowCount() - 1, 0);
			setCurrentIndex(index);
		}

		if ( model_->isSelected(sort_->mapToSource(index)) )
			currentSelectionAction = FileListModel::SA_Deselect;
		else
			currentSelectionAction = FileListModel::SA_Select;

		mouseMovePrevIndex = index;

		model_->Select(sort_->mapToSource(index), currentSelectionAction);
		for (int i = 0; i < model_->columnCount(); i++)
			update( sort_->index(index.row(), i) );
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
			index = sort_->index(event->pos().y() < 0 ? 0 : model_->rowCount() - 1, 0);

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
					list[i] = sort_->mapToSource(sort_->index(i + start, 0));

				model_->SelectRange(list, currentSelectionAction);

				for (int i = start; i < end; i++)
					for (int j = 0; j < model_->columnCount(); j++)
						update( sort_->index(i, j) );
			}
			else
				model_->Select(sort_->mapToSource(index), currentSelectionAction);
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
			index = sort_->index(event->pos().y() < 0 ? 0 : model_->rowCount() - 1, 0);
		currentSelectionAction = model_->GetFileInfo(sort_->mapToSource(index).row())->selected ? FileListModel::SA_Select : FileListModel::SA_Deselect;

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
	list.resize(model_->rowCount() - excludeCurrent);

	int currentRow = sort_->mapToSource(currentIndex()).row();

	for (int i = 0, shift = 0; i < model_->rowCount(); i++)
	{
		if (shift)
			list[i - shift] = model_->index(i);

		if (excludeCurrent && currentRow == model_->index(i).row())
			shift++;
		if (!shift)
			list[i] = model_->index(i);
	}
	model_->SelectRange(list, selectAction);

	setDirtyRegion(QRegion(0, 0, width(), height())); // Kinda not the best solution. Should fix someday
}

void FileListView::wheelEvent( QWheelEvent *event )
{
	if (!model_->isFocused)
		return;

	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

// 	if (abs(numSteps) > 1)
// 	{
// 		__asm int 3;
// 	
// 	}
// 	int newRow = qMin(model_->rowCount() - 1, qMax(0, currentIndex().row() - numSteps));
// 	for (int i = currentIndex().row(); ; numSteps > 0 ? i-- : i++)
// 	{
// 		setCurrentIndex(sort_->index(i , 0));
// 		if (i == newRow)
// 			break;
// 	}

	setCurrentIndex(sort_->index( qMin(model_->rowCount() - 1, qMax(0, currentIndex().row() - numSteps)) , 0));
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

FileListView::FileListView()
	:currentSelectionAction(FileListModel::SA_None), isSearchMode(false)
{
	searchEdit = new SearchLineEdit(this);
	searchEdit->setVisible(isSearchMode);
	searchEdit->setFocusPolicy(Qt::ClickFocus);

// 	QPalette pal = palette();
// 	pal.setColor(QPalette::AlternateBase, QColor(248, 248, 248));
// 	setPalette(pal);

#ifdef DARK
	QPalette pal = palette();
	pal.setColor(QPalette::Base, QColor(33, 38, 38));
	setPalette(pal);
#endif
}

FileListModel * FileListView::Model()
{
	return model_;
}

void FileListView::SetModel( FileListModel *model )
{
	model_ = model;
}

SortModel * FileListView::Sort()
{
	return sort_;
}

void FileListView::SetSortModel( SortModel *sort )
{
	sort_ = sort;
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
