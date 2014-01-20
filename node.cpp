#include "node.h"
#include "mainwindow.h"

#include <limits>

#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneMouseEvent>


Node::Node(INodeObserver* observer, int x, int y)
    : observer(observer)
    , shortestPath(std::numeric_limits<int>::max())
{
    setPos(x, y);

    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setAcceptHoverEvents(true);
}

void Node::addEdge(Edge& edge)
{
    edgesList.push_back(edge);
    edge.adjust();
}

void Node::removeEdge(Edge& edge)
{
    edgesList.remove(edge);
}

void Node::removeConnections()
{
    edgesList.clear();
}

std::list<uintptr_t> Node::edgeIdentifiers() const
{
    std::list<uintptr_t> ids;
    for (auto it = begin(edgesList); it != end(edgesList); ++it)
        ids.push_back((uintptr_t)(*it));
    return ids;
}

QRectF Node::boundingRect() const
{
    return QRectF(-ellipseSideWidth/2, -ellipseSideWidth/2, ellipseSideWidth, ellipseSideWidth);
}

QPainterPath Node::shape() const
{
    QPainterPath path;
    path.addEllipse(-ellipseSideWidth/2, -ellipseSideWidth/2, ellipseSideWidth, ellipseSideWidth);
    return path;
}

void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget*)
{
    QRadialGradient gradient(-3, -3, 10);
    if (option->state & QStyle::State_Sunken) {
        // Change color if selected
        gradient.setColorAt(0, QColor(Qt::black).light(120));
        gradient.setColorAt(1, QColor(Qt::darkGray).light(120));
    } else {
        gradient.setColorAt(0, Qt::darkGray);
        gradient.setColorAt(1, Qt::black);
    }

    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));

    painter->drawEllipse(-ellipseSideWidth/2, -ellipseSideWidth/2, ellipseSideWidth, ellipseSideWidth);
}

QVariant Node::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
    switch(change) {
    case ItemPositionHasChanged:
        foreach(Edge& edge, edgesList)
            edge.adjust();
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    switch (event->button()) {
    case Qt::RightButton:
        observer->removeNode((uintptr_t)this);
        break;
    case Qt::LeftButton:
        observer->nodeClicked((uintptr_t)this);
        break;
    }

    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

