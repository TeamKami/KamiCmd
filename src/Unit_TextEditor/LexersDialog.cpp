#include "LexersDialog.h"
#include <QMessageBox>
#include <Qsci/qscilexer.h>

LexersDialog::LexersDialog(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog);
}

LexersDialog::~LexersDialog()
{

}

void LexersDialog::show(std::map<QString, QsciLexer *> const & lexers, QsciLexer * lexer)
{
	int active = 0, id = 1;

	ui.lexers->addItem("(None)", QVariant::fromValue(static_cast<void*>(0)));
	
	for (std::map<QString, QsciLexer *>::const_iterator it = lexers.begin(); it != lexers.end(); ++it)
	{
		//ui.lexers->addItem(it->first, it->second);
		if (it->second == lexer)
		{
			active = id;
		}

		ui.lexers->addItem(QString(it->second->language()), 
			QVariant::fromValue(static_cast<void*>(it->second)));

		id++;
	}
	
	ui.lexers->setCurrentIndex(active);

	QMainWindow::show();
}

void LexersDialog::on_actionSelect_triggered()
{
	emit setLexer(static_cast<QsciLexer*>(ui.lexers->itemData(ui.lexers->currentIndex()).value<void*>()));
}
