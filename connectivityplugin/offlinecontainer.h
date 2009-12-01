#ifndef OFFLINECONTAINER_H
#define OFFLINECONTAINER_H

#include <DuiContainer>

class DuiButton;
class DuiLabel;

class OfflineContainer : public DuiContainer
{
   Q_OBJECT

public:
    OfflineContainer(bool state, DuiWidget *parent = 0);
    virtual ~OfflineContainer();

    bool offlineMode();

signals:
    void toggled(bool);

private slots:
    void offlineButtonClicked();

private:
    void setLayout();

private:
    DuiButton *offlineButton;
};

#endif // OFFLINECONTAINER_H
