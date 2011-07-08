#include "ColorSelectionWidget.h"
#include <Qsci/qscilexer.h>
#include <QPainter>
#include <QMessageBox>
#include <QSettings>

ColorSelectionWidget::ColorSelectionWidget(QWidget * parent, QsciLexer * lexer, int style) :
	QWidget(parent), lexer_(lexer), style_(style)
{
	color_ = lexer_->color(style_);
	cdlg_ = new QColorDialog(this);
	connect(cdlg_, SIGNAL(colorSelected(QColor const &)), this, SLOT(selectColor(QColor const &)));
}

ColorSelectionWidget::~ColorSelectionWidget()
{
}
	
void ColorSelectionWidget::mousePressEvent(QMouseEvent*)
{
	cdlg_->setCurrentColor(color_);
	cdlg_->show();
}

void ColorSelectionWidget::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.fillRect(this->rect(), QBrush(color_));
	painter.end();
}

void ColorSelectionWidget::save()
{
	if (color_ != lexer_->color(style_))
	{
		lexer_->setColor(color_, style_);	
		QSettings set;
		set.setIniCodec("UTF-8");
		set.beginGroup("ILexerPlugin");
		lexer_->writeSettings(set);
		set.endGroup();
	}
}

void ColorSelectionWidget::selectColor(QColor const & color)
{
	color_ = color;
}