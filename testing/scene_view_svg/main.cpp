#include <QtGui>
#include "ViewWindow.h"
#include "SvgItemNode.h"
#include "Scene.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Scene *s = new Scene();
    ViewWindow *win = new ViewWindow(s);
    
    SvgItemNode node1, node2;
    node1.moveBy(200, 180);
    
    node2.moveBy(20, 18);
    
    /* Add nodes to the scene inside the view */
    s->addItem(&node1);
    s->addItem(&node2);
    QGraphicsTextItem text;
    text.setHtml("<b>Press Control to select<br />multiple objects!</b>");
    text.setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    text.moveBy(200, 100);
    
    s->addItem(&text);
    
    win->show();
    
	
    return app.exec();
}
