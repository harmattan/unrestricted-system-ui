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
#ifndef SHUTDOWNUI_H
#define SHUTDOWNUI_H

#include <MApplicationPage>

class MLabel;
class MImageWidget;

class ShutdownUI : public MApplicationPage
{
    Q_OBJECT

public:
    ShutdownUI ();
    virtual ~ShutdownUI ();
    virtual void createContent ();

    void showWindow (QString& text1, QString& text2, int timeout);

private slots:
    void showLogo ();
    void turnOffScreen ();

private:
    MLabel          *m_text1;
    MLabel          *m_text2;
    MImageWidget    *m_logo;
};

#endif // SHUTDOWNUI_H
