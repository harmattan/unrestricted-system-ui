#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include "dcpwidget.h" 
#include "profilebuttons.h"
#include "profiledatainterface.h"

class DuiButton;
class DuiContainer;
class DuiGridLayoutPolicy;
class DuiSlider;
class DuiWidgetController;
class ProfileContainer;

class ProfileWidget : public DcpWidget
{
    Q_OBJECT
		
public:
    ProfileWidget(QGraphicsWidget *parent = 0);
    virtual ~ProfileWidget();

    QString currentProfile();

protected:
    void initWidget();

private slots:
    void initProfiles();
    void setProfile(int profileId);
    void setVolume(int profileId, int level);
    void setVibration(int profileId, bool enabled);

    void profileSelected(int id);
    void sliderValueChanged(int index);
    void vibrationChanged(bool enabled);

private: //methods
    DuiContainer* createContainer();

private: //attributes
    ProfileButtons* profileButtons;
    QHash<int, ProfileContainer*> containers;

    ProfileDataInterface *profileIf;
};
#endif // PROFILEWIDGET_H
