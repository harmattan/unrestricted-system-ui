#include "profilebuttons.h"
#include "profiletranslation.h"

#include <QHash>
#include <DuiButton>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiGridLayoutPolicy>
#include <DuiStylableWidget>
#include <QDebug>

ProfileButtons::ProfileButtons(ProfileButtons::ProfileId selectedProfile, DuiWidget *parent) :
        DuiContainer(parent),
        selected(ProfileButtons::none)
{
    createProfileButtons(selectedProfile);
    setLayout();
}

ProfileButtons::~ProfileButtons()
{
}

void ProfileButtons::createProfileButtons(ProfileButtons::ProfileId selectedProfile)
{
    addButton(DcpProfile::RingingText, ProfileButtons::ringing);
    addButton(DcpProfile::SilentText, ProfileButtons::silent);
    addButton(DcpProfile::BeepText, ProfileButtons::beep);
    addButton(DcpProfile::LoudText, ProfileButtons::loud);
    selectProfile(selectedProfile);
}

void  ProfileButtons::addButton(QString name, ProfileButtons::ProfileId id)
{
    DuiButton* btn = new DuiButton(name, this);
    btn->setCheckable(true);
    connect(btn, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));
    buttons.insert(id, btn);
}

bool ProfileButtons::selectProfile(ProfileButtons::ProfileId id)
{
    qDebug() << "ProfileButtons::selectProfile:" << id << "vs." << selectedProfile();
    if(id != selected) {
        DuiButton* btn = buttons.value(selected);
        if(btn)
            btn->setChecked(false);
        selected = id;
        btn = buttons.value(id);
        if(btn)
            btn->setChecked(true);
        qDebug() << "ProfileButtons::selectProfile:" << selectedProfile();
        return true;
    }
    return false;
}

ProfileButtons::ProfileId ProfileButtons::selectedProfile()
{

    return selected;
}

void ProfileButtons::buttonToggled(bool checked)
{
    DuiButton* btn = static_cast<DuiButton*>(this->sender());
    DuiButton* old = buttons.value(selected);
    //qDebug() << "ProfileButtons::buttonTggled(" << checked << ") for" << (QObject*)btn << "(old:" << (QObject*)old << ")";

    if(old == btn)
    {
        // allow only checking, not unchecking
        if(!btn->isChecked()) {
            btn->setChecked(true);
        }
    }
    else
    {
        if(checked == true)
        {
            // only one profile at a time can be checked
            selected = (ProfileButtons::ProfileId)buttons.key(btn);
            if(old) {
                old->setChecked(false);
            }
            qDebug() << "ProfileButtons::buttonTggled() for" << selected;
            emit profileSelected(selected);
        }
    }
}

void ProfileButtons::setLayout()
{
    DuiLayout *layout = new DuiLayout();

    DuiLinearLayoutPolicy *landscapePolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    layout->setLandscapePolicy(landscapePolicy); // ownership transferred

    DuiGridLayoutPolicy *portraitPolicy = new DuiGridLayoutPolicy(layout);
    layout->setPortraitPolicy(portraitPolicy); // ownership transferred

    // set objects
    DuiButton* btn = buttons.value(ringing);
    landscapePolicy->addItem(btn);
    portraitPolicy->addItemAtPosition(btn, 0, 0);

    btn = buttons.value(silent);
    landscapePolicy->addItem(btn);
    portraitPolicy->addItemAtPosition(btn, 0, 1);

    btn = buttons.value(beep);
    landscapePolicy->addItem(btn);
    portraitPolicy->addItemAtPosition(btn, 1,0);

    btn = buttons.value(loud);
    landscapePolicy->addItem(btn);
    portraitPolicy->addItemAtPosition(btn, 1,1);

    landscapePolicy->setSpacing(5);
    portraitPolicy->setSpacing(5);

    DuiStylableWidget *layoutWidget = new DuiStylableWidget();
    layoutWidget->setLayout(layout);
//    layoutWidget->setObjectName("profileContainerLayout");

    setCentralWidget(layoutWidget);
}
