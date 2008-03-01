#ifndef VIEWWINDOW_H
#define VIEWWINDOW_H

#include <QGraphicsView>
#include <QDebug>

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

private slots:
	void notifySelection();

};
#endif
