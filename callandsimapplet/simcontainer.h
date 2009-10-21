#ifndef SIMCONTAINER_H
#define SIMCONTAINER_H

#include <DuiContainer>
#include <QString>

class DuiLabel;
class DuiButton;
class QDBusInterface;

class SimContainer : public DuiContainer
{
    Q_OBJECT

public:
    SimContainer(DuiWidget *parent);
    virtual ~SimContainer();

signals:
    void valueChanged(bool);

public slots:
    void setPinRequest(bool enabled);

private slots:
    void buttonToggled(bool checked);
    void launchPinQuery();

private:
    void setLayout();

private:
    DuiLabel* pinRequestLabel;
    DuiButton* pinRequestButton;
    DuiButton* changePinButton;

    // DBus interface for pin query launching
    QDBusInterface *dbusIf;
};

#endif // SIMCONTAINER_H
