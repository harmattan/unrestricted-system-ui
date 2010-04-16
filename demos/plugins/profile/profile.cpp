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
/*
 * profile.cpp
 *
 * This file is part of system ui
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating,
 * any or all of this material requires the prior written consent of
 * Nokia Corporation. This material also contains confidential
 * information which may not be disclosed to others without the prior
 * written consent of Nokia.
 */

#include "profile.h"
#include <MStatusIndicatorMenuPluginInterface>
#include <MButton>
#include <MButtonGroup>
#include <MContainer>
#include <MApplicationPage>
#include <QGraphicsLinearLayout>

Profile::Profile(MStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent) :
    MWidget(parent),
    statusIndicatorMenu(statusIndicatorMenu),
    profileModificationPage(new MApplicationPage)
{
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create a container for the profiles
    MContainer *container = new MContainer;
    MWidget *widget = new MWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(layout);
    container->setTitle("Profiles");
    container->setCentralWidget(widget);
    connect(container, SIGNAL(headerClicked()), this, SLOT(showProfileModificationPage()));
    mainLayout->addItem(container);

    // Create a button group for the profiles
    MButtonGroup *buttonGroup = new MButtonGroup(widget);
    buttonGroup->connect(buttonGroup, SIGNAL(buttonClicked(MButton *)), this, SLOT(buttonClicked(MButton *)));

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

    // Set up the profile modification page
    profileModificationPage->setTitle("Profile");
    profileModificationPage->setEscapeMode(MApplicationPageModel::EscapeManualBack);
    connect(profileModificationPage, SIGNAL(backButtonClicked()), this, SLOT(buttonClicked()));
}

Profile::~Profile()
{
    delete profileModificationPage;
}

void Profile::showProfileModificationPage()
{
    profileModificationPage->appear();
}

void Profile::buttonClicked()
{
    statusIndicatorMenu.showStatusIndicatorMenu();
}

void Profile::buttonClicked(MButton *)
{
    statusIndicatorMenu.hideStatusIndicatorMenu();
}
