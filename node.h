#pragma once

#include "edge.h"

#include <list>

#include <QGraphicsItem>


class Node : public QGraphicsItem
{
public:
    class INodeObserver
    {
        virtual void removeNode(int id) = 0;
        virtual void nodeClicked(int id) = 0;
    };

    Node(INodeObserver* observer, qreal x = 0, qreal y = 0, int id = -1);

    void addEdge(Edge& edge);
    void removeEdge(Edge& edge);
    void removeConnections();

    int id() const                          { return id; }
    void setId(int id)                      { this->id = id; }
    std::list<Edge&> edges() const          { return edgesList; }

private:
    static const int  ellipseSideWidth = 15;

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    virtual QRectF boundingRect() const override;
    virtual QPainterPath shape() const override;

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    INodeObserver*    observer;
    int               id;
    int               shortestPath;
    std::list<Edge&>  edgesList;

};

