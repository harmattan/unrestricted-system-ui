#include "shutdownui.h"

#include <DuiLabel>
#include <DuiSceneManager>

#include <QDebug>

ShutdownUI::ShutdownUI() :
        text(0)
{
    setFullscreen(true);
    setNavigationBarVisible(false);
    setPannableAreaInteractive(false);
}

ShutdownUI::~ShutdownUI()
{
}

void ShutdownUI::createContent()
{
    qDebug() << "ShutdownUI::createContent()";
    DuiApplicationPage::createContent();

    DuiWidget* panel = centralWidget();

    text = new DuiLabel("Ruikku", panel);
    text->setAlignment(Qt::AlignCenter);

    alignText();
}

void ShutdownUI::resizeEvent(QGraphicsSceneResizeEvent* event)
{
    qDebug() << "ShutdownUI::resizeEvent()";
    DuiApplicationPage::resizeEvent(event);

    alignText();
}

void ShutdownUI::alignText()
{
    qDebug() << "ShutdownUI::alignText()";
    QSize size = DuiSceneManager::instance()->visibleSceneSize();
    text->setGeometry(QRectF(0, 0, size.width(), size.height()));
}
