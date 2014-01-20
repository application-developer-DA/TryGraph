#pragma once

#include <QGraphicsItem>
#include <QPen>


class Edge : public QGraphicsItem
{
public:
    class IEdgeObserver
    {
    public:
        virtual bool isOriented() const = 0;
        virtual void removeEdge(uintptr_t id) = 0;
        virtual void displayCostDialog(uintptr_t id) = 0;
    };

    Edge(IEdgeObserver* observer, const QGraphicsItem* sourceNode, const QGraphicsItem* destinationNode);

    void adjust();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    static const int lineWidth = 2;

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    IEdgeObserver*  observer;

    const QGraphicsItem*     source;
    const QGraphicsItem*     destination;

    QPointF sourcePoint;
    QPointF destinationPoint;

    int  cost;
    QPen pen;
};

