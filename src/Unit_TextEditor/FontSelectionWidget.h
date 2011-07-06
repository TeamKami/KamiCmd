#ifndef FONTSELECTIONWIDGET_H
#define FONTSELECTIONWIDGET_H

#include <QWidget>
#include <QFontComboBox>
class QsciLexer;


class FontSelectionWidget : public QFontComboBox
{
	Q_OBJECT
public:
	FontSelectionWidget(QWidget * parent, QsciLexer * lexer, int style);
	~FontSelectionWidget();
	
public slots:
	void save();

private:
	QsciLexer * lexer_;
	int style_;
};

#endif //COLORSELECTIONWIDGET_H