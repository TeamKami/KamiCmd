#ifndef QUICKSEARCHWIDGET_H
#define QUICKSEARCHWIDGET_H

#include <QWidget>
#include "ui_QuickSearchWidget.h"

class QuickSearchWidget : public QWidget
{
	Q_OBJECT

public:
	QuickSearchWidget(QWidget *parent = 0);
	~QuickSearchWidget();

private:
	Ui::QuickSearchWidget ui;
};

#endif // QUICKSEARCHWIDGET_H
