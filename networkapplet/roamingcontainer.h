#ifndef ROAMINGCONTAINER_H
#define ROAMINGCONTAINER_H

#include <DuiContainer>

class DuiButton;
class DuiLabel;
class DuiLayout;
class DuiLinearLayoutPolicy;


class RoamingContainer : public DuiContainer
{
    Q_OBJECT

public:
    RoamingContainer(DuiWidget *parent);
    virtual ~RoamingContainer();

public slots:    
    void initRoamingButton(bool value);
    void initRoamingUpdatesButton(bool value);

private slots:
    void toggleRoamingUpdates(bool toggle);

signals:
    void roamingToggled(bool);
    void roamingUpdatesToggled(bool);

private: //methods
    void setLayout();    

private: //attributes
    DuiButton *roamingButton;
    DuiButton *roamingUpdatesButton;
    DuiLabel *roamingUpdatesLabel;        

};

#endif // ROAMINGCONTAINER_H
