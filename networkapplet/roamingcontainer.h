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
    RoamingContainer(DuiWidget *parent = 0);
    virtual ~RoamingContainer();
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
    QGraphicsWidget *roamingUpdatesWidget;
    DuiButton *roamingButton;
    DuiButton *roamingUpdatesButton;    
    DuiLinearLayoutPolicy *landscapeLayoutPolicy;
    DuiLinearLayoutPolicy *portraitLayoutPolicy;    

};

#endif // ROAMINGCONTAINER_H
