#include <QtGui>
#include "ViewWindow.h"
#include "SvgItemNode.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    
    ViewWindow *win = new ViewWindow();
    
    SvgItemNode node1, node2;
    node1.moveBy(200, 180);
    
    node2.moveBy(20, 18);
    
    /* Add nodes to the scene inside the view */
    win->getScene()->addItem(&node1);
    win->getScene()->addItem(&node2);
    QGraphicsTextItem text;
    text.setHtml("<b>Hello</b> <em>World</em>!");
    text.setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    win->getScene()->addItem(&text);
    
    win->show();
    
	
    return app.exec();
}
