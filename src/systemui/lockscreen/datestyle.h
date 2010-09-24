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

#ifndef DATESTYLE_H_
#define DATESTYLE_H_

#include <MWidgetStyle>

class DateStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(DateStyle)

    //! Format string for the date
    M_STYLE_ATTRIBUTE(QString, dateFormat, DateFormat)
    //! Label alignment
    M_STYLE_ATTRIBUTE(Qt::Alignment, horizontalAlign, HorizontalAlign)
};

class DateStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(DateStyle)
};

#endif /* DATESTYLE_H_ */
