#include "LexerSettings.h"
#include <QMessageBox>
#include <Qsci/qscilexer.h>
#include <QTableWidget>
#include <QLabel>
#include <QFontComboBox>
#include <QTreeWidget>
#include "ColorSelectionWidget.h"
#include "FontSelectionWidget.h"
#include <QThread>


std::map<QString, QsciLexer*> & lexers();

LexerSettings::LexerSettings(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void LexerSettings::showEvent(QShowEvent * event)
{
	static bool first = true;
	if (!first)
		return;
	first = false;

	for (std::map<QString, QsciLexer*>::iterator it = lexers().begin(); it != lexers().end(); ++it)
	{
		QTreeWidgetItem * lexer = new QTreeWidgetItem(ui.styles);
		ui.styles->addTopLevelItem(lexer);
		lexer->setText(0, it->second->language());

		QsciLexer * lexer_ = it->second;
	
		for (int i = 0; lexer_->description(i) != ""; ++i)
		{
			QTreeWidgetItem * it = new QTreeWidgetItem(lexer);
			lexer->addChild(it);
			it->setText(0, lexer_->description(i));
			QTreeWidgetItem * ci = new QTreeWidgetItem(it);
			it->addChild(ci);
			ci->setText(0, "Font");	
			FontSelectionWidget * fw = new FontSelectionWidget(ui.styles, lexer_, i);
			ui.styles->setItemWidget(ci, 1, fw);
			ci = new QTreeWidgetItem(it);
			it->addChild(ci);
			ci->setText(0, "Color");
			ColorSelectionWidget * cw = new ColorSelectionWidget(ui.styles, lexer_, i);
			ui.styles->setItemWidget(ci, 1, cw);
			connect(this, SIGNAL(updateLexers()), fw, SLOT(save()));
			connect(this, SIGNAL(updateLexers()), cw, SLOT(save()));
		}
	}
}

LexerSettings::~LexerSettings()
{

}

void LexerSettings::save()
{
	emit updateLexers();
}

