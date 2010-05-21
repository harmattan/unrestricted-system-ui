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
    virtual void PluginListConstructor(MWindow *applicationWindow, MApplicationPage *applicationPage, QGraphicsItem *parent);
    virtual void PluginListDestructor();
    virtual void showStatusIndicatorMenu();
    virtual void setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension);
    virtual void settingsButtonClicked();
    virtual void setNotificationCount(int notificationCount);
    virtual void hideStatusIndicatorMenu();
    virtual void addPlugin(const QString &path);
}; 

// 2. IMPLEMENT STUB
void PluginListStub::PluginListConstructor(MWindow *applicationWindow, MApplicationPage *applicationPage, QGraphicsItem *parent) {
    Q_UNUSED(applicationWindow);
    Q_UNUSED(applicationPage);
    Q_UNUSED(parent);

}
void PluginListStub::PluginListDestructor() {

}

void PluginListStub::setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension)
{
    QList<ParameterBase*> params;
    params.append(new Parameter<MApplicationExtensionInterface*>(extension));
    stubMethodEntered("setStatusIndicatorMenuInterface", params);
}

void PluginListStub::showStatusIndicatorMenu() {
    stubMethodEntered("showStatusIndicatorMenu");
}

void PluginListStub::settingsButtonClicked() {
    stubMethodEntered("settingsButtonClicked");
}

void PluginListStub::setNotificationCount(int notificationCount) {
    QList<ParameterBase*> params;
    params.append( new Parameter<int >(notificationCount));
    stubMethodEntered("setNotificationCount",params);
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
PluginList::PluginList(MWindow *applicationWindow, MApplicationPage *applicationPage, QGraphicsItem *parent) {
    gPluginListStub->PluginListConstructor(applicationWindow, applicationPage, parent);
}

PluginList::~PluginList() {
    gPluginListStub->PluginListDestructor();
}

void PluginList::showStatusIndicatorMenu() {
    gPluginListStub->showStatusIndicatorMenu();
}

void PluginList::settingsButtonClicked() {
    gPluginListStub->settingsButtonClicked();
}

void PluginList::setNotificationCount(int notificationCount) {
    gPluginListStub->setNotificationCount(notificationCount);
}

void PluginList::hideStatusIndicatorMenu() {
    gPluginListStub->hideStatusIndicatorMenu();
}

void PluginList::loadPlugins() {

}

void PluginList::addPlugin(const QString &path) {
    gPluginListStub->addPlugin(path);
}

void PluginList::setStatusIndicatorMenuInterface(MApplicationExtensionInterface *extension)
{
    gPluginListStub->setStatusIndicatorMenuInterface(extension);
}

#endif
