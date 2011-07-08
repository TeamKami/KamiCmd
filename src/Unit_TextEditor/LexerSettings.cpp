#include "Unit_TextEditor/LexerSettings.h"
#include <QMessageBox>
#include <Qsci/qscilexer.h>
#include <QTreeView>
#include "Unit_TextEditor/ILexerPlugin.h"
#include "Unit_TextEditor/LexersModel.h"
#include "Unit_TextEditor/LexersDelegate.h"

LexerSettings::LexerSettings(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void LexerSettings::showEvent(QShowEvent *)
{
	ui.lexers->setEditTriggers(QAbstractItemView::DoubleClicked
                                 | QAbstractItemView::SelectedClicked);
	ui.lexers->setSelectionBehavior(QAbstractItemView::SelectRows);

	LexersModel * model = new LexersModel(this);
	connect(this, SIGNAL(updateLexers()), model, SLOT(save()));
	ui.lexers->setModel(model);
	ui.lexers->setItemDelegate(new LexersDelegate(this));
	
}

LexerSettings::~LexerSettings()
{

}

void LexerSettings::save()
{
	emit updateLexers();
}

