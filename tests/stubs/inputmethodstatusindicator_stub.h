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
#ifndef INPUTMETHODSTATUSINDICATOR_STUB
#define INPUTMETHODSTATUSINDICATOR_STUB

#include "statusindicator_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class InputMethodStatusIndicatorStub : public StubBase
{
public:
    virtual void InputMethodStatusIndicatorConstructor(QGraphicsItem *parent);
    virtual void InputMethodStatusIndicatorDestructor();
};

// 2. IMPLEMENT STUB
void InputMethodStatusIndicatorStub::InputMethodStatusIndicatorConstructor(QGraphicsItem *parent)
{
    Q_UNUSED(parent);

}
void InputMethodStatusIndicatorStub::InputMethodStatusIndicatorDestructor()
{

}



// 3. CREATE A STUB INSTANCE
InputMethodStatusIndicatorStub gDefaultInputMethodStatusIndicatorStub;
InputMethodStatusIndicatorStub *gInputMethodStatusIndicatorStub = &gDefaultInputMethodStatusIndicatorStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
InputMethodStatusIndicator::InputMethodStatusIndicator(QGraphicsItem *parent)
{
    gInputMethodStatusIndicatorStub->InputMethodStatusIndicatorConstructor(parent);
}

InputMethodStatusIndicator::~InputMethodStatusIndicator()
{
    gInputMethodStatusIndicatorStub->InputMethodStatusIndicatorDestructor();
}


#endif
