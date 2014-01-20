#pragma once

#include <QGraphicsItem>
#include <QPen>

class Node;

class Edge : public QGraphicsItem
{
public:
    class IEdgeObserver
    {
        virtual bool isOriented() const = 0;
        virtual void removeEdge(uintptr_t id) = 0;
        virtual void displayCostDialog(uintptr_t id) = 0;
    };

    Edge(IEdgeObserver* observer, const Node* sourceNode, const Node* destinationNode, int costValue = 1);

    const Node* sourceNode() const         { return source; }
    const Node* destinationNode() const    { return destination; }

    int cost() const                 { return cost; }
    void setCost(int costValue)      { cost = costValue; }

    void adjust();

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

private:
    static const int lineWidth = 2;

    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    IEdgeObserver*  observer;

    int             cost;
    const Node*     source;
    const Node*     destination;

    QPointF sourcePoint;
    QPointF destinationPoint;

    QPen pen;
};

