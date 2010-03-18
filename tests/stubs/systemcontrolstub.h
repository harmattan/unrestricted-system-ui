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
#ifndef SYSTEMCONTROL_H
#define SYSTEMCONTROL_H

#include <QObject>

namespace Cellular
{

class SystemControl : public QObject
{
    Q_OBJECT

public:
    SystemControl(QObject *parent = 0);
    ~SystemControl();

    bool power() const;
    void setPower(bool value);

signals:
    void powerChanged(bool value);

};

}
#endif // SYSTEMCONTROL_H
