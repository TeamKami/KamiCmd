#ifndef FILELISTHEADER_H
#define FILELISTHEADER_H

#include <QHeaderView>

class FileListHeader : public QHeaderView
{
	Q_OBJECT

public:
	FileListHeader(Qt::Orientation orientation, QWidget *parent = NULL);
	~FileListHeader();

	virtual QSize sizeHint() const;

public slots:
	void sectionDoubleClickedSlot(int logicalIndex);
};

#endif // FILELISTHEADER_H
