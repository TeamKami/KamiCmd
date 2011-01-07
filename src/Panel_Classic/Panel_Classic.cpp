#include "Panel_Classic.h"

Panel_Classic::Panel_Classic(QWidget *parent)
	: IPanel(parent)
{
	view = new FileListView();
	view->SetModel(new FileListModel());
	view->SetSortModel(new SortModel());

	pathEdit = new QLineEdit(this);
	pathEdit->setFocusPolicy(Qt::ClickFocus);

	view->setItemsExpandable(false);
	view->setRootIsDecorated(false);
//	view->setSelectionMode(QAbstractItemView::NoSelection);

	view->Sort()->setSourceModel(view->Model());
	view->setModel(view->Sort());

//	view->setAllColumnsShowFocus(true);
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
	connect(view->Model(), SIGNAL(modelReset()), view, SLOT(keyboardSearchNullify()));
	connect(view->Model(), SIGNAL(PathChanged()), this, SIGNAL(TextChanged()));
	connect(view, SIGNAL(FocusIn()), this, SIGNAL(FocusIn()));
	//connect(list, SIGNAL(list->header()->mouseDoubleClickEvent()), list, SLOT(list->header()->))

	view->Model()->SetPath(QApplication::applicationDirPath()); // Kinda crutch. Should fix someday
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
	QModelIndex current = view->Model()->SetPath(path);
	if (current.isValid())
		view->setCurrentIndex(view->Sort()->mapFromSource(current));
	else
		view->setCurrentIndex(view->Sort()->index(0, 0));
	pathEdit->setText(view->Model()->GetPath());
}

void Panel_Classic::EnterSelected()
{
	QModelIndex current = view->Model()->Enter( view->Model()->index(view->Sort()->mapToSource(view->currentIndex()).row()) );
	if (current.isValid())
		view->setCurrentIndex( view->Sort()->mapFromSource(current) );
	else
		view->setCurrentIndex(view->Sort()->index(0, 0));
	pathEdit->setText(view->Model()->GetPath());
}

void Panel_Classic::pathEditReturnPressed()
{
	emit SetPath(pathEdit->text());
}

QString Panel_Classic::GetPath()
{
	return view->Model()->GetPath();
}

QString Panel_Classic::GetText()
{
	QString str = view->Model()->GetPath();
	if (!view->Model()->GetFs()->isRoot())
	{
		int pos = str.lastIndexOf("/", -2) + 1;
		str = str.mid(str.lastIndexOf("/", -2) + 1, str.length() - pos - 1);
	}
	return str;
}

const FileInfo *const Panel_Classic::GetCurrentFile()
{
	return view->Model()->GetFileInfo(view->Sort()->mapToSource(view->currentIndex()).row());
}

QVector<const FileInfo*> Panel_Classic::GetSelectedFiles()
{
	QVector<const FileInfo*> arr;
	int i = 0, n = view->Model()->selectedNum;

	if (view->Model()->rowCount())
	{
		if (n)
		{
			const FileInfo* info = view->Model()->GetFileInfo( view->Sort()->mapToSource(view->Sort()->index(0, 0)).row() );
			if (info->name == "..")
				i++;

			arr.reserve(n - i);
			for (; i < view->Model()->rowCount(); i++ )
			{
				const FileInfo* info = view->Model()->GetFileInfo( view->Sort()->mapToSource(view->Sort()->index(i, 0)).row() );
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
	if (index > 0 && index < view->Model()->rowCount())
	{
		view->setCurrentIndex(view->Sort()->index(index, 0));
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