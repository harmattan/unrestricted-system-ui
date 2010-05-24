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

#ifndef PLUGINLIST_H_
#define PLUGINLIST_H_

#include <MWidget>
#include "mstatusindicatormenuplugininterface.h"

class StatusIndicatorMenuWindow;
class QGraphicsLinearLayout;
class QStringList;

/*!
 * The plugin list displays a list of plugins instantiated from shared
 * libraries.
 */
class PluginList : public MWidget, public MStatusIndicatorMenuInterface {
    Q_OBJECT

public:
    /*!
     * Creates a new plugin list.
     *
     * \param window the window to minimize if the application should be hidden
     * \param parent the parent item
     */
    PluginList(StatusIndicatorMenuWindow *window, QGraphicsItem *parent = NULL);

    /*!
     * Destroys the plugin list.
     */
    virtual ~PluginList();

    //! \reimp
    virtual void showStatusIndicatorMenu();
    //! \reimp_end

public slots:
    //! \reimp
    virtual void hideStatusIndicatorMenu();
    //! \reimp_end

private slots:
    /*!
     * \brief Slot for loading the status-menu plugins when we're idle
     */
    void loadPlugins();

private:
    /*!
     * Adds a plugin to the list.
     *
     * \param path the path of the plugin to load
     */
    void addPlugin(const QString &path);

    //! The layout for the list
    QGraphicsLinearLayout *mainLayout;

    //! The window to minimize if the menu should be hidden
    StatusIndicatorMenuWindow *window;

    //! Stuff to handle sequential delayed plugin loading. Some of the plugins
    //! are loading slow, we have to get a chance to run while they are loading.
    QStringList pluginsToBeLoaded;
};

#endif /* PLUGINLIST_H_ */
