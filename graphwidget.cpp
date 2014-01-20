#include "edge.h"
#include "graphwidget.h"
#include "node.h"

#include <QGraphicsView>
#include <QtGui>

GraphWidget::GraphWidget(QWidget* parent)
    : QGraphicsView(parent)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
}

void GraphWidget::mousePressEvent(QMouseEvent* event)
{
    if(event->button() == Qt::MiddleButton)
        addVertex(event->x(), event->y());

    update();
    QGraphicsView::mousePressEvent(event);
}

void GraphWidget::keyPressEvent(QKeyEvent* event)
{
    if(event->key() == Qt::Key_Plus)
        scale(2.0, 2.0);
    else if(event->key() == Qt::Key_Minus)
        scale(0.9, 0.9);
    QGraphicsView::keyPressEvent(event);
}

void GraphWidget::addVertex(int x, int y)
{
    Node node(this, x, y);
    scene()->addItem(node);
    node.setPos(mapToScene(x, y));

    Q_ASSERT(vertices.find(&node) == vertices.end());
    vertices.insert(std::make_pair(&node, node));
}

