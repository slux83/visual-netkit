#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QGraphicsView>
#include <QDebug>
#include <QKeyEvent>

#include "Scene.h"

class ViewWindow : public QGraphicsView
{
	Q_OBJECT
	
public:
	ViewWindow();
	virtual ~ViewWindow();
	Scene * getScene() { return scene; }

private:
	Scene *scene;
	void scaleView(qreal scaleFactor);

	
protected:
    void keyPressEvent(QKeyEvent *event);

};
#endif
