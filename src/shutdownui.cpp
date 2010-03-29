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
#include "shutdownui.h"
#include "sysuid.h"

#include <DuiLabel>
#include <DuiSceneManager>
#include <DuiApplicationWindow>
#include <DuiLocale>

#include <QDebug>

ShutdownUI::ShutdownUI() :
        text(0)
{
    setProperty ("pannable", QVariant (false));

    // let's hide home button
    setComponentsDisplayMode (DuiApplicationPage::AllComponents,
                              DuiApplicationPageModel::Hide);
}

ShutdownUI::~ShutdownUI()
{
}

void ShutdownUI::createContent()
{
    qDebug() << "ShutdownUI::createContent()";
    DuiApplicationPage::createContent();

    QGraphicsWidget* panel = centralWidget();

    //% "Shutting down, good bye!"
    text = new DuiLabel (qtTrId ("qtn_shut_down"), panel);
    text->setAlignment(Qt::AlignCenter);

    alignText();
}

void ShutdownUI::showWindow()
{
    DuiApplicationWindow &win = Sysuid::sysuid()->applicationWindow();
    if (win.isHidden()) {
        win.show();
    }
    qDebug() << Q_FUNC_INFO << "win.isHidden()" << win.isHidden();
}

void ShutdownUI::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    qDebug() << "ShutdownUI::resizeEvent()";
    DuiApplicationPage::resizeEvent(event);

    alignText();
}

void ShutdownUI::alignText()
{
    qDebug() << "ShutdownUI::alignText()";
    QSize size = Sysuid::sysuid()->applicationWindow().sceneManager()->visibleSceneSize();
    text->setGeometry(QRectF(0, 0, size.width(), size.height()));
}
