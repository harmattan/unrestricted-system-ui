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
#include <DuiStatusIndicatorMenuPluginInterface>
#include <DuiButton>
#include <DuiButtonGroup>
#include <DuiContainer>
#include <DuiApplicationPage>
#include <QGraphicsLinearLayout>

Profile::Profile(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent) :
    DuiWidget(parent),
    statusIndicatorMenu(statusIndicatorMenu),
    profileModificationPage(new DuiApplicationPage)
{
    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create a container for the profiles
    DuiContainer *container = new DuiContainer;
    DuiWidget *widget = new DuiWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Horizontal);
    layout->setContentsMargins(0, 0, 0, 0);
    widget->setLayout(layout);
    container->setTitle("Profiles");
    container->setCentralWidget(widget);
    connect(container, SIGNAL(headerClicked()), this, SLOT(showProfileModificationPage()));
    mainLayout->addItem(container);

    // Create a button group for the profiles
    DuiButtonGroup *buttonGroup = new DuiButtonGroup(widget);
    buttonGroup->connect(buttonGroup, SIGNAL(buttonClicked(DuiButton *)), this, SLOT(buttonClicked(DuiButton *)));

    // Place the buttons in the button group and in the layout
    DuiButton *button;
    button = new DuiButton("Ringing");
    button->setCheckable(true);
    button->setChecked(true);
    layout->addItem(button);
    buttonGroup->addButton(button);
    button = new DuiButton("Silent");
    button->setCheckable(true);
    layout->addItem(button);
    buttonGroup->addButton(button);
    button = new DuiButton("Beep");
    button->setCheckable(true);
    layout->addItem(button);
    buttonGroup->addButton(button);
    button = new DuiButton("Loud");
    button->setCheckable(true);
    layout->addItem(button);
    buttonGroup->addButton(button);

    // Set up the profile modification page
    profileModificationPage->setTitle("Profile");
    profileModificationPage->setEscapeButtonMode(DuiEscapeButtonPanelModel::BackMode);
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

void Profile::buttonClicked(DuiButton *)
{
    statusIndicatorMenu.hideStatusIndicatorMenu();
}
