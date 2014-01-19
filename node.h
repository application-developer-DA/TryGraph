#pragma once

#include <QGraphicsItem>
#include <list>

class Edge;
class GraphWidget;

class Node : public QGraphicsItem
{
public:
    Node(GraphWidget *graphwidget, qreal x = 0, qreal y = 0, int _id = -1);
    ~Node();

    int id;
    int shortestPath;

    void addEdge(Edge* edge);
    void removeEdge(Edge* edge);
    void removeConnections();
    void setId(int _id);
    std::list<Edge*> getEdges() const;

    Edge* isDirectlyConnected(Node* firstNode, Node* secondNode);

    // Inherited from base class, need to be reimplemented
    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    static void setUIDcounter(int value) { Node::uniqueId = value; }

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    static const int  ellipseSideWidth = 15;
    static       int  uniqueId;    // Stores the unique ID for the next node

    QPointF           newPos;
    GraphWidget*      graph;
    std::list<Edge*>  edgesList;

};

