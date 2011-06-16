#include "AssociationsDialog_Edit.h"
#include "library.h"
#include <QColorDialog>
#include <QFileDialog>

void AssociationsDialog_Edit::SyncButtonsColors()
{
	if (Assc->textColor.isValid())
	{
		QPalette palette = ui.textColorButton->palette();
		palette.setColor(QPalette::Button, Assc->textColor);
		ui.textColorButton->setPalette(palette);
	}
	if (Assc->backColor.isValid())
	{
		QPalette palette = ui.backColorButton->palette();
		palette.setColor(QPalette::Button, Assc->backColor);
		ui.backColorButton->setPalette(palette);
	}
}

AssociationsDialog_Edit::AssociationsDialog_Edit( QWidget *parent /*= 0*/, Association *assc, QVector<QAction *> *actions )
: QDialog(parent)
{
	ui.setupUi(this);
	connect(ui.buttonBox, SIGNAL(accepted()), SLOT(accept()));
	connect(ui.buttonBox, SIGNAL(rejected()), SLOT(reject()));
	Assc = assc;
	Actions = actions;

	ui.descriptionEdit->setText(Assc->description);
	ui.maskEdit->setText(Assc->mask);
	ui.iconEdit->setText(Assc->iconPath);
	ui.textColorEdit->setText(Associations::QStringFromQColor(Assc->textColor));
	ui.backColorEdit->setText(Associations::QStringFromQColor(Assc->backColor));
	SyncButtonsColors();

	for (int i = 0; i < Actions->size(); i++)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem(ui.actionsTree);
		item->setText(0, Actions->at(i)->text());

		if (i < Assc->actions.size())
			item->setText(1, Assc->actions.at(i));
		else
			item->setText(1, "");
		item->setFlags(item->flags() | Qt::ItemIsEditable);
	}
}

AssociationsDialog_Edit::~AssociationsDialog_Edit()
{

}

void AssociationsDialog_Edit::accept()
{
	Assc->description = ui.descriptionEdit->text();
	Assc->mask = ui.maskEdit->text();
	Assc->iconPath = ui.iconEdit->text();
	Assc->textColor = Associations::QColorFromQString(ui.textColorEdit->text());
	Assc->backColor = Associations::QColorFromQString(ui.backColorEdit->text());
	Assc->icon = QIcon(Assc->iconPath);
	
	int i = 0;
	for (QTreeWidgetItemIterator it(ui.actionsTree); *it; ++it)
		Assc->actions[i++] = (*it)->text(1);
	QDialog::accept();
}

void AssociationsDialog_Edit::reject()
{
	QDialog::reject();
}

void AssociationsDialog_Edit::on_iconEdit_textChanged(const QString &)
{
	QIcon icon(ui.iconEdit->text());
	if (!icon.isNull())
		ui.iconButton->setText("");
	else
		ui.iconButton->setText(tr("Icon..."));
	ui.iconButton->setIcon(icon);
}

void AssociationsDialog_Edit::on_textColorButton_clicked()
{
	QColor color = QColorDialog::getColor(Assc->textColor, this, tr("Pick a color"), QColorDialog::ShowAlphaChannel);
	if (color.isValid())
	{
		Assc->textColor = color;
		ui.textColorEdit->setText(Associations::QStringFromQColor(color));
		SyncButtonsColors();
	}
}

void AssociationsDialog_Edit::on_textColorEdit_textChanged(const QString &str)
{
	QColor color = Associations::QColorFromQString(str);
	if (color.isValid())
	{
		Assc->textColor = color;
		SyncButtonsColors();
	}
}

void AssociationsDialog_Edit::on_backColorEdit_textChanged(const QString &str)
{
	QColor color = Associations::QColorFromQString(str);
	if (color.isValid())
	{
		Assc->backColor = color;
		SyncButtonsColors();
	}
}

void AssociationsDialog_Edit::on_backColorButton_clicked()
{
	QColor color = QColorDialog::getColor(Assc->backColor, this, tr("Pick a color"), QColorDialog::ShowAlphaChannel);
	if (color.isValid())
	{
		Assc->backColor = color;
		ui.backColorEdit->setText(Associations::QStringFromQColor(color));
		SyncButtonsColors();
	}
}

void AssociationsDialog_Edit::on_iconButton_clicked()
{
	QString str = QFileDialog::getOpenFileName(this, tr("Open Icon"), tr("/"),
		tr("Image Files (*.png *.jpg *.bmp *.gif *.svg *.jpeg *.pbm *.pgm *.ppm *.tiff *.tif *.xbm *.xpm)"));
	if (!str.isEmpty())
		ui.iconEdit->setText(str);
}