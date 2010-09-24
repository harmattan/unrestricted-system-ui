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

#ifndef CLOCKSTYLE_H_
#define CLOCKSTYLE_H_

#include <MWidgetStyle>

/*!
 * A style class for the Clock widget.
 */
class ClockStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(ClockStyle)

    //! Formatting string for the time
    M_STYLE_ATTRIBUTE(QString, timeFormat, TimeFormat)
    //! Formatting string for the time, short variant
    M_STYLE_ATTRIBUTE(bool, shortRemoveAmPmIndicator, ShortRemoveAmPmIndicator)
    //! Label alignment
    M_STYLE_ATTRIBUTE(Qt::Alignment, horizontalAlign, HorizontalAlign)
};

class ClockStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(ClockStyle)

    //! \brief Style mode for 12 hour mode
    M_STYLE_MODE(TwelveHour)
};

#endif /* CLOCKSTYLE_H_ */
