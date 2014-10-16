#include "edge.h"

#include <cmath>

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>


static const double Pi = 3.14159265358979323846264338327950288419717;
static const double TwoPi = 2.0 * Pi;


Edge::Edge(IEdgeObserver* observer, const QGraphicsItem* sourceNode, const QGraphicsItem* destinationNode)
    : observer(observer)
    , source(sourceNode)
    , destination(destinationNode)
    , cost(-1)
    , pen(Qt::black, lineWidth, Qt::SolidLine, Qt::Roundcap, Qt::RoundJoin)
{
    // source->addEdge(this);
    // destination->addEdge(this);

    adjust();
}

void Edge::adjust()
{
    if (!source || !destination)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(destination, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if (length > qreal(20.)) {
        sourcePoint = line.p1();
        destinationPoint = line.p2();
    }
    else {
        sourcePoint = destinationPoint = line.p1();
    }
}

QRectF Edge::boundingRect() const
{
    if (!source || !destination)
        return QRectF();

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(destination, 0, 0));
    const qreal x1 = line.p1().x();
    const qreal x2 = line.p2().x();
    const qreal y1 = line.p1().y();
    const qreal y2 = line.p2().y();
    qreal lx = qMin(x1, x2);
    qreal rx = qMax(x1, x2);
    qreal ty = qMin(y1, y2);
    qreal by = qMax(y1, y2);
    return QRectF(lx, ty, rx - lx, by - ty);
}

QPainterPath Edge::shape() const
{
    QPainterPath path;

    path.moveTo(sourcePoint.toPoint());
    path.lineTo(destinationPoint.toPoint());

    // Code from QGrapthicsLineItem implemetation
    const qreal penWidthZero = qreal(0.00000001);

    QPainterPathStroker ps;
    ps.setCapStyle(pen.capStyle());
    ps.setWidth(pen.widthF() <= 0.0 ? penWidthZero : pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());

    QPainterPath p = ps.createStroke(path);
    p.addPath(path);

    return p;
}

void Edge::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    if (!source || !destination)
        return;

    QLineF line(sourcePoint, destinationPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    bool isInTree = false;

    painter->setPen(isInTree ? QPen(Qt::darkGreen, lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    // Get mid point of the line
    int x = line.x1() + line.x2();
    int y = line.y1() + line.y2();
    QPointF midpoint(x / 2., y / 2.);
    painter->drawText(midpoint, QString::number(cost));

    if(observer->isOriented()) {
        // Draw the arrows
        double angle = ::acos(line.dx() / line.length());
        if (line.dy() >= 0)
            angle = TwoPi - angle;
        double arrowSize = 15;

        QPointF destArrowP1 = destinationPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                                  cos(angle - Pi / 3) * arrowSize);
        QPointF destArrowP2 = destinationPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                                  cos(angle - Pi + Pi / 3) * arrowSize);

        painter->setBrush(Qt::yellow);
        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    }
}

void Edge::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    switch (event->button()) {
    case Qt::RightButton:
        observer->removeEdge((uintptr_t)this);
        break;
    case Qt::LeftButton:
        observer->displayCostDialog((uintptr_t)this);
        break;
    }

    update();
    QGraphicsItem::mousePressEvent(event);
}

void Edge::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

