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
#ifndef LOCKSCREENWINDOW_STUB
#define LOCKSCREENWINDOW_STUB

#include "lockscreenui.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class LockScreenWindowStub : public StubBase {
  public:
  virtual void LockScreenWindowConstructor(MWindow *window, MWidget *locklift, MWidget *lockland);
  virtual void LockScreenWindowDestructor();
  virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
  virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
  virtual void reloadLandscapeBackground();
  virtual void reloadPortraitBackground();
  virtual void updateDnDicon();
  virtual void resetState();
}; 

// 2. IMPLEMENT STUB
void LockScreenWindowStub::LockScreenWindowConstructor(MWindow *window, MWidget *locklift, MWidget *lockland) {
  Q_UNUSED(window);
  Q_UNUSED(locklift);
  Q_UNUSED(lockland);

}
void LockScreenWindowStub::LockScreenWindowDestructor() {

}
void LockScreenWindowStub::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QPainter * >(painter));
  params.append( new Parameter<const QStyleOptionGraphicsItem * >(option));
  params.append( new Parameter<QWidget * >(widget));
  stubMethodEntered("paint",params);
}

void LockScreenWindowStub::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mousePressEvent",params);
}

void LockScreenWindowStub::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseMoveEvent",params);
}

void LockScreenWindowStub::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  QList<ParameterBase*> params;
  params.append( new Parameter<QGraphicsSceneMouseEvent * >(event));
  stubMethodEntered("mouseReleaseEvent",params);
}

void LockScreenWindowStub::reloadLandscapeBackground() {
  stubMethodEntered("reloadLandscapeBackground");
}

void LockScreenWindowStub::reloadPortraitBackground() {
  stubMethodEntered("reloadPortraitBackground");
}

void LockScreenWindowStub::updateDnDicon() {
  stubMethodEntered("updateDnDicon");
}

void LockScreenWindowStub::resetState() {
  stubMethodEntered("resetState");
}

// 3. CREATE A STUB INSTANCE
LockScreenWindowStub gDefaultLockScreenWindowStub;
LockScreenWindowStub* gLockScreenWindowStub = &gDefaultLockScreenWindowStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
LockScreenWindow::LockScreenWindow(MWindow *window, MWidget *locklift, MWidget *lockland) :
    m_confBgLandscape (0), m_confBgPortrait (0), m_Window (window),
    m_DnDicon (0), m_DnDstate (0), m_LockLiftArea (0), m_LockLandArea (0)
{
  gLockScreenWindowStub->LockScreenWindowConstructor(window, locklift, lockland);
}

LockScreenWindow::~LockScreenWindow() {
  gLockScreenWindowStub->LockScreenWindowDestructor();
}

void LockScreenWindow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
  gLockScreenWindowStub->paint(painter, option, widget);
}

void LockScreenWindow::mousePressEvent(QGraphicsSceneMouseEvent *event) {
  gLockScreenWindowStub->mousePressEvent(event);
}

void LockScreenWindow::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
  gLockScreenWindowStub->mouseMoveEvent(event);
}

void LockScreenWindow::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
  gLockScreenWindowStub->mouseReleaseEvent(event);
}

void LockScreenWindow::reloadLandscapeBackground() {
  gLockScreenWindowStub->reloadLandscapeBackground();
}

void LockScreenWindow::reloadPortraitBackground() {
  gLockScreenWindowStub->reloadPortraitBackground();
}

void LockScreenWindow::updateDnDicon() {
  gLockScreenWindowStub->updateDnDicon();
}

void LockScreenWindow::resetState() {
  gLockScreenWindowStub->resetState();
}

#endif
