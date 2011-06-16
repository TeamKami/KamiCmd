#include "LinkedUnit.h"
#include "library.h"
#include "../IPanel.h"

void LinkedUnit::Create( IUnit *active, IUnit *inactive, bool isActiveLeft )
{
	left = isActiveLeft ? active : inactive;
	right = isActiveLeft ? inactive : active;
	this->active = active;
	setFocusProxy(active);
	splitter->addWidget(left);
	splitter->addWidget(right);
	splitter->setSizes(QList<int>() << width() << width());
	layout->addWidget(splitter);
	left->show();
	right->show();
	connect(left, SIGNAL(FocusIn()), SLOT(LinkedUnitFocusIn()));
	connect(right, SIGNAL(FocusIn()), SLOT(LinkedUnitFocusIn()));
	connect(left, SIGNAL(TextChanged()), this, SIGNAL(TextChanged()));
	connect(right, SIGNAL(TextChanged()), this, SIGNAL(TextChanged()));
}

LinkedUnit::LinkedUnit(QWidget *parent)
	: ILinkedUnit(parent)
{
	left = right = NULL;
	splitter = new Splitter(Qt::Horizontal, this);
	setCentralWidget(new QWidget(this));
	layout = new QVBoxLayout(centralWidget());
	layout->setMargin(0);
}

QString LinkedUnit::GetText()
{
	if (left && right)
		return left->GetText() + " | " + right->GetText();
	return QString("");
}

void LinkedUnit::LinkedUnitFocusIn()
{
	active = static_cast<IUnit *>(sender());
	setFocusProxy(active);
}

void LinkedUnit::SaveState( QSettings &set )
{
	if (left)
	{
		set.beginGroup("left"); // extend this to work not only with panels. Add "type" field
		left->SaveState(set);
		set.endGroup();
	}
	if (right)
	{
		set.beginGroup("right");
		right->SaveState(set);
		set.endGroup();
	}
	if (left)
		set.setValue("isActiveLeft", left == active);
	//set.setValue("splitterPos", splitter->pos());
}

void LinkedUnit::LoadState( QSettings &set )
{
	if (IPanel *left = dynamic_cast<IPanel *>(g_Core->QueryModule("Panel", 1)))
	{
		IPanel *right = dynamic_cast<IPanel *>(g_Core->QueryModule("Panel", 1));
		bool isActiveLeft = set.value("isActiveLeft", true).toBool();
		Create(isActiveLeft ? left : right, isActiveLeft ? right : left, isActiveLeft);
		set.beginGroup("left");
		left->LoadState(set);
		set.endGroup();
		set.beginGroup("right");
		right->LoadState(set);
		set.endGroup();
		if (isActiveLeft)
			active = left;
		else
			active = right;
		active->setFocus();
		//splitter->setPos set.value("splitterPos");
	}
	else
		g_Core->DebugWrite("UnitManager_Tabs", "Panel module not found", ICoreFunctions::Error);
}

IUnit * LinkedUnit::GetActiveUnit()
{
	return active;
}

IUnit * LinkedUnit::GetInactiveUnit()
{
	return active == left ? right : left;
}

IUnit * LinkedUnit::GetLeftUnit()
{
	return left;
}

IUnit * LinkedUnit::GetRightUnit()
{
	return right;
}

QIcon LinkedUnit::GetIcon()
{
	return QIcon();
}


QSplitterHandle* Splitter::createHandle()
{
	SplitterHandle *splitterHandle = new SplitterHandle(orientation(), this);
	connect(splitterHandle, SIGNAL(MouseDoubleClicked()), SLOT(MouseDoubleClicked()));
	return splitterHandle;
}

void Splitter::MouseDoubleClicked()
{
	setSizes(QList<int>() << width() << width());
}

void SplitterHandle::mouseDoubleClickEvent( QMouseEvent *event )
{
	if (event->button() == Qt::LeftButton)
		emit MouseDoubleClicked();
	QSplitterHandle::mouseDoubleClickEvent(event);
}
