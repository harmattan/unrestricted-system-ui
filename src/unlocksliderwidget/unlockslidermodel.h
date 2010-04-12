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
#ifndef UNLOCKSLIDERMODEL_H
#define UNLOCKSLIDERMODEL_H

#include <mwidgetmodel.h>

class UnlockSliderModel : public MWidgetModel
{
    Q_OBJECT
    M_MODEL(UnlockSliderModel)

public:
    M_MODEL_PROPERTY(bool,            handlePressed, HandlePressed, true,  false)
    M_MODEL_PROPERTY(qreal,           position,      Position,      false, 0)
    M_MODEL_PROPERTY(bool,            iconVisible,   IconVisible,   true,  true)
    M_MODEL_PROPERTY(bool,            magnetic,      Magnetic,      true,  true)
    M_MODEL_PROPERTY(bool,            blinking,      Blinking,      true,  false)
    M_MODEL_PROPERTY(Qt::Orientation, orientation,   Orientation,   true,  Qt::Horizontal)
};

#endif // UNLOCKSLIDERMODEL_H
