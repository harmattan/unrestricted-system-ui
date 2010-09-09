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

#ifndef MSTATUSINDICATORMENUEXTENSIONINTERFACE_H
#define MSTATUSINDICATORMENUEXTENSIONINTERFACE_H

#include <QObject>
#include <mapplicationextensioninterface.h>
#include <mstatusindicatormenuinterface.h>


/**
 * MStatusIndicatorMenuExtensionInterface is the base class for status indicator
 * menu extension classes.
 *
 * Extension developers need to implement this interface in their extension binary
 * and export their implementation from the binary. The host application will maintain
 * an inventory of available extensions and instantiate them using this interface
 * when needed.
 */
class MStatusIndicatorMenuExtensionInterface : public MApplicationExtensionInterface
{
public:
    /*!
     * Destructor.
     */
    virtual ~MStatusIndicatorMenuExtensionInterface() {}

    /*!
     * Gives the extension a reference to the menu interface.
     */
    virtual void setStatusIndicatorMenuInterface(MStatusIndicatorMenuInterface &menuInterface) = 0;
};

Q_DECLARE_INTERFACE(MStatusIndicatorMenuExtensionInterface, "com.meego.core.MStatusIndicatorMenuExtensionInterface/1.0")

#endif // MSTATUSINDICATORMENUEXTENSIONINTERFACE_H
