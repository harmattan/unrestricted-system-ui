#include "volumebar.h"
#include "volumebarlogic.h"

#define DEBUG
#include "../debug.h"

#include <DuiStatusIndicatorMenuPluginInterface>
#include <DuiLinearLayoutPolicy>
#include <DuiContainer>
#include <DuiSlider>
#include <DuiLabel>
#include <DuiLayout>
#include <DuiLocale>
#include <DuiTheme>

#include <QGraphicsLinearLayout>

#define SYSTEMUI_TRANSLATION "duicontrolpanel-systemui"

// const QString cssDir = "/usr/share/duistatusindicatormenu/themes/style/";

VolumeBar::VolumeBar (DuiStatusIndicatorMenuInterface &statusIndicatorMenu,
                      QGraphicsItem *parent) :
        DuiWidget (parent),
        m_bar (0),
        m_logic (0)
{
    Q_UNUSED(statusIndicatorMenu)

// Comment out when its needed (and .css to debian/*volume*.install file)
//    DuiTheme::loadCSS (cssDir + "volumebarplugin.css");

    m_logic = new VolumeBarLogic;

    QGraphicsLinearLayout *mainLayout =
        new QGraphicsLinearLayout (Qt::Vertical);

    setLayout (mainLayout);
    mainLayout->setContentsMargins (0, 0, 0, 0);

    DuiContainer *container = new DuiContainer;

    container->setHeaderVisible (false);

    DuiLayout *layout = new DuiLayout;
    DuiLinearLayoutPolicy *hbox =
        new DuiLinearLayoutPolicy (layout, Qt::Horizontal);
    container->setLayout (layout);

    //% "Volume"
    DuiLabel *label = new DuiLabel (qtTrId ("qtn_volume"));

    m_bar = new DuiSlider;
    m_bar->setMinLabelVisible (true);
    m_bar->setRange (0, 100);

    hbox->addItem (label, Qt::AlignLeft);
    hbox->addItem (m_bar, Qt::AlignRight);

    mainLayout->addItem (container);

    connect (m_bar, SIGNAL (valueChanged (int)),
             this, SLOT (volumeChanged (int)));

    int current_volume;

    current_volume = (int) (100.0 * m_logic->getVolume ());

    if (current_volume > 0)
        m_bar->setMinLabelIconID ("icon-m-volume");
    else
        m_bar->setMinLabelIconID ("icon-m-volume-off");

    m_bar->setValue (current_volume);

}

VolumeBar::~VolumeBar ()
{
    //Free the resources here
    delete m_logic;
}

void
VolumeBar::volumeChanged (int val)
{
    if (val > 0)
        m_bar->setMinLabelIconID ("icon-m-volume");
    else
        m_bar->setMinLabelIconID ("icon-m-volume-off");

    m_logic->setVolume ((double) val / 100.0);
}


