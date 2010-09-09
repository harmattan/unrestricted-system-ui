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

#ifndef STATUSINDICATORANIMATIONSTYLE_H_
#define STATUSINDICATORANIMATIONSTYLE_H_

#include <MWidgetStyle>

class StatusIndicatorAnimationStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(StatusIndicatorAnimationStyle)

    //! The number of milliseconds in which the given images are animated
    M_STYLE_ATTRIBUTE(int, animationDuration, AnimationDuration)
    //! If the indicator should size itself according to the icons size. This will override the sizes defined in the style
    M_STYLE_ATTRIBUTE(bool, useIconSize, UseIconSize)
};

class StatusIndicatorAnimationStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(StatusIndicatorAnimationStyle)

    //! \brief Icon mode.
    M_STYLE_MODE(Icon)
};

#endif /* STATUSINDICATORANIMATIONSTYLE_H_ */
