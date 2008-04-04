#ifndef SVGITEMLINK_H
#define SVGITEMLINK_H

#include <QGraphicsLineItem>
#include <QGraphicsItem>
#include <QRectF>
#include <QPainterPath>
#include <QObject>

class SvgItemNode;
class QColor;

class SvgItemLink :  public QObject, public QGraphicsLineItem
{
	Q_OBJECT	
	
public:
	 SvgItemLink(SvgItemNode *sourceNode, SvgItemNode *destNode,
			 QGraphicsItem *parent = 0, QGraphicsScene *scene = 0);
	 ~SvgItemLink();
	 
	 
	QRectF boundingRect() const;
	QPainterPath shape() const;
	 
	 void setSourceNode(SvgItemNode *node);
	 void setDestNode(SvgItemNode *node);

     void setColor(const QColor &color) { myColor = color; }
     SvgItemNode *startNode() const { return myStartNode; }
     SvgItemNode *endNode() const { return myEndNode; }
     
	 void adjust();
	 QGraphicsLineItem* draw();

public slots:
     void updatePosition();

//protected:
//     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
//               QWidget *widget = 0);
private:
	 SvgItemNode *myStartNode;
	 SvgItemNode *myEndNode;
     QColor myColor;

     QPointF sourcePoint;
     QPointF destPoint;

};
#endif
