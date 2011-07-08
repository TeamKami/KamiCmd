#ifndef LEXERSDELEGATE_H
#define LEXERSDELEGATE_H

#include <QStyledItemDelegate>

class QColorDialog;
class LexersModelNode;

class LexersDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	LexersDelegate(QObject * parent = 0);
	~LexersDelegate();
	virtual void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	virtual QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
	virtual QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    virtual void setEditorData(QWidget *editor, const QModelIndex &index) const;
    virtual void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    virtual void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	virtual bool editorEvent(QEvent * event, QAbstractItemModel * model, const QStyleOptionViewItem & option, const QModelIndex & index);

private:
	QColorDialog * cdlg;
	LexersModelNode * cache;
private slots:
	void selectColor(QColor const & color);
};

#endif //LEXERSDELEGATE_H