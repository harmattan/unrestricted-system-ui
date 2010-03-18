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
#include "unlockslidermodel.h"

const qreal& UnlockSliderModel::position() const
{
    return _position();
}

void UnlockSliderModel::setPosition(const qreal& position)
{
    // limit position to 0..1 range
    qreal pos = position < 0.0f ? 0.0f : (position > 1.0f ? 1.0f : position);

    if (pos != _position()) {
        _position() = pos;
        memberModified(Position);
    }
}
