#include "FileListModel.h"
#include <QLocale>
#include <QColor>

//#define DARK

QString FormatSize(qint64 size)
{
	// Qt note:
	// According to the Si standard KB is 1000 bytes, KiB is 1024
	// but on windows sizes are calculated by dividing by 1024 so we do what they do.
	const quint64 kb = 1024;
	const quint64 mb = 1024 * kb;
	const quint64 gb = 1024 * mb;
	const quint64 tb = 1024 * gb;
	const quint64 pb = 1024 * tb;
	quint64 bytes = size;

	if (bytes >= 10*pb)
		return QLocale().toString((bytes + tb/2) / tb) + QString::fromLatin1(" T");
	if (bytes >= 10*tb)
		return QLocale().toString((bytes + gb/2) / gb) + QString::fromLatin1(" G");
	if (bytes >= 10*gb)
		return QLocale().toString((bytes + mb/2) / mb) + QString::fromLatin1(" M");
	if (bytes >= gb)
		return QLocale().toString((bytes + kb/2) / kb) + QString::fromLatin1(" K");
	return QLocale().toString(bytes);
}

QModelIndex FileListModel::index( int row, int column /*= 0*/, const QModelIndex & /*parent*/ /*= QModelIndex()*/ ) const
{
	if (column > columnCount() || row > list.count() || list.isEmpty())
		return QModelIndex();
	return createIndex(row, column);
}

QModelIndex FileListModel::parent( const QModelIndex& ) const
{
	return QModelIndex();
}

QVariant FileListModel::data( const QModelIndex & index, int role /*= Qt::DisplayRole */ ) const
{
	if (!index.isValid() || index.row() > list.count() || list.isEmpty())
		return QVariant();

	const FileInfo &current = list[index.row()];

	switch (role)
	{
	case Qt::DisplayRole:
		switch (index.column())
		{
		case 0:
			return current.name;
			break;
		case 1:
			return current.name == ".." ? tr("<Up>") :
				current.attributes & FileInfo::Directory ? tr("<Folder>") :
				FormatSize(current.size);
			break;
		}
		break;

	case Qt::UserRole:
		return current.attributes;
		break;

	case Qt::DecorationRole:
		if (!index.column())
			return current.icon;
		break;

	case Qt::ForegroundRole:
#ifdef DARK
		if (index.row() == selection.row() && isFocused)
			return QColor(255, 255, 255);
#endif

		if (current.attributes & FileInfo::Directory)
#ifdef DARK
			return QColor(255, 255, 255);
#else
			return QColor(2, 88, 112);
#endif
		else
		{
			QColor color = Assc->GetTextColor(current.path + current.name);
			return color.isValid() ? color :
#ifdef DARK
				QColor(175, 247, 255);
#else
				QVariant();
#endif
		}
		break;

	case Qt::BackgroundRole:
// 		if (index.row() == selection.row())
// 			return isFocused ? (current.selected ? selectionMarked : selectionActive) : (current.selected ? selectionMarkedInactive : selectionInactive);

		if (current.selected)
#ifdef DARK
			return QColor(70, 90, 123);
#else
			return QColor(255, 215, 188);
#endif // DARK
		else
		{
			QColor color = Assc->GetBackColor(current.path + current.name);
			return color.isValid() ? color : QVariant();
		}
		break;

	case Qt::TextAlignmentRole:
		if (index.column() == 1)
			return int(Qt::AlignRight | Qt::AlignVCenter);
		break;

// 	case Qt::SizeHintRole:
// 		return QSize(10, 40);
	}

/*
	if (role==Qt::DecorationRole && index.column()==0)
	{
		if (list.at(index.row()).isStandardIcon)
		{
			return isDir(index) ? qiFolderIcon : qiFileIcon;
		}
		return *list.at(index.row()).icon;
	}

	if (role==Qt::UserRole )
	{
		switch (index.column())
		{
		case 2:
			return list.at(index.row()).size;
			break;
		}
	}

	if (role==Qt::EditRole && index.column()==0)
	{
		return list.at(index.row()).name;
	}

	if (role==Qt::TextAlignmentRole && index.column()==2)
	{
		if (!isDir(index))
			return Qt::AlignRight;
		else
			return Qt::AlignHCenter;
	}

	if (role==Qt::ToolTipRole)
	{
		switch(index.column())
		{
		case 0:return list.at(index.row()).name; break;
		case 1:return list.at(index.row()).name; break;
		case 2: return list.at(index.row()).size; break;
		case 3: return list.at(index.row()).lastUpdateDate.toString("dd.MM.yyyy hh:mm:ss"); break;
		default: return QVariant();
		}
		;
	}

	if (role==Qt::ForegroundRole)
	{
		if (list.at(index.row()).lastUpdateDate.date().daysTo(QDate::currentDate())<=3)
			return Qt::blue;
	}
*/
	return QVariant();
}

