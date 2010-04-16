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

#ifndef MSTATUSINDICATORMENUPLUGININTERFACE_H
#define MSTATUSINDICATORMENUPLUGININTERFACE_H

#include <QtPlugin>

class MWidget;

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

/**
 * MStatusIndicatorMenuPluginInterface is the base class for status indicator
 * menu plugin classes.
 *
 * Plugin developers need to implement this interface in their plugin binary
 * and export their implementation from the binary. The host application will maintain
 * an inventory of available plugins and instantiate them using this interface
 * when needed.
 */
class MStatusIndicatorMenuPluginInterface
{
public:
    /*!
     * Destroys the plugin.
     */
    virtual ~MStatusIndicatorMenuPluginInterface() {}

    /*!
     * This method is called to construct a new MWidget instance.
     * Caller will maintain the ownership of the constructed widget and
     * will destroy it when it is no longer needed.
     *
     * \param statusIndicatorMenu an interface for controlling the status indicator menu
     * \return New MWidget class instance that visualizes this plugin.
     */
    virtual MWidget *constructWidget(MStatusIndicatorMenuInterface &statusIndicatorMenu) = 0;
};

Q_DECLARE_INTERFACE(MStatusIndicatorMenuPluginInterface, "com.meego.core.MStatusIndicatorMenuPluginInterface/1.0")

#endif // MSTATUSINDICATORMENUPLUGININTERFACE_H
