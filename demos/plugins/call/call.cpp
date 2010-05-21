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

#include "call.h"
#include "callplugin.h"
#include <MLabel>
#include <MButton>
#include <QGraphicsLinearLayout>
#include <MLayout>
#include <MLinearLayoutPolicy>
#include <MDialog>

Call::Call(CallPlugin *callPlugin, QGraphicsItem *parent) :
    MWidget(parent),
    plugin(callPlugin)
{
    // Create a layout for the button
    MLayout *mainLayout = new MLayout();
    setLayout(mainLayout);
    MLinearLayoutPolicy *linearPolicy = new MLinearLayoutPolicy(mainLayout, Qt::Horizontal);

    // Place the button into the layout
    linearPolicy->addItem(new MLabel("Ongoing Call"));
    linearPolicy->addStretch();
    MButton *button = new MButton("Options");
    connect(button, SIGNAL(clicked()), this, SLOT(showCallDialog()));
    linearPolicy->addItem(button);
}

Call::~Call()
{
}

void Call::showCallDialog()
{
    // Create a dialog for choosing the call
    MDialog* dialog = new MDialog("Call", M::NoStandardButton);
    MWidget *centralWidget = new MWidget;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    centralWidget->setLayout(layout);
    dialog->setCentralWidget(centralWidget);

    // Place the buttons in the button group and in the layout
    MButton *button;
    button = new MButton("End Call");
    connect(button, SIGNAL(clicked()), dialog, SLOT(accept()));
    layout->addItem(button);

    // Show the dialog
    dialog->exec();

    // Hide the status indicator menu
    if(MStatusIndicatorMenuInterface *menu = plugin->statusIndicatorMenuInterface()) {
        menu->hideStatusIndicatorMenu();
    }
}