#include "library.h"

QModelIndex FileListModel::SetPath( QString path )
{
	if (!GetFs()->SetPath(path))
	{
		for (int i = 0; i < Archivers.size(); i++)
			if (IArchiver *arc = dynamic_cast<IArchiver *>(g_Core->QueryModule(Archivers[i]->type, Archivers[i]->interfaceVersion, Archivers[i]->name, Archivers[i]->moduleVersion)))
			{
				if (arc->OpenFile(GetFs()->GetFile(path)))
				{
					fs.push(arc);
					break;
				}
				else
					delete arc;
			}
	}

	bool isRoot = GetFs()->isRoot() == false || fs.size() > 1;
	list.resize(GetFs()->GetNumberOfFiles() + isRoot);

	int i = 0;
	if (isRoot)
	{
		list[i].init();
		list[i].name = "..";
		list[i].path = GetFs()->GetPath() + list[i].name;
		list[i].attributes = FileInfo::Directory;

		list[i].icon = QIcon(":/Icons/Folder.png");
//		Assc->GetIcon(list[i].icon, list[i].path);

		i++;
	}

	if (i < list.size())
		for (bool result = GetFs()->GetFirstFileInfo(list[i]); result; result = GetFs()->GetNextFileInfo(list[++i]))
		{
			if (list[i].attributes & FileInfo::Directory)
				list[i].icon = QIcon(":/Icons/Folder.png");
			else
				Assc->GetIcon(list[i].icon, list[i].path + list[i].name);
			if (i == list.size() - 1)
				break;
		}

	this->path = GetFs()->GetPath();
	this->reset();
	selectedNum = 0;
	
	emit PathChanged();
	return QModelIndex();
}

QModelIndex FileListModel::Enter( QModelIndex selected )
{
	QModelIndex result;
	if (selected.row() != -1)
	{
		if (selected.data().toString() == "..")
			result = UpOneLevel();
		else // if (list[selected.row()].attributes)// & FileInfo::Directory)
		{
			if (path[path.size() - 1] != '/')
				path += "/" + selected.data().toString();
			else
				path += selected.data().toString();
			result = SetPath(path);
		}
	}
	return result;
}

QModelIndex FileListModel::UpOneLevel()
{
	if (GetFs()->isRoot())
	{
		delete fs.pop();
		SetPath(GetFs()->GetPath());
	}
	else
	{
		QString toSelect = path;
		GetFs()->UpOneLevel();
		SetPath(GetFs()->GetPath());
		toSelect.remove(path).remove("/");

		for(int i = 0; i < list.size(); i++)
			if (list[i].name == toSelect)
				return index(i);
	}
	return QModelIndex();
}

QString FileListModel::GetPath()
{
	return path;
}

