#ifndef STATUSAREAWINDOW_STUB_H
#define STATUSAREAWINDOW_STUB_H

#include <stubbase.h>
#include <statusarea.h>
#include <statusareawindow.h>

class StatusAreaWindowStub : public StubBase
{
public:
    virtual void StatusAreaWindowConstructor();
    virtual void StatusAreaWindowDestructor();
    virtual StatusArea *statusArea() const;
    virtual void rotate(const Dui::Orientation& orientation);
};

void StatusAreaWindowStub::StatusAreaWindowConstructor()
{
    stubMethodEntered("StatusAreaWindowConstructor");
}

void StatusAreaWindowStub::StatusAreaWindowDestructor()
{
    stubMethodEntered("StatusAreaWindowDestructor");
}

StatusArea * StatusAreaWindowStub::statusArea() const
{
    stubMethodEntered("statusArea");
    return stubReturnValue<StatusArea *>("statusArea");
}

void StatusAreaWindowStub::rotate(const Dui::Orientation& orientation)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<Dui::Orientation>(orientation));
    stubMethodEntered("rotate", params);
}

StatusAreaWindowStub gDefaultStatusAreaWindowStub;
StatusAreaWindowStub *gStatusAreaWindowStub = &gDefaultStatusAreaWindowStub;

StatusAreaWindow::StatusAreaWindow(QWidget *parent) :
    DuiWindow(parent),
    scene(0),
    statusArea_(0)
{
    gStatusAreaWindowStub->StatusAreaWindowConstructor();
}

StatusAreaWindow::~StatusAreaWindow()
{
    gStatusAreaWindowStub->StatusAreaWindowDestructor();
}

StatusArea *StatusAreaWindow::statusArea() const
{
    return gStatusAreaWindowStub->statusArea();
}

void StatusAreaWindow::rotate(const Dui::Orientation &orientation)
{
    gStatusAreaWindowStub->rotate(orientation);
}

#endif
