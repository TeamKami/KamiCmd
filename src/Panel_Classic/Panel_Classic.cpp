#include "Panel_Classic.h"
#include "FilesDelegate.h"
#include "SelectionModel.h"

Panel_Classic::Panel_Classic(QWidget *parent)
	: IPanel(parent)
{
	QWidget *central = new QWidget(this);

	flView = new FileListView();
	flModel = new FileListModel();
	sortModel = new SortModel();
	sortModel->setSourceModel(flModel);
	flView->setModel(sortModel);
	flView->setSelectionModel(new SelectionModel(flView->model(), this));

	FilesDelegate *filesDelegate = new FilesDelegate(this);
	if (1)
	{
		for (int i = 0; i < flModel->columnCount(); i++)
			flView->setItemDelegateForColumn(i, filesDelegate);
	}
	else
	{
		static bool isFirst = true;
		if (isFirst)
		{
			for (int i = 0; i < flModel->columnCount(); i++)
				flView->setItemDelegateForColumn(i, filesDelegate);
			isFirst = false;
		}
	}

	// flView->setAlternatingRowColors(true);
	// flView->setAllColumnsShowFocus(true);
	flView->setItemsExpandable(false);
	flView->setRootIsDecorated(false);
	flView->setUniformRowHeights(true);
	flView->setSortingEnabled(true);
	flView->sortByColumn(0, Qt::AscendingOrder);
	flView->setSelectionMode(QAbstractItemView::NoSelection);
	flView->setSelectionBehavior(QAbstractItemView::SelectRows);
	setFocusProxy(flView);

	QToolBar *toolbar = addToolBar("lol");
	toolbar->addAction(new QAction(QIcon(":/Images/F5.png"), "omg some test", this));

	pathEdit = new QLineEdit(this);
	pathEdit->setFocusPolicy(Qt::ClickFocus);

	layout = new QVBoxLayout(central);
	layout->addWidget(pathEdit);
	layout->addWidget(flView);
	layout->addWidget(flView->QuickSearchBar());
	layout->setMargin(0);

	setCentralWidget(central);

	connect(flView, SIGNAL(EnterSelected()), SLOT(EnterSelected()));
	connect(flView, SIGNAL(OpenSelected()), SLOT(OpenSelected()));
	connect(pathEdit, SIGNAL(returnPressed()), SLOT(pathEditReturnPressed()));
	connect(flView->model(), SIGNAL(modelReset()), flView, SLOT(KeyboardSearchNullify()));
	connect(flModel, SIGNAL(PathChanged()), this, SIGNAL(TextChanged()));
	connect(flView, SIGNAL(FocusIn()), this, SIGNAL(FocusIn()));
	connect(flView, SIGNAL(PaletteChanged()), filesDelegate, SLOT(PaletteChanged()));
	connect(flView, SIGNAL(QuickSearch(QString)), filesDelegate, SLOT(QuickSearchChanged(QString)));
	connect(flView, SIGNAL(QuickSearch(QString)), flModel, SLOT(QuickSearchChanged(QString)));
	connect(flView, SIGNAL(CurrentRowChanged(int)), filesDelegate, SLOT(CurrentRowChanged(int)));

	flModel->SetPath(QApplication::applicationDirPath()); // Kinda crutch. Should fix someday
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
	flModel->SetPath(path);
	SetCurrentIndex(0);
	pathEdit->setText(flModel->GetPath());
}

void Panel_Classic::EnterSelected()
{
	QModelIndex current = flModel->Enter(flView->currentIndex());
	if (current.isValid())
		flView->setCurrentIndex(sortModel->mapFromSource(current));
	else
		SetCurrentIndex(0);
	pathEdit->setText(flModel->GetPath());
}

void Panel_Classic::pathEditReturnPressed()
{
	emit SetPath(pathEdit->text());
}

QString Panel_Classic::GetPath()
{
	return flModel->GetPath();
}

QString Panel_Classic::GetText()
{
	QString str = flModel->GetPath();
	if (!flModel->GetFs()->isRoot())
	{
		int pos = str.lastIndexOf("/", -2) + 1;
		str = str.mid(str.lastIndexOf("/", -2) + 1, str.length() - pos - 1);
	}
	return str;
}

const FileInfo * Panel_Classic::GetCurrentFile()
{
	return flModel->GetFileInfo(sortModel->mapToSource(flView->currentIndex()).row());
}

const FileInfo * Panel_Classic::SetCurrentFileToPrev()
{
	if (SetCurrentIndex(flView->currentIndex().row() - 1))
		return GetCurrentFile();
	return NULL;
}

const FileInfo * Panel_Classic::SetCurrentFileToNext()
{
	if (SetCurrentIndex(flView->currentIndex().row() + 1))
		return GetCurrentFile();
	return NULL;
}

QVector<FileInfo> Panel_Classic::GetSelectedFiles()
{
	int selectedNum = flView->selectionModel()->selectedRows().count();
	QVector<FileInfo> arr;

	if (flModel->rowCount())
	{
		if (selectedNum)
		{
			arr.reserve(selectedNum);
			foreach(QModelIndex index, flView->selectionModel()->selectedRows())
			{
				FileInfo &info = index.data(FileListModel::FileInfoRole).value<FileInfo>();
				if (!(info.attributes & FileInfo::UpOneLevel))
					arr.append(info);
			}
		}
		else
			arr.append(*GetCurrentFile());
	}
	return arr;
}

bool Panel_Classic::SetCurrentIndex( int index )
{
	if (index > 0 && index < flModel->rowCount())
	{
		flView->setCurrentIndex(flView->model()->index(index, 0));
		return true;
	}
	return false;
}

int Panel_Classic::GetCurrentIndex()
{
	return flView->currentIndex().row();
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

QIcon Panel_Classic::GetIcon()
{
	return QIcon();
}

void Panel_Classic::OpenSelected()
{
	QDesktopServices::openUrl(QUrl::fromLocalFile(flView->currentIndex().data(FileListModel::FileInfoRole).value<FileInfo>().FilePath()));
}
