#ifndef LEXERSMODEL_H
#define LEXERSMODEL_H

#include <QAbstractItemModel>
#include <QTreeView>
#include <QSettings>
class ILexer;

enum LexersModelLeafType
{
    Font, Color,
    //Wildcard,
    Properties,
    Lexer,
    Style
};

/**
 * \headerfile Unit_TextEditor/LexersModel.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Cached settings. Current settings will be saved when OK or Apply pressed and dropped when Cancel pressed/
 */
class LexersModelCacheItem: public QObject
{
Q_OBJECT
public:
    LexersModelCacheItem(ILexer * lexer = 0, int style = 0, QObject * parent =
            0);
    void save(QSettings & set) const;
    void reset();
    QFont & font();
    QColor & color();
private:
    ILexer * lexer_;
    int style_;
    QFont font_;
    QColor color_;
};

/**
 * \headerfile Unit_TextEditor/LexersModel.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Class, describing node of LexersModel. Used only in LexersModel and LexersDelegate.
 */
class LexersModelNode: public QObject
{
    Q_OBJECT
public:
    /**
     * \param lexer Index of lexer in lexers() array
     * \param style Style id or -1 if type is Lexer
     * \param type Type of this node
     * \param cache Pointer to cached settings
     */
    LexersModelNode(int lexer, int style, LexersModelLeafType type,
            LexersModelCacheItem * cache = 0, QObject * parent = 0);

    /**
     * \brief return lexer id
     */
    int lexer() const;

    /**
     * \brief return style id
     */
    int style() const;

    /**
     * \brief return node type
     */
    LexersModelLeafType type() const;

    /**
     * \brief return reference to current font
     */
    QFont & font() const;

    /**
     * \brief return reference to current color
     */
    QColor & color() const;

private:
    int lexer_, style_;
    LexersModelLeafType type_;
    LexersModelCacheItem * cache_;
};

/**
 * \headerfile Unit_TextEditor/LexersModel.h
 * \author Nikolay Filchenko <finomen812@gmail.com>
 * \brief Model used to display lexers settings
 */
class LexersModel: public QAbstractItemModel
{
    Q_OBJECT
public:
    LexersModel(QObject * parent = 0);
    QModelIndex index(int row, int column, const QModelIndex & parent =
            QModelIndex()) const;
    QModelIndex parent(const QModelIndex & index) const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role =
            Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex & index) const;

public slots:
void save();
private:
    mutable QMap<QString, LexersModelNode*> nodes;
    mutable QMap<QString, LexersModelCacheItem*> cache_;
    LexersModelNode
            * createNode(int lexer, int style, LexersModelLeafType type) const;
};

#endif //LEXERSMODEL_H
