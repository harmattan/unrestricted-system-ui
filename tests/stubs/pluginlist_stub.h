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
#ifndef PLUGINLIST_STUB
#define PLUGINLIST_STUB

#include "pluginlist.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class PluginListStub : public StubBase {
public:
    virtual void PluginListConstructor(StatusIndicatorMenuWindow *window, QGraphicsItem *parent);
    virtual void PluginListDestructor();
    virtual void showStatusIndicatorMenu();
    virtual void hideStatusIndicatorMenu();
    virtual void addPlugin(const QString &path);
}; 

// 2. IMPLEMENT STUB
void PluginListStub::PluginListConstructor(StatusIndicatorMenuWindow *window, QGraphicsItem *parent) {
    Q_UNUSED(window);
    Q_UNUSED(parent);

}
void PluginListStub::PluginListDestructor() {

}

void PluginListStub::showStatusIndicatorMenu() {
    stubMethodEntered("showStatusIndicatorMenu");
}

void PluginListStub::hideStatusIndicatorMenu() {
    stubMethodEntered("hideStatusIndicatorMenu");
}

void PluginListStub::addPlugin(const QString &path) {
    QList<ParameterBase*> params;
    params.append( new Parameter<const QString & >(path));
    stubMethodEntered("addPlugin",params);
}



// 3. CREATE A STUB INSTANCE
PluginListStub gDefaultPluginListStub;
PluginListStub* gPluginListStub = &gDefaultPluginListStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
PluginList::PluginList(StatusIndicatorMenuWindow *window, QGraphicsItem *parent) {
    gPluginListStub->PluginListConstructor(window, parent);
}

PluginList::~PluginList() {
    gPluginListStub->PluginListDestructor();
}

void PluginList::showStatusIndicatorMenu() {
    gPluginListStub->showStatusIndicatorMenu();
}

void PluginList::hideStatusIndicatorMenu() {
    gPluginListStub->hideStatusIndicatorMenu();
}

void PluginList::loadPlugins() {

}

void PluginList::addPlugin(const QString &path) {
    gPluginListStub->addPlugin(path);
}

#endif
