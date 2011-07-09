#include "Unit_TextEditor/EditorSettings.h"
#include <QMessageBox>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include "Unit_TextEditor/SciSettings.h"

EditorSettings::EditorSettings(QWidget *parent, SciSettings * sci) :
    QWidget(parent), sci_(sci)
{
    ui.setupUi(this);
    sci_->setUi(ui);
}

EditorSettings::~EditorSettings()
{
}

void EditorSettings::save()
{
    sci_->getUi(ui);
}

