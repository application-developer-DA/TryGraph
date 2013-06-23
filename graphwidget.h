#ifndef GRAPHWIDGET_H
#define GRAPHWIDGET_H

#include <QGraphicsView>

class Node;
class Edge;

#define TestPurposes

class GraphWidget : public QGraphicsView
{
    Q_OBJECT

public:
    GraphWidget(QWidget* parent = 0);

    void addEdgeById(int, int, int cost = 0);
    void removeNode(Node* node);
    void removeEdge(Edge* edge);
    void clean();

    Edge* createEdge(Node *node1, Node *node2, int cost = 1);
    Node* createNode(qreal x, qreal y, int id = -1);

    Node* from;
    QList<QGraphicsItem*> items;


    bool addEdgesMode;  // Whether addEdges mode on or off
    bool isOriented;
    bool isDijkstraStart;
    bool isSearchDraw; // DFS
    bool isBfs;
    bool isBellmanFord;

    // Algorithms
    void DijkstrasAlgorithm(int id);
    void dfs(int id);
    void _dfs(Node* node, QMap<int, bool>& visited, int& idx);
    int bfs(int id, bool showResult = true);
    void bellmanFord(int id);
    void kruskalsAlgorithm();
    void primsAlgorithm();
    bool checkConnectivity(bool displayMessage = false);

public slots:
    void changeType(bool value) { addEdgesMode = value; }
    void clear();
    void startKruskals();

protected:
    void mousePressEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QList<QGraphicsTextItem*> texts;
    void checkTests();
    int getSetIdx(const QList< QSet<int> >& sets, int idx);
    int findMinCost(const QList<Edge*>& edges, const QSet<int>& verticesSet);
};

#endif // GRAPHWIDGET_H
