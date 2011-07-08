#include "SortModel.h"
#include "FileListModel.h"
#ifdef Q_WS_WIN
//#include "windows.h"
#include "shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")
#endif

// #include <iostream>
// using namespace std;

void SortModel::sort( int column, Qt::SortOrder order )
{
	QSortFilterProxyModel::sort(column_ = column, order_ = order);
// 	freopen("output.txt", "w", stdout);
}

bool SortModel::lessThan(const QModelIndex &upIndex, const QModelIndex &downIndex) const
{
	static int times = 0;
	times++;
//	cout << times << ", " << upIndex.row() << "x" << downIndex.row() << " " << column_ << endl;
	FileListModel *src = dynamic_cast<FileListModel *>(sourceModel());
	if (!src)
		return 0;

	const FileInfo *const up = src->GetFileInfo(upIndex.row()),
		*const down = src->GetFileInfo(downIndex.row());

        if (up->name == ".." || (up->attributes & FileInfo::Directory && !(down->attributes & FileInfo::Directory)))
		return !order_;
        else if (down->name == ".." || (down->attributes & FileInfo::Directory && !(up->attributes & FileInfo::Directory)))
		return order_;

	switch (column_)
	{
	case 0:
		break;
	case 1:
		if (up->attributes & FileInfo::Directory && down->attributes & FileInfo::Directory)
			break;
		return up->size < down->size;
		break;
	}

#ifdef Q_WS_WIN
 	return StrCmpLogicalW((WCHAR *)up->name.constData(), (WCHAR *)down->name.constData()) < 0;
#else
	return QString::compare(up->name, down->name, Qt::CaseSensitive) < 0; // should find better alternative to StrCmpLogicalW someday
#endif
}
