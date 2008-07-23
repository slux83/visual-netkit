#include <QApplication>
#include <QGraphicsItemGroup>
#include <QGraphicsItem>
#include <QLineF>
#include <QLinkedList>
#include "ViewWindow.h"
#include "SvgItemNode.h"
#include "SvgItemLink.h"
#include "Scene.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Scene *s = new Scene();
    ViewWindow *win = new ViewWindow(s);
	
    win->show();
    
	
    return app.exec();
}
