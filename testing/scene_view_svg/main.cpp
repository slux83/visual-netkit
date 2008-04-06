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
    
    SvgItemNode node2;
    SvgItemNode *vm1 = new SvgItemNode();
    SvgItemNode *vm2 = new SvgItemNode();
    
    
    vm1->moveBy(300, 250);
    vm2->moveBy(0, 190);
    

    vm1->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    vm2->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    
    SvgItemNode *node1 = new SvgItemNode();
    node1->moveBy(200, 300);
    
    node2.moveBy(200, 180);
    
    /* Add nodes to the scene inside the view */
    s->addItem(node1);
    s->addItem(&node2);
    //QGraphicsTextItem text;
    //text.setHtml("<b>Press Control to select<br />multiple objects!</b>");
    //text.setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    //text.moveBy(300, 200);
    
    //s->addItem(&text);
	s->addItem(vm1);
	s->addItem(vm2);
	
	/* Add link between two VMs */
	SvgItemLink *link12 = new SvgItemLink(vm2, node1);
	/*
	 * forse facendo un gruppo tra 2 vm e il link, il dragging è facilitato (automatico)
	QLinkedList<QGraphicsItem *> items;
	items.append(vm1);
	items.append(vm2);
	items.append(link12);
	win->makeGroupFromItems(items);
	*/
	s->addItem(link12);
	
    win->show();
    
	
    return app.exec();
}