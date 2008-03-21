#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QDebug>
#include "SvgItemNode.h"
#include "SvgItemLink.h"

class QGraphicsSceneMouseEvent;
class QMenu;
class QPointF;
class QGraphicsLineItem;
//class QFont;
//class QGraphicsTextItem;
class QColor;

class Scene : public QGraphicsScene
{
	Q_OBJECT

public:
	Scene();
	virtual ~Scene();
	
	enum Mode { InsertItem, InsertLine, InsertText, MoveItem };
	QMenu *myItemMenu;
	Mode myMode;
	bool leftButtonDown;
	QPointF startPoint;
	QGraphicsLineItem *line;
	//QFont myFont;
	//DiagramTextItem *textItem;
	//QColor myTextColor;
	QColor myItemColor;
	QColor myLineColor;

private:
	bool isItemChange(int type);

public slots:
	void setMode(Mode mode);
	
signals:
     void vmInserted(SvgItemNode *item);
     void linkInserted(SvgItemLink *item);
     //void textInserted(QGraphicsTextItem *item);
     void itemSelected(QGraphicsItem *item);

protected:
     void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
     void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent);
     void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

};
#endif
