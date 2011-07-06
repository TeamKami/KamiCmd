#include "SettingsDialog.h"
#include <QMessageBox>
#include <Qsci/qscilexer.h>
#include "LexerSettings.h"
#include "EditorSettings.h"

SettingsDialog::SettingsDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog);
}

SettingsDialog::~SettingsDialog()
{

}

void SettingsDialog::show(QsciScintilla * sci)
{
	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(onDialogButton(QAbstractButton*)));
	connect(ui.navigation, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this, SLOT(onPageSelected(QTreeWidgetItem*, int)));

	QTreeWidgetItem * appearance = new QTreeWidgetItem();
	appearance->setText(0, "Appearance");
	QTreeWidgetItem * lexer = new QTreeWidgetItem(appearance);
	appearance->addChild(lexer);
	lexer->setText(0, "Coloring");
	LexerSettings * ls = new LexerSettings(this);
	lexer->setData(0, Qt::UserRole, QVariant::fromValue(static_cast<void*>(dynamic_cast<QWidget*>(ls))));	
	connect(this, SIGNAL(save()), ls, SLOT(save()));
	ls->hide();
	QTreeWidgetItem * editor = new QTreeWidgetItem(appearance);
	appearance->addChild(editor);
	editor->setText(0, "Editor");
	EditorSettings * es = new EditorSettings(this, sci);
	editor->setData(0, Qt::UserRole, QVariant::fromValue(static_cast<void*>(dynamic_cast<QWidget*>(es))));	
	connect(this, SIGNAL(save()), es, SLOT(save()));
	es->hide();

	ui.navigation->addTopLevelItem(appearance);

	QDialog::show();
}

void SettingsDialog::onDialogButton(QAbstractButton * button)
{
	if (ui.buttonBox->standardButton(button) == QDialogButtonBox::Ok
		|| ui.buttonBox->standardButton(button) == QDialogButtonBox::Apply)
	{
		emit saveSettings();
	}
}

void SettingsDialog::onPageSelected(QTreeWidgetItem * item, int column)
{
	QWidget * page = static_cast<QWidget*>(item->data(0, Qt::UserRole).value<void*>());
	
	ui.settings->takeWidget();
	ui.settings->setWidget(page);
	page->show();
	ui.settings->show();
}

