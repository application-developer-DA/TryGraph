#include <QPainter>
#include <QGraphicsScene>
#include <QStyleOption>
#include <QGraphicsSceneMouseEvent>
#include <QLabel>
#include <QMessageBox>

#include "node.h"
#include "edge.h"
#include "mainwindow.h"
#include "graphwidget.h"

int Node::uniqueId = 0;

Node::Node(GraphWidget *graphwidget, qreal x, qreal y, int _id)
    : graph(graphwidget)
    , shortestPath(99999999)
{
    id = (id >= 0) ? (_id) : uniqueId++;
    setPos(x, y);

    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setAcceptHoverEvents(true);

    if(graph->isOriented)
        setZValue(-1);
    else
        setZValue(1);
}

Node::~Node()
{
    // Should be 0 when all nodes are deleted
    //--uniqueId;
}

void Node::addEdge(Edge *edge)
{
    edgesList.push_back(edge);
    edge->adjust();
}

void Node::removeEdge(Edge *edge)
{
    edgesList.remove(edge);
}

void Node::removeConnections()
{
    std::list<Edge*> edgesListCopy = edgesList;

    for(std::list<Edge*>::iterator it = edgesListCopy.begin(); it != edgesListCopy.end(); ++it)
        graph->removeEdge(*it);
}

void Node::setId(int _id)
{
    id = _id;
}

std::list<Edge*> Node::getEdges() const
{
    return edgesList;
}

Edge *Node::isDirectlyConnected(Node *firstNode, Node *secondNode)
{
    if(!graph->isOriented)
    {
        std::list<Edge*> edges = firstNode->getEdges();
        for(std::list<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it)
            if((*it)->getDestinationNode() == secondNode || (*it)->getSourceNode() == secondNode)
                return *it;
    }
    else
    {
        std::list<Edge*> edges = firstNode->getEdges();
        for(std::list<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it)
            if((*it)->getDestinationNode() == secondNode)
                return *it;
    }

    return NULL;
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
    if (option->state & QStyle::State_Sunken)
    {
        // Change color if selected
        gradient.setColorAt(0, QColor(Qt::black).light(120));
        gradient.setColorAt(1, QColor(Qt::darkGray).light(120));
    }
    else
    {
        gradient.setColorAt(0, Qt::darkGray);
        gradient.setColorAt(1, Qt::black);
    }
    painter->setBrush(gradient);
    painter->setPen(QPen(Qt::black, 0));

    painter->drawEllipse(-ellipseSideWidth/2, -ellipseSideWidth/2, ellipseSideWidth, ellipseSideWidth);
}

QVariant Node::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch(change)
    {
    case ItemPositionHasChanged:
        foreach(Edge* edge, edgesList)
            edge->adjust();
        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::RightButton)
    {
        // Remove node and its edges
        graph->removeNode(this);
        return;
    }
    else if(event->button() == Qt::LeftButton)
    {
        if(graph->addEdgesMode)
        {
            if(graph->from == NULL)
            {
                // If it's the first node to be selected
                graph->from = this;
            }
            else
            {
                // If it's the second node = create an edge between them
                graph->createEdge(graph->from, this);
                // Reset 'from' pointer
                graph->from = NULL;
            }
        }
        else
        {
            if(graph->isDijkstraStart)
            {
                graph->DijkstrasAlgorithm(this->id);
            }
            else if(graph->isSearchDraw)
            {
                graph->dfs(this->id);
            }
            else if(graph->isBfs)
            {
                graph->bfs(id);
            }
            else if(graph->isBellmanFord)
            {
                graph->bellmanFord(id);
            }

        }
    }
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

