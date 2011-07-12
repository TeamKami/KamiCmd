#include "Unit_TextEditor/SciSettings.h"
#include "Unit_TextEditor/SettingsDialog.h"
#include "UnitManager_Tabs/ActionManager.h" 
#include <Qsci/qsciscintilla.h>
#include <QSettings>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QAction>
#include <QMessageBox>
#include "ILibrary.h"

template<typename T>
struct SettingCast
{
    typedef int realType;

    static realType exec(T val)
    {
        return static_cast<int> (val);
    }

    static T exec(int val)
    {
        return static_cast<T> (val);
    }
};

template<>
struct SettingCast<int>
{
    typedef int realType;

    static int exec(int val)
    {
        return val;
    }
};

template<>
struct SettingCast<bool>
{
    typedef bool realType;

    static bool exec(bool val)
    {
        return val;
    }
};

template<typename T>
struct UiSetting
{
    typedef QComboBox* Ui::EditorSettings::*widgetPtr;

    static T get(Ui::EditorSettings & ui, widgetPtr w)
    {
        return static_cast<T> (((ui).*w)->currentIndex());
    }

    static void set(Ui::EditorSettings & ui, widgetPtr w, T value)
    {
        ((ui).*w)->setCurrentIndex(value);
    }
};

template<>
struct UiSetting<int>
{
    typedef QSpinBox* Ui::EditorSettings::*widgetPtr;

    static int get(Ui::EditorSettings & ui, widgetPtr w)
    {
        return ((ui).*w)->value();
    }

    static void set(Ui::EditorSettings & ui, widgetPtr w, int value)
    {
        ((ui).*w)->setValue(value);
    }
};

template<>
struct UiSetting<bool>
{
    typedef QCheckBox* Ui::EditorSettings::*widgetPtr;

    static bool get(Ui::EditorSettings & ui, widgetPtr w)
    {
        return ((ui).*w)->isChecked();
    }

    static void set(Ui::EditorSettings & ui, widgetPtr w, bool value)
    {
        ((ui).*w)->setChecked(value);
    }
};

template<typename T>
class Setting: public SciSetting
{
public:
    typedef T(QsciScintilla::*G)() const;
    typedef void(QsciScintilla::*S)(T);

    Setting()
    {
    }

    Setting(QString const & n, G g, S s, typename UiSetting<T>::widgetPtr w) :
        name(n), setter(s), getter(g), widget(w)
    {
    }

    void apply(QsciScintilla * editor)
    {
        ((editor)->*setter)(value);
    }

    void load(QsciScintilla * editor)
    {
        value = ((editor)->*getter)();
    }

    void read(QSettings & set)
    {
        value = SettingCast<T>::exec(set.value(name).value<
                typename SettingCast<T>::realType> ());
    }

    void write(QSettings & set)
    {
        set.setValue(name, QVariant::fromValue(SettingCast<T>::exec(value)));
    }

    void setUi(Ui::EditorSettings & ui)
    {
        UiSetting<T>::set(ui, widget, value);
    }

    void getUi(Ui::EditorSettings & ui)
    {
        value = UiSetting<T>::get(ui, widget);
    }

private:
    QString name;
    S setter;
    G getter;
    typename UiSetting<T>::widgetPtr widget;
protected:
    T value;
};

class Folding: public Setting<QsciScintilla::FoldStyle>
{
public:
    Folding() :
        Setting("folding", &QsciScintilla::folding, 0,
                &Ui::EditorSettings::foldingStyle)
    {
    }

    void apply(QsciScintilla * editor)
    {
        editor->setFolding(value);
    }

};

#define SETTING(name, Name, type) settings.push_back(new Setting<type>(#name, &QsciScintilla:: name, &QsciScintilla:: set##Name, &Ui::EditorSettings:: name));

SciSettings::SciSettings(QObject * parent, ICoreFunctions *core) :
    QObject(parent), g_Core(core)
{
    SETTING(autoIndent, AutoIndent, bool);
    SETTING(backspaceUnindents, BackspaceUnindents, bool);
    SETTING(indentationGuides, IndentationGuides, bool);
    SETTING(indentationsUseTabs, IndentationsUseTabs, bool);
    SETTING(tabIndents, TabIndents, bool);
    SETTING(indentationWidth, IndentationWidth, int);
    SETTING(tabWidth, TabWidth, int);

    //FIXME:: setter has extra argument
    //SETTING(folding, Folding, QsciScintilla::FoldStyle, int, 2);
    settings.push_back(new Folding());

    SETTING(edgeColumn, EdgeColumn, int);
    SETTING(edgeMode, EdgeMode, QsciScintilla::EdgeMode);

    SETTING(autoCompletionCaseSensitivity, AutoCompletionCaseSensitivity, bool);
    SETTING(autoCompletionReplaceWord, AutoCompletionReplaceWord, bool);
    SETTING(autoCompletionShowSingle, AutoCompletionShowSingle, bool);
    SETTING(autoCompletionFillupsEnabled, AutoCompletionFillupsEnabled, bool);
    SETTING(autoCompletionThreshold, AutoCompletionThreshold, int);
    SETTING(autoCompletionSource, AutoCompletionSource, QsciScintilla::AutoCompletionSource);
    SETTING(autoCompletionUseSingle, AutoCompletionUseSingle, QsciScintilla::AutoCompletionUseSingle);

    SETTING(wrapMode, WrapMode, QsciScintilla::WrapMode);
    SETTING(wrapIndentMode, WrapIndentMode, QsciScintilla::WrapIndentMode);

	sd = NULL;
	connect(sd, SIGNAL(settingsChanged()), this, SLOT(settingsUpdated()));
    loadSettings();
}

SciSettings::~SciSettings()
{
    saveSettings();
foreach(SciSetting * setting, settings)
{
    delete setting;
}
}

void SciSettings::applySettings(QsciScintilla * editor)
{
foreach(SciSetting * setting, settings)
{
    setting->apply(editor);
}
}

void SciSettings::loadSettings()
{
    QSettings set;
    set.setIniCodec("UTF-8");
    set.beginGroup("TextEditor");
    foreach(SciSetting * setting, settings)
    {
        setting->read(set);
    }
    set.endGroup();
    emit settingsChanged();
}

void SciSettings::saveSettings()
{
    QSettings set;
    set.setIniCodec("UTF-8");
    set.beginGroup("TextEditor");
    foreach(SciSetting * setting, settings)
    {
        setting->write(set);
    }
    set.endGroup();
}

void SciSettings::setUi(Ui::EditorSettings & ui)
{
foreach(SciSetting * setting, settings)
{
    setting->setUi(ui);
}
}

void SciSettings::getUi(Ui::EditorSettings & ui)
{
    foreach(SciSetting * setting, settings)
    {
        setting->getUi(ui);
    }

    emit settingsChanged();
}

void SciSettings::showSettings()
{
	if (!sd)
	{
		sd = new SettingsDialog(parent()->isWidgetType() ? qobject_cast<QWidget*>(parent()) : NULL, this);
		connect(sd, SIGNAL(settingsChanged()), this, SLOT(settingsChanged()));
	}

	sd->show();
}

void SciSettings::settingsUpdated()
{
    emit settingsChanged();
}
