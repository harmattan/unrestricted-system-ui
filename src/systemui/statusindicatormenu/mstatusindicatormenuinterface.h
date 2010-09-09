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

#ifndef MSTATUSINDICATORMENUINTERFACE_H
#define MSTATUSINDICATORMENUINTERFACE_H

/**
 * The MStatusIndicatorMenuInterface interface represents a status indicator
 * menu application, its window and main page. The interface can be used to
 * show the main status indicator menu application page or to hide the status
 * indicator menu window.
 */
class MStatusIndicatorMenuInterface
{
public:
    /*!
     * Shows the main status indicator menu application page.
     */
    virtual void showStatusIndicatorMenu() = 0;

    /*!
     * Hides the status indicator menu window.
     */
    virtual void hideStatusIndicatorMenu() = 0;
};

#endif // MSTATUSINDICATORMENUINTERFACE_H
