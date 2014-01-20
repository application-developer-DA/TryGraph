#include "edge.h"
#include "graphwidget.h"
#include "node.h"

#include <exception>

#include <QGraphicsView>
#include <QtGui>


GraphWidget::GraphWidget(QWidget* parent, bool isOriented)
    : QGraphicsView(parent)
    , isOriented(isOriented)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
}

void GraphWidget::removeNode(uintptr_t id)
{
    auto it = vertices.find(id);
    Q_ASSERT(it != vertices.end());

    auto adjacentEdges = it->edgeIdentifiers();
    for (auto it = begin(adjacentEdges); it != end(adjacentEdges); ++it)
        edges.erase(*it);

    vertices.erase(id);
}

void GraphWidget::nodeClicked(uintptr_t id)
{
    switch (pressAction) {
    case AddEdgeAction:
        break;
    case EditAction:
        break;
    case AlgorithmAction:
        break;
    default:
        Q_ASSERT(0);
    }
}

void GraphWidget::isOriented() const
{
    return isOriented;
}

void GraphWidget::removeEdge(uintptr_t id)
{
    auto it = edges.find(id);
    Q_ASSERT(it != edges.end());
    edges.erase(it);
}

void GraphWidget::displayCostDialog(uintptr_t id)
{
    Q_UNUSED(id);
    throw std::runtime_error("not implemented");
}

void GraphWidget::applyAlgorithm()
{
    Algorithm algorithm = (Algorithm)sender()->property("Algorithm");
    switch (algorithm) {
    case DijkstraAlgorithm:
        break;
    case DfsAlgorithm:
        break;
    case BfsAlgorithm:
        break;
    case BellmanFordAlgorithm:
        break;
    case KruskalAlgorithm:
        break;
    case PrimAlgorithm:
        break;
    case AlgorithmCount:
        break;
    case ConnectivityCheckAlgorithm:
        break;
    default:
        Q_ASSERT(0);
    }
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



