#include "Unit_PictureView.h"
#include "library.h"

Unit_PictureView::Unit_PictureView(QWidget *parent)
	: IUnit(parent)
{
	layout = new QHBoxLayout(this);
	layout->setMargin(0);
	layout->setSpacing(0);
	viewer = new PictureView(this);
	layout->addWidget(viewer);
	setFocusProxy(viewer);

	connect(viewer, SIGNAL(ViewPrevImage()), SLOT(ViewPrevImage()));
	connect(viewer, SIGNAL(ViewNextImage()), SLOT(ViewNextImage()));
}

QString Unit_PictureView::GetText()
{
	return text;
}

void Unit_PictureView::SaveState( QSettings &/*set*/ )
{
}

void Unit_PictureView::LoadState( QSettings &/*set*/ )
{
}

void Unit_PictureView::Create( IUnit *createdFrom )
{
	if (hostPanel = dynamic_cast<IPanel *>(createdFrom))
	{
		const FileInfo *const info = hostPanel->GetCurrentFile();
		text = info->name;
		path = info->path;
		emit TextChanged();
		viewer->OpenFile(info->path + info->name);
	}
}

QIcon Unit_PictureView::GetIcon()
{
	return QIcon();
}

bool Unit_PictureView::ViewPrevImage()
{
	if (const FileInfo *const info = hostPanel->SetCurrentFileToPrev())
	{
		text = info->name;
		path = info->path;
		emit TextChanged();
		viewer->OpenFile(info->path + info->name);
		return true;
	}
	return false;
}

bool Unit_PictureView::ViewNextImage()
{
	if (const FileInfo *const info = hostPanel->SetCurrentFileToNext())
	{
		text = info->name;
		path = info->path;
		emit TextChanged();
		viewer->OpenFile(info->path + info->name);
		return true;
	}
	return false;
}




void PictureView::paintEvent( QPaintEvent * /*event*/ )
{
	QPainter painter(this);
	//painter.drawText(10, 10, "Hello word!");
	QPixmap scaled = img.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	painter.drawPixmap((width() - scaled.width()) / 2, (height() - scaled.height()) / 2, scaled);
}

PictureView::PictureView(QWidget *parent)
: QWidget(parent)
{
	file = NULL;
	setFocusPolicy(Qt::StrongFocus);
	setBackgroundRole(QPalette::Base);
	setAutoFillBackground(true);
	setMouseTracking(true);
}

PictureView::~PictureView()
{
	if (file)
		file->close();
}


void PictureView::OpenFile( QString path )
{
	file = new QFile(path, this);
	file->open(QIODevice::ReadOnly);

	uchar *txt;
	if (txt = file->map(0, file->size()))
	{
		img.loadFromData(txt, file->size());
		update();
	}
}

void PictureView::keyPressEvent( QKeyEvent *event )
{
	switch (event->key())
	{
	case Qt::Key_Right:
	case Qt::Key_PageDown:
	case Qt::Key_Space:
		emit ViewNextImage();
		break;

	case Qt::Key_Left:
	case Qt::Key_PageUp:
	case Qt::Key_Backspace:
		emit ViewPrevImage();
		break;
	}
}


void PictureView::wheelEvent( QWheelEvent *event )
{
	int numDegrees = event->delta() / 8;
	int numSteps = numDegrees / 15;

	for (int i = 0; i < abs(numSteps); i++)
		if (numSteps < 0)
			emit ViewNextImage();
		else
			emit ViewPrevImage();
	update();
}