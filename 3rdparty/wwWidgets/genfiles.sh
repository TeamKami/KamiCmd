#!/bin/bash

WIDGET=$1
LOWER=`echo $1 | awk 'END { print tolower($0)}'`
UPPER=`echo $1 | awk 'END { print toupper($0)}'`

mkdir widgets/$LOWER
touch widgets/$LOWER/$LOWER.h
touch widgets/$LOWER/$LOWER.cpp

IFACE=$LOWER"iface"
GUARD=$UPPER"IFACE_H"

IFACECLASS=$WIDGET"Iface"

cat > plugin/$IFACE.h << EOF
//
// C++ Interface: $IFACE
//
// Description:
//
//
// Author: Witold Wysota <wysota@wysota.eu.org>, (C) 2008
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef $GUARD
#define $GUARD

#include "wwinterfaces.h"

class $IFACECLASS : public wwWidgetInterface
{
Q_OBJECT
Q_INTERFACES(QDesignerCustomWidgetInterface);
public:
    $IFACECLASS(QObject *parent = 0);
    ~$IFACECLASS();
    QWidget *createWidget(QWidget *parent);
    QIcon icon() const;
};
                
#endif
EOF

cat > plugin/$IFACE.cpp << EOF2
//
// C++ Implementation: %{MODULE}
//
// Description:
//
//
// Author: %{AUTHOR} <%{EMAIL}>, (C) %{YEAR}
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "$IFACE.h"
#include "$LOWER.h"

$IFACECLASS::$IFACECLASS(QObject *parent)
        : wwWidgetInterface(parent){}
        
$IFACECLASS::~$IFACECLASS() {}
        
QWidget * $IFACECLASS::createWidget(QWidget * parent) {
    return new $WIDGET(parent);
}
EOF2

awk "`echo '{ sub(/#ADDHERE#/, "'$WIDGET '#ADDHERE#") } ; { print $0 }'`" wwwidgets_install.pri > wwwidgets_install.pri.tmp
cp wwwidgets_install.pri wwwidgets_install.pri.bak
mv wwwidgets_install.pri.tmp wwwidgets_install.pri