QVariant FileListModel::headerData( int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const
{
	if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
		switch (section)
		{
			case 0: return tr("Name"); break;
			case 1: return tr("Size"); break;
		}
	return QVariant();
}

Qt::ItemFlags FileListModel::flags( const QModelIndex &index ) const
{
	if (!index.isValid())
		return 0;
	//	if (index.column()==0)
	//		return Qt::ItemIsEditable | Qt::ItemIsEnabled | Qt::ItemIsSelectable;
	return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;
}

int FileListModel::rowCount( const QModelIndex& /*= QModelIndex()*/ ) const
{
	return list.size();
}

int FileListModel::columnCount( const QModelIndex& /*= QModelIndex()*/ ) const
{
	return columns;
}

bool FileListModel::setData( const QModelIndex & /*index*/, const QVariant & /*value*/, int /*role*/ )
{
	return true;
}

const FileInfo *const FileListModel::GetFileInfo( int index )
{
	return (const FileInfo *const)(&list[index]);
}

bool FileListModel::Select( QModelIndex index, int selectAction /*= SA_Toggle*/ )
{
	if (index.isValid() && index.row() < rowCount())
	{
		list[index.row()].selected = selectAction == SA_Select ? true : selectAction == SA_Deselect ? false : !list[index.row()].selected;
		if (list[index.row()].selected)
			selectedNum++;
		else
			selectedNum--;
		return true;
	}
	return false;
}

bool FileListModel::SelectRange( QVector<QModelIndex> &range, int selectAction /*= SA_Toggle*/ )
{
	if (!range.isEmpty())
	{
		for(int i = 0; i < range.size(); i++)
		{
			list[range[i].row()].selected = selectAction == SA_Select ? true : selectAction == SA_Deselect ? false : !list[range[i].row()].selected;
			if (list[range[i].row()].selected)
				selectedNum++;
			else
				selectedNum--;
		}
		return true;
	}
	return false;
}

bool FileListModel::isSelected( QModelIndex index )
{
	if (index.isValid() && index.row() < rowCount())
		return list[index.row()].selected;
	return false;
}

bool archiverLessThan( const Module *first, const Module *second )
{
	if (first->typeDependentInfo == second->typeDependentInfo)
		return first->moduleVersion < second->moduleVersion;
	return first->typeDependentInfo < second->typeDependentInfo;
}

FileListModel::FileListModel()
{
	columns = 2;
	isFocused = false;

	QLinearGradient act(0, 0, 0, 1);
#ifdef DARK
	act.setColorAt(0, QColor(77, 123, 179));
	act.setColorAt(1, QColor(39, 62, 97));
#else
	act.setColorAt(0, QColor(154, 192, 236));
	act.setColorAt(1, QColor(94, 132, 172));
#endif
	act.setCoordinateMode(QGradient::ObjectBoundingMode);
	act.setSpread(QGradient::RepeatSpread);
	selectionActive = QBrush(act);

	QLinearGradient inact(0, 0, 0, 1);
#ifdef DARK
	inact.setColorAt(0, QColor(77, 77, 77));
	inact.setColorAt(1, QColor(39, 39, 39));
#else
	inact.setColorAt(0, QColor(246, 246, 246));
	inact.setColorAt(1, QColor(234, 234, 234));
#endif
	inact.setCoordinateMode(QGradient::ObjectBoundingMode);
	inact.setSpread(QGradient::RepeatSpread);
	selectionInactive = QBrush(inact);

	QLinearGradient sel(0, 0, 0, 1);
#ifdef DARK
	sel.setColorAt(0, QColor(87, 133, 189));
	sel.setColorAt(1, QColor(49, 72, 107));
#else
	sel.setColorAt(0, QColor(233, 184, 152));
	sel.setColorAt(1, QColor(173, 127, 96));
#endif
	sel.setCoordinateMode(QGradient::ObjectBoundingMode);
	sel.setSpread(QGradient::RepeatSpread);
	selectionMarked = QBrush(sel);

	QLinearGradient selInact(0, 0, 0, 1);
#ifdef DARK
	selInact.setColorAt(0, QColor(87, 87, 87));
	selInact.setColorAt(1, QColor(49, 49, 49));
#else
	selInact.setColorAt(0, QColor(245, 207, 181));
	selInact.setColorAt(1, QColor(234, 198, 173));
#endif
	selInact.setCoordinateMode(QGradient::ObjectBoundingMode);
	selInact.setSpread(QGradient::RepeatSpread);
	selectionMarkedInactive = QBrush(selInact);

	if (IFileSystem *fileSystem = dynamic_cast<IFileSystem *>(g_Core->QueryModule("FS", 1)))
		fs.push(fileSystem);
	else
		g_Core->DebugWrite("Panel_Classic", "FileSystem module not found", ICoreFunctions::Error);

	Archivers = g_Core->GetModulesInfo("Archiver", 1);
	if (Archivers.size())
		qSort(Archivers.begin(), Archivers.end(), archiverLessThan);

	if (Assc = dynamic_cast<IAssociations *>(g_Core->QueryModule("Associations", 1)));
	else
		g_Core->DebugWrite("UnitManager_Tabs", "Associations module not found", ICoreFunctions::Error);
}

IFileSystem* FileListModel::GetFs()
{
	return fs.top();
}
