#include "shutdownui.h"

#include <DuiLabel>
#include <DuiSceneManager>
#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiLocale>

#include <QDebug>

ShutdownUI::ShutdownUI() :
        text(0)
{
    setFullscreen(true);
    setComponentsDisplayMode(DuiApplicationPage::EscapeButton, DuiApplicationPageModel::Hide);
    setComponentsDisplayMode(DuiApplicationPage::NavigationBar, DuiApplicationPageModel::Hide);
    setComponentsDisplayMode(DuiApplicationPage::HomeButton, DuiApplicationPageModel::Hide);
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

    text = new DuiLabel(trid("qtn_shut_down", "Shutting down, good bye!"), panel);
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
    QSize size = DuiApplication::activeApplicationWindow()->sceneManager()->visibleSceneSize();
    text->setGeometry(QRectF(0, 0, size.width(), size.height()));
}
