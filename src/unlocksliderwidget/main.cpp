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
#include <MTheme>
#include <MApplication>
#include <MApplicationWindow>
#include <MSlider>

#include "unlockslider.h"
#include "unlocksliderview.h"

int main(int argc, char** argv)
{
    MApplication app(argc, argv);

    MApplicationWindow w;
    w.show();

    MApplicationPage p;
    p.appear();

    MTheme::addPixmapDirectory("./");
    MTheme::loadCSS("./unlocksliderstyle.css");

    // Create widget controller and view.
    UnlockSlider slider1;
    UnlockSlider slider2;
    slider1.setOrientation(Qt::Horizontal);
    slider2.setOrientation(Qt::Vertical);
    slider1.setGeometry(QRectF(400, 0, 400, 400));
    slider2.setGeometry(QRectF(0, 0, 400, 400));

    // Attach widget to parent widget so it will be drawn and run m application.
    slider1.setParentItem(p.centralWidget());
    slider2.setParentItem(p.centralWidget());

    return app.exec();
}

