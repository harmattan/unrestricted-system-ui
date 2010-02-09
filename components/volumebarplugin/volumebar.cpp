#include "volumebar.h"
#include "volumebarlogic.h"

#undef DEBUG
#include "../debug.h"

#include <DuiStatusIndicatorMenuPluginInterface>
#include <DuiContainer>
#include <DuiSlider>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiTheme>

#include <QGraphicsLinearLayout>

#define SYSTEMUI_TRANSLATION "duicontrolpanel-systemui"

const QString cssDir = "/usr/share/duistatusindicatormenu/themes/style/";

VolumeBar::VolumeBar (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
                      QGraphicsItem *parent) :
        DuiWidget (parent),
        m_bar (NULL)
{
    DuiTheme::loadCSS (cssDir + "volumebarplugin.css");

    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);

    setLayout (mainLayout);
    mainLayout->setContentsMargins (0, 0, 0, 0);

    DuiContainer *container = new DuiContainer;
    QGraphicsLinearLayout *hbox =
        new QGraphicsLinearLayout (Qt::Horizontal);
    container->setLayout (hbox);

    //% "Volume"
    DuiLabel *label = new DuiLabel (qtTrId ("qtn_volume"));

    m_bar = new DuiSlider;
    m_bar->setMinLabelIconId ("icon-m-volume");
    m_bar->setMinLabelVisible (true);

    hbox->addItem (label, Qt::AlignLeft);
    hbox->addItem (m_bar, Qt::AlignRight);

    mainLayout->addItem (container);
}

VolumeBar::~VolumeBar ()
{
    //Free the resources here
}

