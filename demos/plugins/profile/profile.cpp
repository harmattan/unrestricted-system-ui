/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include "profile.h"
#include "profileplugin.h"
#include <MButton>
#include <MButtonGroup>
#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MDialog>

Profile::Profile(ProfilePlugin *profilePlugin, QGraphicsItem *parent) :
    MContentItem(MContentItem::IconAndTwoTextLabels, parent),
    plugin(profilePlugin)
{
    setTitle("Profile");
    setSubtitle("Silent");
    setObjectName("StatusIndicatorMenuExtensionContentItem");
    setImageID("icon-m-profile-silent");
    connect(this, SIGNAL(clicked()), this, SLOT(showProfileDialog()));
}

Profile::~Profile()
{
}

void Profile::showProfileDialog()
{
    // Create a dialog for choosing the profile
    MDialog* dialog = new MDialog("Profile", M::NoStandardButton);
    MWidget *centralWidget = new MWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    centralWidget->setLayout(layout);
    dialog->setCentralWidget(centralWidget);

    // Create a button group for the profiles
    MButtonGroup *buttonGroup = new MButtonGroup(centralWidget);
    buttonGroup->connect(buttonGroup, SIGNAL(buttonClicked(MButton *)), dialog, SLOT(accept()));

    // Place the buttons in the button group and in the layout
    MButton *button;
    button = new MButton("Ringing");
    button->setCheckable(true);
    button->setChecked(true);
    layout->addItem(button);
    buttonGroup->addButton(button);
    button = new MButton("Silent");
    button->setCheckable(true);
    layout->addItem(button);
    buttonGroup->addButton(button);
    button = new MButton("Beep");
    button->setCheckable(true);
    layout->addItem(button);
    buttonGroup->addButton(button);
    button = new MButton("Loud");
    button->setCheckable(true);
    layout->addItem(button);
    buttonGroup->addButton(button);

    // Show the dialog
    dialog->exec();

    // Hide the status indicator menu
    if(MStatusIndicatorMenuInterface *menu = plugin->statusIndicatorMenuInterface()) {
        menu->hideStatusIndicatorMenu();
    }
}
