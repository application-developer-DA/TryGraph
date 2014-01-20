#pragma once

#include "node.h"
#include "edge.h"

#include <map>
#include <memory>

#include <QGraphicsView>

enum Algorithm {
    DijkstraAlgorithm,
    DfsAlgorithm,
    BfsAlgorithm,
    BellmanFordAlgorithm,
    KruskalAlgorithm,
    PrimAlgorithm,
    ConnectivityCheckAlgorithm,
    AlgorithmCount
};

class GraphWidget : public QGraphicsView
                  , public Node::INodeObserver
                  , public Edge::IEdgeObserver
{
    Q_OBJECT

public:
    enum OnPressAction {
        AddEdgeAction,
        EditAction,
        AlgorithmAction,
        ActionCount
    } pressAction;

    GraphWidget(QWidget* parent = nullptr, bool oriented = false);

    virtual void removeNode(uintptr_t id) override;
    virtual void nodeClicked(uintptr_t id) override;

    virtual bool isOriented() const override;
    virtual void removeEdge(uintptr_t id) override;
    virtual void displayCostDialog(uintptr_t id) override;

public slots:
    void applyAlgorithm();

private:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void keyPressEvent(QKeyEvent* event) override;

    void addVertex(int x, int y);

    /* Graph Algorithms */
    void dijkstra(int id);
    void dfs(int id);
    void bfs(int id);
    void bellmanFord(int id);
    void kruskal();
    void prim();

    std::map<uintptr_t, std::unique_ptr<Node>> vertices;
    std::map<uintptr_t, std::unique_ptr<Edge>> edges;

    bool oriented;
};

