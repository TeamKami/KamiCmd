#include "FontSelectionWidget.h"
#include <Qsci/qscilexer.h>

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
	lexer_->setFont(currentFont(), style_);	
}