#include "Unit_TextEditor/LexerSettings.h"
#include <QMessageBox>
#include <Qsci/qscilexer.h>
#include <QTreeView>
#include "Unit_TextEditor/ILexerPlugin.h"
#include "Unit_TextEditor/LexersModel.h"

LexerSettings::LexerSettings(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
}

void LexerSettings::showEvent(QShowEvent * event)
{
	ui.lexers->setModel(new LexersModel());
}

LexerSettings::~LexerSettings()
{

}

void LexerSettings::save()
{
	emit updateLexers();
}

