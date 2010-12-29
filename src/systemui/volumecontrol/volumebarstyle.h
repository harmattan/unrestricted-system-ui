/***************************************************************************
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
#ifndef VOLUMEBARSTYLE_H
#define VOLUMEBARSTYLE_H

#include <MWidgetStyle>

class VolumeBarStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(VolumeBarStyle)

    //! The volume bar visibility timeout
    M_STYLE_ATTRIBUTE(int, visibleDuration, VisibleDuration)
    //! The fade in/out animation length
    M_STYLE_ATTRIBUTE(int, fadeDuration, FadeDuration)
    //! The movement animation length
    M_STYLE_ATTRIBUTE(int, moveDuration, MoveDuration)
    //! The opacity of the volume bar
    M_STYLE_ATTRIBUTE(qreal, volumeBarOpacity, VolumeBarOpacity)
    //! volume non-muted icon id
    M_STYLE_ATTRIBUTE(QString, iconId, IconId)
    //! volume muted icon id
    M_STYLE_ATTRIBUTE(QString, mutedIconId, MutedIconId)
    //! the volume icon size
    M_STYLE_ATTRIBUTE(QSize, iconSize, IconSize)
};

class VolumeBarStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(VolumeBarStyle)
};

#endif
