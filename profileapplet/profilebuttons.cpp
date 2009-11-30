#include "profilebuttons.h"
#include "profiletranslation.h"

#include <QHash>
#include <DuiButton>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiGridLayoutPolicy>
#include <DuiStylableWidget>
#include <QDebug>

ProfileButtons::ProfileButtons(DuiWidget *parent) :
        DuiContainer(parent),
        selected(-1)
{
    createProfileButtons();
    setLayout();
}

ProfileButtons::~ProfileButtons()
{
}

void ProfileButtons::init(QMap<int, QString> data)
{
    QList<int> ids = data.keys();
    for(int i = 0; i < data.count(); ++i)
    {
        int id = ids.at(i);
        addButton(data.value(id), id);
        qDebug() << Q_FUNC_INFO << id << "," << data.value(id);
    }
    toggleSelected();
    setLayout();
}

void ProfileButtons::createProfileButtons()
{
}

void  ProfileButtons::addButton(QString name, int id)
{
    DuiButton* btn = new DuiButton(name, this);
    btn->setCheckable(true);
    connect(btn, SIGNAL(toggled(bool)), this, SLOT(buttonToggled(bool)));
    buttons.insert(id, btn);
}

void ProfileButtons::toggleSelected()
{
    qDebug() << Q_FUNC_INFO << selectedProfile();
    if(0 <= selected) {
        DuiButton* btn = buttons.value(selected);
        if(btn)
            btn->setChecked(true);
        qDebug() << Q_FUNC_INFO << selectedProfile();
    }
}

bool ProfileButtons::selectProfile(int id)
{
    qDebug() << Q_FUNC_INFO << id << "vs." << selectedProfile();
    if(id != selected) {
        DuiButton* btn = buttons.value(selected);
        if(btn)
            btn->setChecked(false);
        selected = id;
        btn = buttons.value(id);
        if(btn)
            btn->setChecked(true);
        qDebug() << Q_FUNC_INFO << selectedProfile();
        return true;
    }
    return false;
}

int ProfileButtons::selectedProfile()
{
    return selected;
}

QString ProfileButtons::selectedProfileName()
{
    DuiButton* btn = buttons.value(selected);
    if(btn)
        return btn->text();
    return QString("");
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
            selected = (int)buttons.key(btn);
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
    int col = 0;
    int row = 0;
    for(int i = 0; i < buttons.count(); ++i)
    {
        qDebug() << Q_FUNC_INFO << "row:" << row << "col:" << col;
        DuiButton* btn = buttons.value(i);
        landscapePolicy->addItem(btn);
        portraitPolicy->addItemAtPosition(btn, row, col);
        ++col;
        if(1 < col){
            ++row;
            col=0;
        }
    }
    landscapePolicy->setSpacing(5);
    portraitPolicy->setSpacing(5);

    DuiStylableWidget *layoutWidget = new DuiStylableWidget();
    layoutWidget->setLayout(layout);

    setCentralWidget(layoutWidget);
}
