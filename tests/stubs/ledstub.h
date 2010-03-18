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
#ifndef QMLED_H
#define QMLED_H

#include <QObject>

namespace Maemo
{
/**
 * @brief Provides information and actions on device LED.
 */
class QmLED : public QObject
{
    Q_OBJECT

public:
    QmLED(QObject *parent = 0);
    ~QmLED();

    /**
     * Activate pattern in device LED.
     * @param pattern Pattern to activate.
     * @return True on success, false otherwise.
     * @todo Add description of strings for patterns.
     */
    bool activate(const QString &pattern);

    /**
     * Deactivate pattern in device LED.
     * @param pattern Pattern to deactivate.
     * @return True on success, false otherwise.
     * @todo Add description of string for patterns.
     */
    bool deactivate(const QString &pattern);

};

} //namespace Maemo


#endif // QMLED_H
