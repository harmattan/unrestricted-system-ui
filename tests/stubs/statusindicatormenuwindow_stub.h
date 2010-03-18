#ifndef STATUSINDICATORMENUWINDOW_STUB_H
#define STATUSINDICATORMENUWINDOW_STUB_H

#include <stubbase.h>
#include <statusindicatormenuwindow.h>

class StatusIndicatorMenuWindowStub : public StubBase
{
public:
    virtual void StatusIndicatorMenuWindowConstructor();
    virtual void StatusIndicatorMenuWindowDestructor();
    virtual void excludeFromTaskBar();
};

void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowConstructor()
{
    stubMethodEntered("StatusIndicatorMenuWindowConstructor");
}

void StatusIndicatorMenuWindowStub::StatusIndicatorMenuWindowDestructor()
{
    stubMethodEntered("StatusIndicatorMenuWindowDestructor");
}

void StatusIndicatorMenuWindowStub::excludeFromTaskBar()
{
    stubMethodEntered("excludeFromTaskBar");
}


StatusIndicatorMenuWindowStub gDefaultStatusIndicatorMenuWindowStub;
StatusIndicatorMenuWindowStub *gStatusIndicatorMenuWindowStub = &gDefaultStatusIndicatorMenuWindowStub;


StatusIndicatorMenuWindow::StatusIndicatorMenuWindow(QWidget *parent) :
    DuiWindow(parent),
    applicationPage(0),
    escapeButtonPanel(0)
{
    gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowConstructor();
}

StatusIndicatorMenuWindow::~StatusIndicatorMenuWindow()
{
    gStatusIndicatorMenuWindowStub->StatusIndicatorMenuWindowDestructor();
}

void StatusIndicatorMenuWindow::excludeFromTaskBar()
{
    gStatusIndicatorMenuWindowStub->excludeFromTaskBar();
}


#endif
