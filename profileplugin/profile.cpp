/*
 * profile.cpp
 *
 * This file is part of duistatusindicatormenu
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
#include "profiledatainterface.h"

#include <DuiButton>
#include <DuiButtonGroup>
#include <DuiContainer>
#include <DuiControlPanelIf>
#include <DuiGridLayoutPolicy>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiLocale>
#include <DuiStatusIndicatorMenuPluginInterface>

#include <QGraphicsLinearLayout>


Profile::Profile(DuiStatusIndicatorMenuInterface &statusIndicatorMenu, QGraphicsItem *parent) :
    DuiWidget(parent),
    statusIndicatorMenu(statusIndicatorMenu),
    dataIf(NULL)
{    
    Q_UNUSED(statusIndicatorMenu);
    dataIf = new ProfileDataInterface();

    QGraphicsLinearLayout *mainLayout = new QGraphicsLinearLayout(Qt::Vertical);
    setLayout(mainLayout);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Create a container for the profiles
    DuiContainer *container = new DuiContainer;
    DuiWidget *widget = new DuiWidget;
    DuiLayout *layout = new DuiLayout;
    DuiLinearLayoutPolicy *landscapePolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    DuiGridLayoutPolicy *portraitPolicy = new DuiGridLayoutPolicy(layout);
    layout->setLandscapePolicy(landscapePolicy);
    layout->setPortraitPolicy(portraitPolicy);
    widget->setLayout(layout);
    container->setTitle(trid("qtn_prof_profile", "Profile"));
    container->setCentralWidget(widget);
    connect(container, SIGNAL(headerClicked()), this, SLOT(showProfileModificationPage()));
    mainLayout->addItem(container);

    // Create a button group for the profiles
    DuiButtonGroup *buttonGroup = new DuiButtonGroup(widget);
    buttonGroup->connect(buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(buttonClicked(int)));

    // Find out the possible profiles
    QStringList profileNames = dataIf->profileNames();
    QString activeName = dataIf->currentProfileName();

    // Place the buttons in the button group and in the layout
    DuiButton *button;
    for(int i=0; i<profileNames.count(); ++i) {
        button = new DuiButton(dataIf->profileName2Text(profileNames.at(i)));
        button->setObjectName("profileButton");
        button->setCheckable(true);
        if(profileNames.at(i) == activeName)
            button->setChecked(true);
        landscapePolicy->addItem(button);
        portraitPolicy->addItemAtPosition(button, i/2, i%2);
        buttonGroup->addButton(button);        
    }
}

Profile::~Profile()
{
    delete dataIf;
    dataIf = NULL;
}

void Profile::showProfileModificationPage()
{    
    // instantiate the interface
    DuiControlPanelIf cpIf;
    // check the interface is valid
    if (!cpIf.isValid())
        return;    
    cpIf.appletPage("Profile");
}

