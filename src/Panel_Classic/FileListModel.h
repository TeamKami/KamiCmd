#ifndef PANEL_LISTMODEL_H
#define PANEL_LISTMODEL_H

#include "../IFileSystem.h"
#include "../IArchiver.h"
#include "../ILibrary.h"
#include "../IAssociations.h"

//#include <QIcon>
#include <QVector>
#include <QStack>
#include <QVarLengthArray>
#include <QAbstractItemModel>
#include <QIcon>
#include <QLinearGradient>

class FileListModel : public QAbstractItemModel
{
	Q_OBJECT

public:

	enum DataUserRoles
	{
		AttributesRole = Qt::UserRole,
		FileInfoRole = Qt::UserRole + 1
	};
	FileListModel(QObject *parent = NULL);

	virtual QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const;
	virtual QModelIndex parent(const QModelIndex&) const;
	virtual int rowCount(const QModelIndex& = QModelIndex()) const;
	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	virtual bool hasChildren(const QModelIndex &parent) const { return parent == QModelIndex();	} // This line is a fucking crutch that took me the whole evening to realize why the hell sorting was done x^2 times longer than anticipated
	virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role);
	virtual Qt::DropActions supportedDropActions() const;
	virtual bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
	virtual QMimeData *mimeData(const QModelIndexList &indexes) const;
	virtual QStringList	mimeTypes() const;
	virtual bool insertRows(int row, int count,	const QModelIndex &parent = QModelIndex());
	virtual bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

	bool SetPath(QString path);
	QString GetPath();
	QModelIndex Enter(QModelIndex selected); // Return value is index of element for selection
	QModelIndex UpOneLevel(); // Return value is index of element for selection
	const FileInfo * GetFileInfo(int index);
	
	IFileSystem* GetFs();

signals:
	void PathChanged();

public slots:
	void QuickSearchChanged(QString search);

private:
	QStack<IFileSystem *>fs;
	QVector<const Module *> Archivers;
	QVector<FileInfo> FileInfoArr;
	QString path;
	int columns;

	IAssociations *Assc;

	QString quickSearch;
};

#endif // PANEL_LISTMODEL_H
