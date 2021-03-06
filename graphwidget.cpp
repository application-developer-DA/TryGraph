#include "edge.h"
#include "graphwidget.h"
#include "node.h"

#include <exception>

#include <QGraphicsView>
#include <QtGui>


GraphWidget::GraphWidget(QWidget* parent, bool oriented)
    : QGraphicsView(parent)
    , oriented(oriented)
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
    if (it == vertices.end()) {
        Q_ASSERT(it != vertices.end());
        return;
    }

    auto adjacentEdges = it->second->edgeIdentifiers().clear();
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

bool GraphWidget::isOriented() const
{
    return oriented;
}

void GraphWidget::removeEdge(uintptr_t id)
{
    auto it = edges.find(id);
    if (it == edges.end()) {
        Q_ASSERT(it != edges.end());
        return;
    }
    edges.erase(it);
}

void GraphWidget::displayCostDialog(uintptr_t id)
{
    Q_UNUSED(id);
    throw std::runtime_error("not implemented");
}

void GraphWidget::applyAlgorithm()
{
    Algorithm algorithm = static_cast<Algorithm>(sender()->property("Algorithm").toInt());
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
    if (event->button() == Qt::MiddleButton)
        addVertex(event->x(), event->y());

    update();
    QGraphicsView::mousePressEvent(event);
}

void GraphWidget::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Plus)
        scale(2.0, 2.0);
    else if (event->key() == Qt::Key_Minus)
        scale(0.9, 0.9);
    QGraphicsView::keyPressEvent(event);
}

void GraphWidget::addVertex(int x, int y)
{
    Node* node = new Node(this, x, y);
    scene()->addItem(node);
    node->setPos(mapToScene(x, y));

    Q_ASSERT(vertices.find((uintptr_t)node) == vertices.end());
    vertices.insert(std::make_pair((uintptr_t)node, std::unique_ptr<Node>(node)));
}

