#include "costdialog.h"
#include "edge.h"
#include "graphwidget.h"
#include "mainwindow.h"
#include "node.h"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOption>

#include <cmath>

static const double Pi = 3.14159265358979323846264338327950288419717;
static const double TwoPi = 2.0 * Pi;

Edge::Edge(GraphWidget *graphwidget, Node *sourceNode, Node *destinationNode, int costValue):
    graph(graphwidget),
    source(sourceNode),
    destination(destinationNode),
    cost(costValue),
    isInTree(false)
{
    source->addEdge(this);
    destination->addEdge(this);
    pen = QPen(Qt::black, lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    adjust();
}

Edge::~Edge()
{
    source->removeEdge(this);
    destination->removeEdge(this);
}

Node* Edge::getSourceNode() const
{
    return source;
}

Node* Edge::getDestinationNode() const
{
    return destination;
}

void Edge::adjust()
{
    if(!source || !destination)
        return;

    QLineF line(mapFromItem(source, 0, 0), mapFromItem(destination, 0, 0));
    qreal length = line.length();

    prepareGeometryChange();

    if(length > qreal(20.))
    {
        sourcePoint = line.p1();
        destinationPoint = line.p2();
    }
    else
        sourcePoint = destinationPoint = line.p1();
}

void Edge::setCost(int costValue)
{
    cost = costValue;
}

int Edge::costLink() const
{
    return cost;
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
    if (pen.widthF() <= 0.0)
        ps.setWidth(penWidthZero);
    else
        ps.setWidth(pen.widthF());
    ps.setJoinStyle(pen.joinStyle());
    ps.setMiterLimit(pen.miterLimit());
    QPainterPath p = ps.createStroke(path);
    p.addPath(path);
    return p;
}

void Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!source || !destination)
        return;

    QLineF line(sourcePoint, destinationPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    // Draw the line itself
    if(!isInTree)
        painter->setPen(pen);
    else
    {
        QPen mypen = QPen(Qt::darkGreen, lineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        painter->setPen(mypen);
    }
    painter->drawLine(line);

    // Getting mid point of the line
    int x = line.x1() + line.x2();
    int y = line.y1() + line.y2();
    QPointF midpoint(x / 2, y / 2);
    painter->drawText(midpoint, QString::number(cost));

    if(graph->isOriented)
    {
        // Draw the arrows
        double angle = ::acos(line.dx() / line.length());
        if (line.dy() >= 0)
            angle = TwoPi - angle;
        double arrowSize = 15;

//        double l = sqrt(line.dx()*line.dx() + line.dy()*line.dy());
//        double divdxl = line.dx() / l;
//        double divdxy = line.dy() / l;
        /*
        QPointF sourceArrowP1 = sourcePoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                      cos(angle + Pi / 3) * arrowSize);
        QPointF sourceArrowP2 = sourcePoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                      cos(angle + Pi - Pi / 3) * arrowSize);*/

        QPointF destArrowP1 = destinationPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                                  cos(angle - Pi / 3) * arrowSize);
        QPointF destArrowP2 = destinationPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                                  cos(angle - Pi + Pi / 3) * arrowSize);

        painter->setBrush(Qt::yellow);
        //painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
        painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);
    }
}

void Edge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        // remove edge and liks to it
        graph->removeEdge(this);
    }
    else
        if(event->button() == Qt::LeftButton)
        {
//            QLineEdit* lineedit = new QLineEdit(graph);
//            // Getting mid point of the line
//            QPointF midpoint(mapToScene(event->pos()));
//            lineedit->pos().setX(midpoint.x());
//            lineedit->pos().setY(midpoint.y());
//            lineedit->show();
            CostDialog* cd = new CostDialog(graph, this);
            cd->show();
        }
        else
        {
            update();
            QGraphicsItem::mousePressEvent(event);
        }
}

void Edge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

