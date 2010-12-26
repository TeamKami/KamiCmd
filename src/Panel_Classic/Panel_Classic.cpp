#include "Panel_Classic.h"

Panel_Classic::Panel_Classic(QWidget *parent)
	: IPanel(parent)
{
	view = new FileListView();
	view->model = new FileListModel();
	view->sort = new SortModel();

	pathEdit = new QLineEdit(this);
	pathEdit->setFocusPolicy(Qt::ClickFocus);

	view->setItemsExpandable(false);
	view->setRootIsDecorated(false);
	view->setSelectionMode(QAbstractItemView::NoSelection);

	view->sort->setSourceModel(view->model);
	view->setModel(view->sort);

	view->setAllColumnsShowFocus(true);
	view->setUniformRowHeights(true);
	view->setSortingEnabled(true);
	view->sortByColumn(0, Qt::AscendingOrder);
	setFocusProxy(view);

	layout = new QVBoxLayout(this);
	layout->addWidget(pathEdit);
	layout->addWidget(view);
	layout->setMargin(0);

	connect(view, SIGNAL(EnterSelected()), this, SLOT(EnterSelected()));
	connect(pathEdit, SIGNAL(returnPressed()), this, SLOT(pathEditReturnPressed()));
	connect(view->model, SIGNAL(modelReset()), view, SLOT(keyboardSearchNullify()));
	connect(view->model, SIGNAL(PathChanged()), this, SIGNAL(TextChanged()));
	connect(view, SIGNAL(FocusIn()), this, SIGNAL(FocusIn()));
	//connect(list, SIGNAL(list->header()->mouseDoubleClickEvent()), list, SLOT(list->header()->))

	view->model->SetPath(QApplication::applicationDirPath()); // Kinda crutch. Should fix someday
}

void Panel_Classic::Create(IUnit *createdFrom)
{
	if (IPanel *panel = dynamic_cast<IPanel *>(createdFrom))
		SetPath(panel->GetPath());
}

void Panel_Classic::Link( IUnit *withUnit )
{
	inactivePanel = withUnit;
}

void Panel_Classic::SetPath( QString path )
{
	QModelIndex current = view->model->SetPath(path);
	if (current.isValid())
		view->setCurrentIndex(view->sort->mapFromSource(current));
	else
		view->setCurrentIndex(view->sort->index(0, 0));
	pathEdit->setText(view->model->GetPath());
}

void Panel_Classic::EnterSelected()
{
	QModelIndex current = view->model->Enter( view->model->index(view->sort->mapToSource(view->currentIndex()).row()) );
	if (current.isValid())
		view->setCurrentIndex( view->sort->mapFromSource(current) );
	else
		view->setCurrentIndex(view->sort->index(0, 0));
	pathEdit->setText(view->model->GetPath());
}

void Panel_Classic::pathEditReturnPressed()
{
	emit SetPath(pathEdit->text());
}

QString Panel_Classic::GetPath()
{
	return view->model->GetPath();
}

QString Panel_Classic::GetText()
{
	QString str = view->model->GetPath();
	if (!view->model->GetFs()->isRoot())
	{
		int pos = str.lastIndexOf("/", -2) + 1;
		str = str.mid(str.lastIndexOf("/", -2) + 1, str.length() - pos);
	}
	return str;
}

const FileInfo *const Panel_Classic::GetCurrentFile()
{
	return view->model->GetFileInfo(view->sort->mapToSource(view->currentIndex()).row());
}

QVector<const FileInfo*> Panel_Classic::GetSelectedFiles()
{
	QVector<const FileInfo*> arr;
	int i = 0, n = view->model->selectedNum;

	if (view->model->rowCount())
	{
		if (n)
		{
			const FileInfo* info = view->model->GetFileInfo( view->sort->mapToSource(view->sort->index(0, 0)).row() );
			if (info->name == "..")
				i++;

			arr.reserve(n - i);
			for (; i < view->model->rowCount(); i++ )
			{
				const FileInfo* info = view->model->GetFileInfo( view->sort->mapToSource(view->sort->index(i, 0)).row() );
				if (info->selected)
					arr.append(info);
			}
		}
		else
			arr.append(GetCurrentFile());
	}
	return arr;
}

bool Panel_Classic::SetCurrentIndex( int index )
{
	if (index > 0 && index < view->model->rowCount())
	{
		view->setCurrentIndex(view->sort->index(index, 0));
		return true;
	}
	return false;
}

int Panel_Classic::GetCurrentIndex()
{
	return view->currentIndex().row();
}

void Panel_Classic::SaveState( QSettings &set )
{
	set.setValue("Path", GetPath());
	set.setValue("CurrentIndex", GetCurrentIndex());
}

void Panel_Classic::LoadState( QSettings &set )
{
	SetPath(set.value("Path", QApplication::applicationDirPath()).toString());
	SetCurrentIndex(set.value("CurrentIndex", 0).toInt());
}