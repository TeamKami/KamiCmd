#include "Unit_TextEditor/LexersDialog.h"
#include <QMessageBox>
#include <Qsci/qscilexer.h>
#include "Unit_TextEditor/ILexerPlugin.h"

LexersDialog::LexersDialog(QWidget *parent) :
    QMainWindow(parent)
{
    ui.setupUi(this);
    setWindowFlags(Qt::Dialog);
}

LexersDialog::~LexersDialog()
{

}

void LexersDialog::show(QVector<ILexer*> const & lexers, QsciLexer * cLexer)
{
    int active = 0, id = 1;

    ui.lexers->addItem("(None)", QVariant::fromValue(static_cast<void*> (0)));

    foreach(ILexer * lexer, lexers)
    {
        if (lexer->getLexer() == cLexer)
        {
            active = id;
        }

        ui.lexers->addItem(QString(lexer->getLexer()->language()),
                QVariant::fromValue(static_cast<void*>(lexer->getLexer())));

        id++;
    }

    ui.lexers->setCurrentIndex(active);

    QMainWindow::show();
}

void LexersDialog::on_actionSelect_triggered()
{
    emit setLexer(static_cast<QsciLexer*> (ui.lexers->itemData(
            ui.lexers->currentIndex()).value<void*> ()));
}
