#ifndef UNIT_PICTUREVIEW_H
#define UNIT_PICTUREVIEW_H

#include <QtGui>
#include "../IUnitManager.h"
#include "../IPanel.h"
#include <QTextEdit>
#include <QScrollBar>

class PictureView : public QWidget
{
	Q_OBJECT

public:
	PictureView(QWidget *parent);
	~PictureView();
	void OpenFile(QString path);

private:
	virtual void paintEvent(QPaintEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void wheelEvent(QWheelEvent *event);

	QFile *file;
	QPixmap img;

signals: 
	bool ViewPrevImage();
	bool ViewNextImage();

	public slots:
};

class Unit_PictureView : public IUnit
{
	Q_OBJECT

public:
	Unit_PictureView(QWidget *parent);
	virtual void Create(IUnit *createdFrom);
	virtual void Link(IUnit * /*withUnit*/) {};
	virtual QString GetText();
	virtual QIcon GetIcon();

	virtual void SaveState(QSettings &set);
	virtual void LoadState(QSettings &set);

private:
	QHBoxLayout *layout;
	PictureView *viewer;
	QString text;
	QString path;

	IPanel *hostPanel;

signals:
	void TextChanged();

public slots:
	bool ViewPrevImage();
	bool ViewNextImage();
};

#endif // UNIT_PICTUREVIEW_H
