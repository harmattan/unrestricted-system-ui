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
    virtual void changeNetWmState(const QWidget* w, bool set, Atom one, Atom two);
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

void StatusIndicatorMenuWindowStub::changeNetWmState(const QWidget* w, bool set, Atom one, Atom two)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<const QWidget*>(w));
    params.append(new Parameter<bool>(set));
    params.append(new Parameter<Atom>(one));
    params.append(new Parameter<Atom>(two));
    stubMethodEntered("changeNetWmState", params);
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

void StatusIndicatorMenuWindow::changeNetWmState(const QWidget* w, bool set, Atom one, Atom two)
{
    gStatusIndicatorMenuWindowStub->changeNetWmState(w, set, one, two);
}

#endif
