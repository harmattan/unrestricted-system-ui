/* -*- Mode: C; indent-tabs-mode: s; c-basic-offset: 4; tab-width: 4 -*- */
/* vim:set et ai sw=4 ts=4 sts=4: tw=80 cino="(0,W2s,i2s,t0,l1,:0" */
#include "profilebuttons.h"

#include <QHash>
#include <DuiButton>
#include <DuiButtonGroup>
#include <DuiLayout>
#include <DuiLinearLayoutPolicy>
#include <DuiGridLayoutPolicy>
#include <QDebug>

#define DEBUG
#include "../debug.h"

static const int nButtonColumns = 2;

ProfileButtons::ProfileButtons (
        DuiWidget *parent) :
    DuiContainer (parent),
    m_Buttons (0)
{
}

ProfileButtons::~ProfileButtons()
{
}


void 
ProfileButtons::init (
        QMap<int, QString> data, 
        int currentId)
{
    m_Buttons = new DuiButtonGroup();
    connect (m_Buttons, SIGNAL(buttonClicked(int)), 
            this, SIGNAL(profileSelected(int)));

    QList<int> ids = data.keys();
    for (int i = 0; i < data.count(); ++i) {
        int id = ids.at(i);
        addButton (data.value(id), id, (currentId == id));
        qDebug() << Q_FUNC_INFO << id << "," << data.value(id);
    }
    setLayout();
}

void
ProfileButtons::addButton (
        QString   name, 
        int       id, 
        bool      checked)
{
    DuiButton *button = new DuiButton (name);
    button->setCheckable (true);
    button->setChecked (checked);
    m_Buttons->addButton (button, id);
}

/*!
 * \returns true if the button for the profile has been found.
 */
bool 
ProfileButtons::selectProfile (
        int id)
{
    bool retval = false;

    qDebug() << Q_FUNC_INFO << id;

    if (m_Buttons == NULL)
        return retval;

    QList<DuiButton*> buttonList = m_Buttons->buttons();
    for (int i = 0; i < buttonList.size(); ++i) {
        DuiButton *button = buttonList.at (i);
        bool       checked;
        
        checked = id == m_Buttons->id (button);
        button->setChecked (checked);
        if (checked)
            retval = true;
    }

    return retval;
}

/*!
 * \returns The translated UI string from the checked button or an empty string
 *   if there is no selected button.
 */
QString 
ProfileButtons::selectedProfileName ()
{
    if (m_Buttons == NULL)
        return QString ("");

    DuiButton *button = m_Buttons->checkedButton();
    if (button)
        return button->text();

    return QString("");
}

/*!
 * Creates the layout for the profile buttons. Takes the profile selection
 * buttons from m_Buttons and sets a portrait and a landscape layout for them.
 */
void 
ProfileButtons::setLayout ()
{
    DuiLayout              *layout;
    DuiLinearLayoutPolicy  *landscapePolicy;
    DuiGridLayoutPolicy    *portraitPolicy;
    QList<DuiButton*>       buttonList = m_Buttons->buttons();
    int                     col = 0;
    int                     row = 0;

    layout = new DuiLayout();

    landscapePolicy = new DuiLinearLayoutPolicy(layout, Qt::Horizontal);
    layout->setLandscapePolicy(landscapePolicy); // ownership transferred

    portraitPolicy = new DuiGridLayoutPolicy(layout);
    layout->setPortraitPolicy(portraitPolicy); // ownership transferred

    // Storing the buttons in the layout. 
    for (int i = 0; i < buttonList.size(); ++i) {
        SYS_DEBUG ("Adding button at %d, %d", col, row);
        landscapePolicy->addItem (buttonList[i]);
        portraitPolicy->addItem(buttonList[i], row, col);

        ++col;
        if (col >= nButtonColumns) {
            ++row;
            col = 0;
        }
    }

    centralWidget()->setLayout (layout);
}
