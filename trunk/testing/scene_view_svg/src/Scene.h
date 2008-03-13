#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QDebug>

class Scene : public QGraphicsScene
{
	Q_OBJECT

public:
	Scene();
	virtual ~Scene();

protected:
	void mousePressEvent  (QGraphicsSceneMouseEvent *mouseEvent);
	void mouseMoveEvent   (QGraphicsSceneMouseEvent *mouseEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
};
#endif
