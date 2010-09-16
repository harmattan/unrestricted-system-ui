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

#ifndef LOCKSCREENHEADERSTYLE_H_
#define LOCKSCREENHEADERSTYLE_H_

#include <MWidgetStyle>

class LockScreenHeaderStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(LockScreenHeaderStyle)
};

class LockScreenHeaderStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(LockScreenHeaderStyle)
};

#endif /* LOCKSCREENHEADERSTYLE_H_ */
