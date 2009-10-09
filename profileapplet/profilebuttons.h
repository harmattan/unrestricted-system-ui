#ifndef PROFILEBUTTONS_H
#define PROFILEBUTTONS_H

#include <DuiContainer>
#include <QHash>
class DuiButton;

class ProfileButtons : public DuiContainer
{
    Q_OBJECT

public:
    enum ProfileId {
        ringing = 0,
        silent,
        beep,
        loud,
        none // none must be last to teel how many profiles there are available
    };

    ProfileButtons(DuiWidget *parent=0);
    virtual ~ProfileButtons();

    ProfileId selectedProfile();
    QString selectedProfileName();

public slots:
    // returns false if selection won't change
    bool selectProfile(ProfileButtons::ProfileId id);

signals:
    void profileSelected(ProfileButtons::ProfileId id);

private slots:
    void buttonToggled(bool checked);

private:
    void createProfileButtons();
    void addButton(QString name, ProfileButtons::ProfileId id);
    void setLayout();

private:
    ProfileId selected;
    QHash<int, DuiButton*> buttons;
};

#endif // PROFILEBUTTONS_H
