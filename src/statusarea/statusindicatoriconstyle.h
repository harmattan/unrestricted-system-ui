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

#include <MWidgetStyle>

class StatusIndicatorIconStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(StatusIndicatorIconStyle)
};

class StatusIndicatorIconStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(StatusIndicatorIconStyle)

    //! \brief Icon mode.
    M_STYLE_MODE(Icon)
};

#endif /* STATUSINDICATORICONSTYLE_H_ */
