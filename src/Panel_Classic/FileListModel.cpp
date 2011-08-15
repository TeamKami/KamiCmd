#include "FileListModel.h"
#include <QLocale>
#include <QColor>
#include "library.h"
#include <QMimeData>
#include <QUrl>
#include <QMessageBox>


bool archiverLessThan( const Module *first, const Module *second )
{
	if (first->typeDependentInfo == second->typeDependentInfo)
		return first->moduleVersion < second->moduleVersion;
	return first->typeDependentInfo < second->typeDependentInfo;
}

FileListModel::FileListModel(QObject *parent /*= NULL*/)
	: QAbstractItemModel(parent)
{
	columns = 2;

	if (IFileSystem *fileSystem = dynamic_cast<IFileSystem *>(g_Core->QueryModule("FS", 1)))
		fs.push(fileSystem);
	else
		g_Core->DebugWrite("Panel_Classic", "FileSystem module not found", ICoreFunctions::Error);

	Archivers = g_Core->GetModulesInfo("Archiver", 1);
	if (Archivers.size())
		qSort(Archivers.begin(), Archivers.end(), archiverLessThan);

	if ((Assc = dynamic_cast<IAssociations *>(g_Core->QueryModule("Associations", 1))) == 0)
		g_Core->DebugWrite("UnitManager_Tabs", "Associations module not found", ICoreFunctions::Error);
}


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
	if (column > columnCount() || row > FileInfoArr.count() || FileInfoArr.isEmpty())
		return QModelIndex();
	return createIndex(row, column);
}

QModelIndex FileListModel::parent( const QModelIndex& ) const
{
	return QModelIndex();
}

QVariant FileListModel::data( const QModelIndex & index, int role /*= Qt::DisplayRole */ ) const
{
	if (!index.isValid() || index.row() > FileInfoArr.count() || FileInfoArr.isEmpty())
		return QVariant();

	const FileInfo &current = FileInfoArr[index.row()];

	switch (role)
	{
	// User roles
	case AttributesRole:
		return current.attributes;
		break;

	case FileInfoRole:
		{
			QVariant var;
			var.setValue(current);
			return var;
		}
		break;

	case IndexRowRole:
		return index.row();
	
	// Qt roles
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

	case Qt::DecorationRole:
		if (!index.column())
			return current.icon;
		break;

	case Qt::ForegroundRole:
		{
			QVariant res;

			if (current.attributes & FileInfo::Directory)
				res = QColor(2, 88, 112);
			else
			{
				QColor color = Assc->GetTextColor(current.path + current.name);
				res = color.isValid() ? color :
					QVariant();
			}

			// If enabled graying out of non-matched with quicksearch items:
			/*if (!quickSearch.isEmpty() && index.data(Qt::DisplayRole).toString().indexOf(quickSearch, 0, Qt::CaseInsensitive) == -1)
			{
				QColor resAdj = res.value<QColor>();
				resAdj.setHsl(resAdj.hslHue(), resAdj.hslSaturation() / 3, 180);
				//resAdj.setHsv(resAdj.hue(), resAdj.saturation(), 200);
				res = resAdj;
			}*/
			return res;
		}
		break;

	case Qt::BackgroundRole:
		{
			QColor color = Assc->GetBackColor(current.path + current.name);
			return color.isValid() ? color : QVariant();
		}
		break;

	case Qt::TextAlignmentRole:
		if (index.column() == 1)
			return int(Qt::AlignRight | Qt::AlignVCenter);
		break;
	}

	return QVariant();
}

bool FileListModel::SetPath( QString path )
{
	bool isSuccess = false;
	if (GetFs()->SetPath(path))
		isSuccess = true;
	else
	{
		for (int i = 0; i < Archivers.size(); i++)
			if (IArchiver *arc = dynamic_cast<IArchiver *>(g_Core->QueryModule(Archivers[i]->type, Archivers[i]->interfaceVersion, Archivers[i]->name, Archivers[i]->moduleVersion)))
			{
				if (arc->OpenFile(GetFs()->GetFile(path)))
				{
					fs.push(arc);
					isSuccess = true;
					break;
				}
				else
					delete arc;
			}
	}

	bool isRoot = GetFs()->isRoot() == false || fs.size() > 1;
	FileInfoArr.resize(GetFs()->GetNumberOfFiles() + isRoot);

	int i = 0;
	if (isRoot)
	{
		FileInfoArr[i].init();
		FileInfoArr[i].name = "..";
		FileInfoArr[i].path = GetFs()->GetPath() + FileInfoArr[i].name;
		FileInfoArr[i].attributes = FileInfo::Directory;

		FileInfoArr[i].icon = QIcon(":/Icons/Folder.png");
//		Assc->GetIcon(list[i].icon, list[i].path);

		i++;
	}

	if (i < FileInfoArr.size())
		for (bool result = GetFs()->GetFirstFileInfo(FileInfoArr[i]); result; result = GetFs()->GetNextFileInfo(FileInfoArr[++i]))
		{
			if (FileInfoArr[i].attributes & FileInfo::Directory)
				FileInfoArr[i].icon = QIcon(":/Icons/Folder.png");
			else
				Assc->GetIcon(FileInfoArr[i].icon, FileInfoArr[i].path + FileInfoArr[i].name);
			if (i == FileInfoArr.size() - 1)
				break;
		}

	quickSearch = "";
	this->path = GetFs()->GetPath();
	this->reset();
	
	emit PathChanged();
	return isSuccess;
}

