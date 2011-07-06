#ifndef COLORSELECTIONWIDGET_H
#define COLORSELECTIONWIDGET_H

#include <QWidget>
#include <QColorDialog>
class QsciLexer;


class ColorSelectionWidget : public QWidget
{
	Q_OBJECT
public:
	ColorSelectionWidget(QWidget * parent, QsciLexer * lexer, int style);
	~ColorSelectionWidget();
	
protected:
	void paintEvent(QPaintEvent *event);
	void mousePressEvent(QMouseEvent*);
public slots:
	void save();
	void selectColor(QColor const & color);

private:
	QsciLexer * lexer_;
	int style_;
	QColor color_;
	QColorDialog * cdlg_;
};

#endif //COLORSELECTIONWIDGET_H