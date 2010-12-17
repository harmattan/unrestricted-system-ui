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

#ifndef PRIVATEMODEAPPLET_H
#define PRIVATEMODEAPPLET_H

#include <DcpAppletIf>
#include <QObject>
class DcpWidget;
class MAction;

class PrivateModeApplet : public QObject, public DcpAppletIf
{
    Q_OBJECT
    Q_INTERFACES(DcpAppletIf)

public:
    virtual void init();
    virtual DcpWidget* constructWidget(int widgetId);
    virtual QVector<MAction *> viewMenuItems();
    virtual DcpBrief* constructBrief(int);
};
#endif // PRIVATEMODEAPPLET_H