QModelIndex FileListModel::Enter( QModelIndex selected )
{
	QModelIndex result;
	if (selected.row() != -1)
	{
		if (selected.data(Qt::DisplayRole).toString() == "..")
			result = UpOneLevel();
		else // if (list[selected.row()].attributes)// & FileInfo::Directory)
		{
			if (path[path.size() - 1] != '/')
				path += "/" + selected.data(Qt::DisplayRole).toString();
			else
				path += selected.data(Qt::DisplayRole).toString();
			SetPath(path);
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

		for(int i = 0; i < FileInfoArr.size(); i++)
			if (FileInfoArr[i].name == toSelect)
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
	Qt::ItemFlags res = Qt::ItemIsDropEnabled;

	if (!index.isValid())
		return res;

	res |= Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsDragEnabled;

	// If enabled graying out of non-matched with quicksearch items:
	//if (quickSearch.isEmpty() || index.data(Qt::DisplayRole).toString().indexOf(quickSearch, 0, Qt::CaseInsensitive) != -1)
	res |= Qt::ItemIsEnabled;

	//res |= Qt::ItemIsUserCheckable;
	return res;
}

int FileListModel::rowCount( const QModelIndex& /*= QModelIndex()*/ ) const
{
	return FileInfoArr.size();
}

int FileListModel::columnCount( const QModelIndex &/*parent*/ /*= QModelIndex()*/ ) const
{
	return columns;
}

bool FileListModel::setData( const QModelIndex & /*index*/, const QVariant & /*value*/, int /*role*/ )
{
	return true;
}

const FileInfo * FileListModel::GetFileInfo( int index )
{
	return (const FileInfo *const)(&FileInfoArr[index]);
}


IFileSystem* FileListModel::GetFs()
{
	return fs.top();
}

void FileListModel::QuickSearchChanged( QString search )
{
	quickSearch = search;
}

Qt::DropActions FileListModel::supportedDropActions() const
{
	return Qt::CopyAction | Qt::MoveAction | Qt::LinkAction | Qt::IgnoreAction | Qt::TargetMoveAction;
}

bool FileListModel::dropMimeData( const QMimeData *mimeData, Qt::DropAction action, int /*row*/, int column, const QModelIndex &/*parent*/ )
{
// 	if (action == Qt::IgnoreAction)
// 		return true;

	if (mimeData->hasUrls())
	{
		QList<QUrl> urlList = mimeData->urls();
		QString str;
		for (int i = 0; i < urlList.size(); i++)
			str.append(urlList[i].toString() + "\n");

		QString actionStr;
		switch (action)
		{
		case Qt::CopyAction: actionStr = "copy"; break;
		case Qt::MoveAction: actionStr = "move"; break;
		case Qt::LinkAction: actionStr = "link"; break;
		case Qt::TargetMoveAction: actionStr = "target-move"; break;
		case Qt::IgnoreAction: actionStr = "ignore"; break;
		}
		QMessageBox::information(NULL, QString("Files to %1").arg(actionStr), str);
		return true;
	}
	else
	{
		QString str;
		foreach (QString format, mimeData->formats())
			str.append(format + " ");
		QMessageBox::information(NULL, "Unknown Drag&Drop MIMEs:", str);
		return false;
	}

	if (column > 0)
		return false;
}

QMimeData * FileListModel::mimeData( const QModelIndexList &indexes ) const
{
	QMimeData *mimeData = new QMimeData();
	QList<QUrl> urls;

	foreach (const QModelIndex &index, indexes)
		if (index.isValid() && !index.column())
			urls << QUrl::fromLocalFile(data(index, FileListModel::FileInfoRole).value<FileInfo>().FilePath()).toString();

	mimeData->setUrls(urls);
	return mimeData;
}

QStringList FileListModel::mimeTypes() const
{
	QStringList types;
	types << "application/kamicmd.file.list"
		<< "text/uri-list";
	return types;
}

bool FileListModel::removeRows(int row, int count, const QModelIndex &parent /*= QModelIndex()*/)
{
	int beginRow = qMax(0, row);
	int endRow = qMin(row + count - 1, rowCount() - 1);
	
	beginRemoveRows(parent, beginRow, endRow);
	FileInfoArr.remove(row, count);
	endRemoveRows();
	return true;
}

bool FileListModel::insertRows( int row, int count, const QModelIndex &parent /*= QModelIndex()*/ )
{
	int beginRow = qMax(0, row);
	int endRow = qMin(row + count - 1, rowCount() - 1);

	beginInsertRows(parent, beginRow, endRow);
	FileInfo info;
	FileInfoArr.insert(row, count, info);
	endInsertRows();

	return true;
}