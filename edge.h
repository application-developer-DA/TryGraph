#ifndef EDGE_H
#define EDGE_H

#include <QGraphicsItem>
#include <QPen>

class GraphWidget;
class Node;

class Edge : public QGraphicsItem
{
public:
    Edge(GraphWidget* graphwidget, Node* sourceNode, Node* destinationNode, int costValue = 1);
    ~Edge();

    Node* getSourceNode() const;
    Node* getDestinationNode() const;


    bool isInTree;

    void adjust();
    void setCost(int costValue);
    int costLink() const;

    // Inherited from base class, need to be reimplemented
    enum { Type = UserType + 2 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    static const int lineWidth = 2;
    int cost;
    Node* source;
    Node* destination;

    QPointF sourcePoint;
    QPointF destinationPoint;

    GraphWidget* graph;

    QPen pen;
};

#endif // EDGE_H
