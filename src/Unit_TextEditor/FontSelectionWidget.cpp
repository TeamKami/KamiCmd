#include "FontSelectionWidget.h"
#include <Qsci/qscilexer.h>
#include <QSettings>

FontSelectionWidget::FontSelectionWidget(QWidget * parent, QsciLexer * lexer, int style) :
	QFontComboBox(parent), lexer_(lexer), style_(style)
{
	setCurrentFont(lexer_->font(style_));
}

FontSelectionWidget::~FontSelectionWidget()
{
}
	
void FontSelectionWidget::save()
{
	if (lexer_->font(style_) != currentFont())
	{
		lexer_->setFont(currentFont(), style_);	
		QSettings set;
		set.setIniCodec("UTF-8");
		set.beginGroup("ILexerPlugin");
		lexer_->writeSettings(set);
		set.endGroup();
	}
}