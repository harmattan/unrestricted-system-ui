#include "profilebuttons.h"
#include "profiletranslation.h"

#include <QHash>
#include <DuiButton>
#include <DuiButtonGroup>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiGridLayoutPolicy>
#include <QDebug>

ProfileButtons::ProfileButtons(DuiWidget *parent) :
        DuiContainer(parent),
        buttons(NULL)        
{        
}

ProfileButtons::~ProfileButtons()
{
    delete buttons;
    buttons = NULL;
}

void ProfileButtons::init(QMap<int, QString> data, int currentId)
{
    buttons = new DuiButtonGroup();    
    connect(buttons, SIGNAL(buttonClicked(int)), this, SIGNAL(profileSelected(int)));
    QList<int> ids = data.keys();
    for(int i = 0; i < data.count(); ++i)
    {
        int id = ids.at(i);
        addButton(data.value(id), id, (currentId == id));
        qDebug() << Q_FUNC_INFO << id << "," << data.value(id);
    }    
    setLayout();
}

void  ProfileButtons::addButton(QString name, int id, bool checked)
{
    DuiButton* button = new DuiButton(name);
    button->setCheckable(true);
    button->setChecked(checked);
    buttons->addButton(button, id);    
}

bool ProfileButtons::selectProfile(int id)
{
    qDebug() << Q_FUNC_INFO << id;
    if(buttons == NULL)
        return false;

    QList<DuiButton*> buttonList = buttons->buttons();
    for(int i=0; i<buttonList.size(); ++i) {
        DuiButton *button = buttonList.at(i);
        if(id == buttons->id(button))
            button->setChecked(true);
        else
            button->setChecked(false);
    }
    return true;
}

QString ProfileButtons::selectedProfileName()
{
    if(buttons == NULL)
        return QString("");

    DuiButton* button = buttons->checkedButton();
    if(button)
        return button->text();
    return QString("");
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
    QList<DuiButton*> buttonList = buttons->buttons();    
    for(int i = 0; i < buttonList.size(); ++i)
    {
        qDebug() << Q_FUNC_INFO << "row:" << row << "col:" << col;
        DuiButton* button = buttonList.at(i);
        landscapePolicy->addItem(button);
        portraitPolicy->addItemAtPosition(button, row, col);
        ++col;
        if(1 < col){
            ++row;
            col=0;
        }
    }

    landscapePolicy->setSpacing(5);
    portraitPolicy->setSpacing(5);

    centralWidget()->setLayout(layout);

}
