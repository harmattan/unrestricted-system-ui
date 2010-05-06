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

#ifndef STATUSINDICATORICONSTYLE_H_
#define STATUSINDICATORICONSTYLE_H_

#include "statusindicatoranimationstyle.h"

class StatusIndicatorIconStyle : public StatusIndicatorAnimationStyle
{
    Q_OBJECT
    M_STYLE(StatusIndicatorIconStyle)

    //! The list of IDs of the images to be shown in the status indicator separated by spaces
    M_STYLE_ATTRIBUTE(QString, imageList, ImageList)
};

class StatusIndicatorIconStyleContainer : public StatusIndicatorAnimationStyleContainer
{
    M_STYLE_CONTAINER(StatusIndicatorIconStyle)
};

#endif /* STATUSINDICATORICONSTYLE_H_ */
