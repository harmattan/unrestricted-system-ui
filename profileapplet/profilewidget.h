#ifndef PROFILEWIDGET_H
#define PROFILEWIDGET_H

#include "dcpwidget.h" 
#include "profilebuttons.h"


class ProfileDBusInterface;
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
    void setProfile(int profileId);
    void setVolume(int profileId, int level);
    void setVibration(int profileId, bool enabled);

    void profileSelected(ProfileButtons::ProfileId id);
    void sliderValueChanged(int index);
    void vibrationChanged(bool enabled);
    void orientationChanged(const Dui::Orientation &);

private: //methods
    DuiContainer* createContainer();

private: //attributes

    ProfileButtons* profileButtons;
    ProfileContainer* ringingContainer;
    ProfileContainer* loudContainer;
    ProfileContainer* beepContainer;
    ProfileContainer* silentContainer;

    ProfileDBusInterface *profileIf;
};
#endif // PROFILEWIDGET_H
