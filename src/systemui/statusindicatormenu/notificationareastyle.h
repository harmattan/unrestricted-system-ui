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

#ifndef NOTIFICATIONAREASTYLE_H
#define NOTIFICATIONAREASTYLE_H

#include <MWidgetStyle>

class NotificationAreaStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(NotificationAreaStyle)

    //! Controls whether to show a clear button on the notification area
    M_STYLE_ATTRIBUTE(bool, clearButton, ClearButton);
    //! The maximum number of banners in notification area
    M_STYLE_ATTRIBUTE(int, maxBanners, MaxBanners);
};

class NotificationAreaStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(NotificationAreaStyle)
};

#endif
