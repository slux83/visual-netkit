/**
 * VisualNetkit is an advanced graphical tool for NetKit <http://www.netkit.org>
 * Copyright (C) 2008  Alessio Di Fazio, Paolo Minasi
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


#include <QFont>

#include <QtDebug>

#include "LabScene.h"
#include "handles/LabHandler.h"
#include "handles/VmMapper.h"
#include "CollisionDomainItem.h"

/**
 * Constructor: the scene size is Normal = 1000x1000
 */
LabScene::LabScene() : QGraphicsScene(0, 0, 1000, 1000)
{
	labHandler = LabHandler::getInstance();
	line = NULL;
	selectionRect = new QGraphicsRectItem();
	border = new QGraphicsRectItem();
	
	//the pen & flags
	QPen pen(Qt::green, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
	border->setFlags(QGraphicsItem::ItemClipsToShape);
	border->setPen(pen);
	//set the rect
	border->setRect(sceneRect());
	border->setFlags(QGraphicsItem::ItemClipsToShape);
	border->setZValue(100);
	
	/* Adde the border to this scene */
	addItem(border);
	
	connect(this, SIGNAL(sceneRectChanged(QRectF)), this, SLOT(adjustSceneBorder(QRectF)));
}

/**
 * Deconstructor
 */
LabScene::~LabScene()
{
}


/**
 * [PRIVATE-SLOT]
 * Adjust the border of this scene when the user resize the scene rect
 */
void LabScene::adjustSceneBorder(QRectF r)
{
	border->setRect(r);
}


/**
 * Mouse press event
 */
void LabScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
	/* The user want to add an element? */
	if(mouseEvent->button() == Qt::LeftButton)
	{
		if (labHandler->getMainWindow()->actionAddVirtualMachine->isChecked()) {
			qDebug() << "Adding a new wirtual machine.";
			VmMapper::getInstance()->showAddVmForm();
		}
		
		//se ho selezionato lo strumento "Add Collision Domain"
		if (labHandler->getMainWindow()->actionAddCollisionDomain->isChecked()) {
			//preparo la linea e la aggiungo alla scena
			line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
            line->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            addItem(line);
		}
		
		//se non ho selezionato nessuno strumento
		if (labHandler->getMainWindow()->actionManageGraph->isChecked()) {
			/*
			//quando il cursore non è su alcun oggetto della scena,
			if ( (itemAt(mouseEvent->scenePos()))->type() != VirtualMachineItem::Type ) {
				//se lo trascino col tasto sinistro premuto,
				//disegno l'area di "selezionamento" e la aggiungo alla scena
				
				//selectionRect = new QGraphicsRectItem(QRectF(mouseEvent->scenePos(), mouseEvent->scenePos()));
				
				QRectF rect(mouseEvent->scenePos(), mouseEvent->scenePos());
				qDebug() << "selectionRect" << selectionRect << rect;
				selectionRect->setRect(rect);
				qDebug() << "Valore rect inizializzata = " << selectionRect;
				selectionRect->setPen(QPen(Qt::black, 1, Qt::DashLine, Qt::SquareCap, Qt::RoundJoin));
	        	addItem(selectionRect);
	        	
			}
			*/

		    QGraphicsScene::mousePressEvent(mouseEvent);
		}
	}
}

/**
 * Mouse move event
 */
void LabScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (labHandler->getMainWindow()->actionAddCollisionDomain->isChecked() && line != NULL) {
        QLineF newLine(line->line().p1(), mouseEvent->scenePos());
        line->setLine(newLine);
    } else if (labHandler->getMainWindow()->actionManageGraph->isChecked()) {
    	/*
    	if (mouseEvent->button() == Qt::LeftButton) {
	    	//aggiorno le dimensioni dell'area di "selezionamento"
	    	QRectF newRect(selectionRect->rect().topLeft(), mouseEvent->scenePos());
	    	selectionRect->setRect(newRect);
    		//selectionRect->setRect(QRectF(selectionRect->rect().topLeft(), mouseEvent->scenePos()));
    	}
    	*/
    }
    
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

/**
 * Mouse release event
 */
void LabScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (line != NULL && labHandler->getMainWindow()->actionAddCollisionDomain->isChecked()) {
        QList<QGraphicsItem *> startItems = items(line->line().p1());
        if (startItems.count() && startItems.first() == line)
            startItems.removeFirst();
        
        QList<QGraphicsItem *> endItems = items(line->line().p2());
        if (endItems.count() && endItems.first() == line)
            endItems.removeFirst();

        removeItem(line);
        delete line;

        if (startItems.count() > 0 && endItems.count() > 0 &&
            startItems.first()->type() == VirtualMachineItem::Type &&
            endItems.first()->type() == VirtualMachineItem::Type &&
            startItems.first() != endItems.first() 
            
            /*--------------------  TO  DO  -----------------------*/
            //controllo che le 2 VM abbiano almeno una interfaccia libera per la connessione
            // && startItems.first().hasFreeInterfaces() &&
            /*endItems.first().hasFreeInterfaces()*/) {
        	
        	//trovo le due VM da collegare
        	VirtualMachineItem *startItem = qgraphicsitem_cast<VirtualMachineItem *>(startItems.first());
            VirtualMachineItem *endItem = qgraphicsitem_cast<VirtualMachineItem *>(endItems.first());
            
            //creo l'elemento CollisionDomainItem e lo inizializzo
            CollisionDomainItem *cd = new CollisionDomainItem(new QString("X"));

            //creo i link tra la prima VM e il CollisionDomainItem 
            //e tra il CollisionDomainItem e la seconda VM
            SvgItemLink *link1 = new SvgItemLink(startItem, cd);
            SvgItemLink *link2 = new SvgItemLink(cd, endItem);
            
            QPointF *p = new QPointF( (link1->pos().x() + link2->pos().x())/2, 
            						  (link1->pos().y() + link2->pos().y())/2 );
            cd->setStartPoint(p);
            cd->updatePosition(p);
            
            
            startItem->addLink(link1);		//associo il link1 alla prima VM
            endItem->addLink(link2);		//associo il link2 alla seconda VM
            cd->addLink(link1);				//associo il link1 al CollisionDomainItem
            cd->addLink(link2);				//associo il link2 al CollisionDomainItem
            
            cd->setZValue(1000.0);
            link1->setZValue(500.0);
            link2->setZValue(500.0);
            
            //aggiorno le posizioni
            link1->updatePosition();
            link2->updatePosition();
            
            //creo l'etichetta e l'associo al link
            //trovo il punto centrale del link
            QPointF start = startItems.first()->pos();
            QPointF end = endItems.first()->pos();
            QPointF center;
            center.setX((end.x() + start.x())/2);
            center.setY((end.y() + start.y())/2);
            
            //inizializzo l'etichetta
            LabelItem *ipLabel1 = new LabelItem(link1, this);
            //LabelItem *ipLabel2 = new LabelItem(link2, this);
            
            
            ipLabel1->setPlainText(QString("LINK1"));
            //ipLabel2->setPlainText( QString(qgraphicsitem_cast<VirtualMachineItem *>(link2->startNode())->type()) );
            QFont font("Courier", 12, QFont::Bold, false);
            ipLabel1->setFont(font);
            //ipLabel2->setFont(font);
            ipLabel1->setPos(center);
            //ipLabel2->setPos(center);
            link1->addLabel(ipLabel1);
            //link2->addLabel(ipLabel2);
            
            //aggiungo gli elementi alla scena
            addItem(link1);
            addItem(link2);
            addItem(cd);
            addItem(ipLabel1);
            //addItem(ipLabel2);
            
            //test per il paint del CollisionDomainItem
            //CollisionDomainItem *cd = new CollisionDomainItem(new QString("C"), link, link);
            //addItem(cd);
        }
    }
    line = NULL;
    
    /*
            
    if (selectionRect != NULL && labHandler->getMainWindow()->actionManageGraph->isChecked()) {
    	QPainterPath path;
    	path.addRect(selectionRect->rect());
    	setSelectionArea(path);
    	removeItem(selectionRect);
    	delete selectionRect;
    }
    selectionRect = NULL;
    */
    
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}


bool LabScene::isItemChange(int type)
{    
	foreach (QGraphicsItem *item, selectedItems()) {
        if (item->type() == type)
            return true;
    }
    return false;
}



//===== METODI DI UTILITA' =====

/*
void LabScene::dumpToPDF() {
	QPrinter *printer = new QPrinter(QPrinter::HighResolution);
	printer->setOutputFormat(QPrinter::PdfFormat);
	QString fileName = QFileDialog::getSaveFileName(this, tr("Export to PDF"), "~/untitled.pdf", tr("File (*.pdf, *.ps)"));
	printer->setOutputFileName(fileName);
	QPainter *pdfPainter = new QPainter(printer);
	scene()->render(pdfPainter);
	pdfPainter->end();
}

void LabScene::dumpToSVG() {
	QSvgGenerator *gen = new QSvgGenerator();
	gen->setFileName("~/out.svg");
	QPainter *svgPainter = new QPainter(gen);
	scene()->render(svgPainter);
	svgPainter->end();
}
*/

