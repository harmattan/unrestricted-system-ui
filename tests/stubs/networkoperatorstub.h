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
#ifndef NETWORKOPERATOR_H
#define NETWORKOPERATOR_H

#include <QObject>

namespace Cellular
{

class NetworkOperator : public QObject
{
    Q_OBJECT

public:
    NetworkOperator(QObject *parent = 0);
    ~NetworkOperator();

    QString name() const;
    QString mnc() const;
    QString mcc() const;

    void setName(const QString &name);
    void setMnc(const QString &mnc);
    void setMcc(const QString &mcc);

    bool isValid();

signals:
    void nameChanged(const QString &name);

private:
    QString currentName;
    QString currentMnc;
    QString currentMcc;

};

}

#endif  /* !NETWORKOPERATOR_H */
