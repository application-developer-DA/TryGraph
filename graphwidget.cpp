
#include "edge.h"
#include "graphwidget.h"
#include "mainwindow.h"
#include "node.h"


#include <QtAlgorithms>
#include <QtGui>
#include <QGraphicsTextItem>
#include <QMap>
#include <QMessageBox>
#include <QMultiMap>
#include <QPainterPath>
#include <QPair>
#include <QSet>

#include <algorithm>

static bool lessThanComparator( Edge* e1, Edge* e2)
{
    return e1->costLink() < e2->costLink();
}

GraphWidget::GraphWidget(QWidget *parent)
    :QGraphicsView(parent),
      addEdgesMode(false),
      from(NULL),
      isOriented(false),
      isDijkstraStart(false),
      isSearchDraw(false),
      isBfs(false),
      isBellmanFord(false)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(0, 0, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
}

void GraphWidget::addEdgeById(int id1, int id2, int cost)
{
    if(id1 == id2)
        return;

    Node* node1 = NULL;
    Node* node2 = NULL;

    foreach(QGraphicsItem* item, items)
    {
        if(Node* node = qgraphicsitem_cast<Node*>(item))
        {
            // Create node
            if(node->id == id1)
                node1 = node;
            else if(node->id == id2)
                node2 = node;
        }
    }

    // If one of nodes doesn't exist - exit
    if((node1 == NULL) || (node2 == NULL))
        return;

    // If there is already an edge - dont do anything
    if(/*Node::*/node1->isDirectlyConnected(node1, node2) != NULL)
        return;

    // Create edge and add it to this frame
    createEdge(node1, node2, cost);
}

void GraphWidget::removeNode(Node *node)
{
    scene()->removeItem(node);
    node->removeConnections();
    items.removeOne(node);
    delete node;
}

void GraphWidget::removeEdge(Edge *edge)
{
    scene()->removeItem(edge);
    items.removeOne(edge);
    delete edge;
}

void GraphWidget::clean()
{
    if(items.isEmpty())
        return;

    QList<Edge*> edges;
    QList<Node*> nodes;
    foreach(QGraphicsItem* item, items)
    {
        if(Edge* edge = qgraphicsitem_cast<Edge*>(item))
            edges << edge;
        else if(Node* node = qgraphicsitem_cast<Node*>(item))
            nodes << node;
    }
    foreach(Edge* edge, edges)
        delete edge;
    foreach(Node* node, nodes)
        delete node;

    items.clear();
    checkTests();
}

Edge *GraphWidget::createEdge(Node *node1, Node *node2, int cost)
{
    // Check if nodes are already connected
    // just return pointer to this edge if they are
    Edge* edge = /*Node::*/node1->isDirectlyConnected(node1, node2);
    if(edge != NULL)
        return edge;

    // else create a new edge
    edge = new Edge(this, node1, node2, cost);
    scene()->addItem(edge);
    items.push_back(edge);

    return edge;
}

Node *GraphWidget::createNode(qreal x, qreal y, int id)
{
    Node* node = new Node(this, x, y, id);
    scene()->addItem(node);
    node->setPos( mapToScene(x, y));
    items.push_back(node);
    return node;
}

void GraphWidget::DijkstrasAlgorithm(int id)
{   
    MainWindow* parentWindow = (MainWindow*)parent();
    parentWindow->setStatusBarMessage("");

    if(!checkConnectivity())
    {
        QMessageBox msg;
        msg.setText("Graph isn't connected. Algorithm cannot be applied.    ");
        msg.exec();
        return;
    }
    QList<Node*> vertices;
    //QList< QPair<int, bool> > visited;
    //QSet< QPair<int, bool> > visited;
    QMap< int, bool > visited;
    Node* initialNode = NULL;
    foreach(QGraphicsItem* item, items)
    {
        if(Node* node = qgraphicsitem_cast<Node*>(item))
        {
            vertices << node;
            //visited.push_back(QPair<int, bool>(node->id, false));
            visited.insert(id, false);
            if(node->id == id)
            {
                initialNode = node;
                node->shortestPath = 0;
            }
        }
        if(Edge* edge = qgraphicsitem_cast<Edge*>(item))
        {
            if(edge->costLink() < 0)
            {
                QMessageBox msg;
                msg.setText("Algorithm cannot be applied - graph has negative edges.");
                msg.exec();
                return;
            }
        }
    }

    Node* currentNode = initialNode;

    for(;;)
    {
Label1:

        QList<Edge*> edgesList;
        std::list<Edge*> edges = currentNode->getEdges();
        bool end = true;
        for(std::list<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it)
        {
            Edge* curEdge = *it;
            Node* destNode = curEdge->getDestinationNode();
            if(destNode == currentNode && isOriented == false)
                destNode = curEdge->getSourceNode();
            edgesList.push_back(curEdge);
            if(visited[destNode->id] == false)
            {
                end = false;
                int sumCost = currentNode->shortestPath + curEdge->costLink();
                if(sumCost < destNode->shortestPath)
                    destNode->shortestPath = sumCost;
            }
        }

        if(end)
        {
            visited[currentNode->id] = true;
            break;
        }

        visited[currentNode->id] = true;
        qSort(edgesList.begin(), edgesList.end(), lessThanComparator);
        Edge* minEdge;
        for(int i = 0; i < edgesList.size(); i++)
        {
            Node* tmp = edgesList[i]->getDestinationNode();
            if(tmp == currentNode && isOriented == false)
                tmp = edgesList[i]->getSourceNode();
            if(visited[tmp->id] != true)
            {
                minEdge = edgesList[i];
                break;
            }
        }
        Node* minNode = minEdge->getDestinationNode();
        if(minNode == currentNode && isOriented == false)
            minNode = minEdge->getSourceNode();
        currentNode = minNode;

    }

#ifdef SecondVariantOfAlrogithm
    ///////
    for(QMap<int, bool>::iterator it = visited.begin(); it != visited.end(); ++it)
    {
        if(it.value())
        {
            foreach(Node* node, vertices)
            {
                if(it.key() == node->id)
                {
                    currentNode = node;
                    goto Label1;
                }
            }
        }
    }
    ///////
#endif

    foreach(Node* tmp, vertices)
    {
        QGraphicsTextItem* text;
        text = new QGraphicsTextItem;
        text->setPlainText(QString::number(tmp->shortestPath));
        text->setDefaultTextColor(Qt::red);
        text->setPos(tmp->pos());
        scene()->addItem(text);

        texts.push_back(text);
     }

    isDijkstraStart = false;
}

void GraphWidget::dfs(int id)
{
    MainWindow* parentWindow = (MainWindow*)parent();
    parentWindow->setStatusBarMessage("");

    if(!checkConnectivity())
    {
        return;
    }
    checkTests();


    QList<Node*> vertices;
    QMap< int, bool > visited;
    Node* initialNode = NULL;
    foreach(QGraphicsItem* item, items)
    {
        if(Node* node = qgraphicsitem_cast<Node*>(item))
        {
            vertices << node;
            visited.insert(id, false);
            if(node->id == id)
            {
                initialNode = node;
            }
        }
    }

    int index = 1;
    _dfs(initialNode, visited, index);

    isSearchDraw = false;
}

void GraphWidget::_dfs(Node *node, QMap<int, bool> &visited, int &idx)
{
    visited[node->id] = true;

    // Add text on scene ---
    QGraphicsTextItem* text;
    text = new QGraphicsTextItem;
    text->setPlainText(QString::number(idx++));
    text->setDefaultTextColor(Qt::red);
    text->setPos(node->pos());
    scene()->addItem(text);
    texts.push_back(text);
    // ---------------------

    std::list<Edge*> edges = node->getEdges();
    for(std::list<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it)
    {
        Edge* curEdge = *it;
        Node* destNode = curEdge->getDestinationNode();
        if(destNode == node && isOriented == false)
            destNode = curEdge->getSourceNode();

        if(!visited[destNode->id])
        {
            _dfs(destNode, visited, idx);
        }
    }
}

int GraphWidget::bfs(int id, bool showResult)
{
    MainWindow* parentWindow = (MainWindow*)parent();
    parentWindow->setStatusBarMessage("");

    checkTests();

    QList<Node*> vertices;
    QMap< int, bool > visited;
    Node* initialNode = NULL;
    foreach(QGraphicsItem* item, items)
    {
        if(Node* node = qgraphicsitem_cast<Node*>(item))
        {
            vertices << node;
            visited.insert(id, false);
            if(node->id == id)
            {
                initialNode = node;
            }
        }
    }

    QQueue<Node*> q;
    q.push_back(initialNode);
    int idx = 1;
    Node* node = initialNode;

    int count = 0;
    while(!q.empty())
    {
        node = q.front();
        q.pop_front();

        if(!visited[node->id])
        {
            // Add text on scene ---
            if(showResult)
            {
                QGraphicsTextItem* text;
                text = new QGraphicsTextItem;
                text->setPlainText(QString::number(idx++));
                text->setDefaultTextColor(Qt::red);
                text->setPos(node->pos());
                scene()->addItem(text);
                texts.push_back(text);
            }
            // ---------------------

            visited[node->id] = true;
            ++count;

            std::list<Edge*> edges = node->getEdges();
            for(std::list<Edge*>::iterator it = edges.begin(); it != edges.end(); ++it)
            {
                Edge* curEdge = *it;
                Node* destNode = curEdge->getDestinationNode();
                if(destNode == node && isOriented == false)
                    destNode = curEdge->getSourceNode();
                q.push_back(destNode);
            }
        }

    }

    isBfs = false;

    return count;
}

void GraphWidget::bellmanFord(int id)
{
    MainWindow* parentWindow = (MainWindow*)parent();
    parentWindow->setStatusBarMessage("");

    if(!checkConnectivity())
    {
        QMessageBox msg;
        msg.setText("Graph isn't connected. Algorithm cannot be applied.");
        msg.exec();
        return;
    }
    checkTests();


    QList<Node*> vertices;
    QList< QPair<int, int> > edgesList;
    QMap<int, int> distances;
    Node* initialNode = NULL;
    QList<int> costs;
    foreach(QGraphicsItem* item, items)
    {
        if(Node* node = qgraphicsitem_cast<Node*>(item))
        {
            vertices << node;

            if(node->id == id)
            {
                initialNode = node;
                node->shortestPath = 0;
                distances.insert(node->id, 0);
            }
            else
            {
                distances.insert(node->id, node->shortestPath);
            }
        }
        else if(Edge* edge = qgraphicsitem_cast<Edge*>(item))
        {
            if(!isOriented && (edge->costLink() < 0))
            {
                QMessageBox msg;
                msg.setText("Graph has negative chain. Algorithm cannot be applied.");
                msg.exec();
                return;
            }

            edgesList.push_back(QPair<int, int>(edge->getSourceNode()->id, edge->getDestinationNode()->id));
            costs << edge->costLink();

            if(!isOriented)
            {
                edgesList.push_back(QPair<int, int>(edge->getDestinationNode()->id, edge->getSourceNode()->id));
                costs << edge->costLink();
            }
        }
    }

    typedef QPair<Node*,Node*> branch_type;
    for(int i = 0; i < vertices.size(); i++)
    {
        for(int j = 0; j < edgesList.size(); j++)
        {
            if(distances[edgesList[j].first] + costs[j] < distances[edgesList[j].second])
            {
                distances[edgesList[j].second] = distances[edgesList[j].first] + costs[j];
            }
        }
    }



    foreach(Node* tmp, vertices)
    {
        tmp->shortestPath = distances[tmp->id];

        QGraphicsTextItem* text;
        text = new QGraphicsTextItem;
        text->setPlainText(QString::number(tmp->shortestPath));
        text->setDefaultTextColor(Qt::red);
        text->setPos(tmp->pos());
        scene()->addItem(text);

        texts.push_back(text);
    }

    isBellmanFord = false;
}

void GraphWidget::kruskalsAlgorithm()
{
    MainWindow* parentWindow = (MainWindow*)parent();
    parentWindow->setStatusBarMessage("");

    if(isOriented)
    {
        QMessageBox msg;
        msg.setText("Graph is oriented. Algorithm cannot be applied.");
        msg.exec();
        return;
    }
    if(!checkConnectivity())
    {
        QMessageBox msg;
        msg.setText("Graph isn't connected. Algorithm cannot be applied.");
        msg.exec();
        return;
    }

    QMultiMap<int, Edge*> edges;
    QSet<int> verticesSet;
    QList< QSet<int> > sets;
    foreach(QGraphicsItem* item, items)
    {
        if(Node* node = qgraphicsitem_cast<Node*>(item))
        {
            QSet<int> tmp;
            tmp.insert(node->id);
            sets.push_back(QSet<int>(tmp));
        }
        else if(Edge* edge = qgraphicsitem_cast<Edge*>(item))
        {
            edge->isInTree = false;
            edges.insert(edge->costLink(), edge);
        }
    }


    QList<Edge*> tree;
    for(QMultiMap<int, Edge*>::iterator it = edges.begin(); it != edges.end(); ++it)
    {
        int v1 = it.value()->getSourceNode()->id;
        int v2 = it.value()->getDestinationNode()->id;

        int v1Idx = getSetIdx(sets, v1);
        int v2Idx = getSetIdx(sets, v2);

        if(v1Idx == v2Idx)
            continue;

        sets[v1Idx].unite(sets[v2Idx]);
        sets.removeAt(v2Idx);


        tree.push_back(it.value());

    }

    for(int i = 0; i < tree.size(); i++)
    {
        tree[i]->isInTree = true;
        tree[i]->update();
    }
}

void GraphWidget::primsAlgorithm()
{
    MainWindow* parentWindow = (MainWindow*)parent();
    parentWindow->setStatusBarMessage("");

    if(isOriented)
    {
        QMessageBox msg;
        msg.setText("Graph is oriented. Algorithm cannot be applied.");
        msg.exec();
        return;
    }
    if(!checkConnectivity())
    {
        QMessageBox msg;
        msg.setText("Graph isn't oriented. Algorithm cannot be applied.");
        msg.exec();
        return;
    }

    QList<Edge*> edgesList;
    QSet<Edge*> tree;
    QSet<int> verticesSet;
    int minCost = 99999;
    Node* initialNode = NULL;
    int verticesCount = 0;
    foreach(QGraphicsItem* item, items)
    {
        if(Node* node = qgraphicsitem_cast<Node*>(item))
        {
            verticesCount++;
        }
        else if(Edge* edge = qgraphicsitem_cast<Edge*>(item))
        {
            edge->isInTree = false;
            edgesList << edge;
            if(edge->costLink() < minCost)
            {
                minCost = edge->costLink();
                initialNode = edge->getSourceNode();
            }
        }
    }
    qSort(edgesList.begin(), edgesList.end(), lessThanComparator);
    verticesSet.insert(initialNode->id);

    Edge* edgeToBeAdded = NULL;
    while(verticesSet.size() != verticesCount)
    {
        minCost = findMinCost(edgesList, verticesSet);
        foreach(Edge* currentEdge, edgesList)
        {
            if(currentEdge->costLink() == minCost)
            {
                if(!tree.contains(currentEdge))
                {
                    if((!verticesSet.contains(currentEdge->getSourceNode()->id) && verticesSet.contains(currentEdge->getDestinationNode()->id))
                            || (verticesSet.contains(currentEdge->getSourceNode()->id) && !verticesSet.contains(currentEdge->getDestinationNode()->id)))
                    {
                        edgeToBeAdded = currentEdge;
                        break;
                    }
                }
            }
        }
        if(edgeToBeAdded)
        {
            tree.insert(edgeToBeAdded);
            verticesSet.insert(edgeToBeAdded->getDestinationNode()->id);
            verticesSet.insert(edgeToBeAdded->getSourceNode()->id);
            edgeToBeAdded->isInTree = true;
            edgeToBeAdded->update();
        }
    }
}

bool GraphWidget::checkConnectivity(bool displayMessage)
{
    QList<Node*> vertices;
    foreach(QGraphicsItem* item, items)
    {
        if(Node* node = qgraphicsitem_cast<Node*>(item))
        {
            vertices << node;
        }
    }

    bool previousState = isOriented;
    if(isOriented)
        isOriented = false;
    int count = bfs(vertices[0]->id, false);
    isOriented = previousState;

    QMessageBox msg;
    if(count == vertices.size())
    {
        if(displayMessage)
        {
            msg.setText("Graph is connected.");
            msg.exec();
        }
        return true;
    }
    else
    {
        if(displayMessage)
        {
            msg.setText("Graph isn't connected.");
            msg.exec();
        }
        return false;
    }

}

void GraphWidget::clear()
{
    clean();
}

void GraphWidget::startKruskals()
{
    kruskalsAlgorithm();
}

void GraphWidget::checkTests()
{
    if(texts.size() > 0)
    {
        foreach(QGraphicsTextItem* text, texts)
        {
            scene()->removeItem(text);
        }
        texts.clear();
    }
}

int GraphWidget::getSetIdx(const QList<QSet<int> > &sets, int idx)
{
    for(int i = 0; i < sets.size(); i++)
    {
        if(sets[i].contains(idx))
            return i;
    }

    return -1;
}

int GraphWidget::findMinCost(const QList<Edge *> &edges, const QSet<int> &verticesSet)
{
    for(int i = 0; i < edges.size(); i++)
    {
        if((verticesSet.contains(edges[i]->getSourceNode()->id) && !verticesSet.contains(edges[i]->getDestinationNode()->id))
                || (!verticesSet.contains(edges[i]->getSourceNode()->id) && verticesSet.contains(edges[i]->getDestinationNode()->id)))
            return edges[i]->costLink();
    }
    return 0xDEADBEEF;
}

void GraphWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::MiddleButton)
        createNode(event->pos().x(), event->pos().y());

    update();
    QGraphicsView::mousePressEvent(event);
}

void GraphWidget::keyPressEvent(QKeyEvent *event)
{

    if(event->key() == Qt::Key_Plus)
    {
        scale(2.0, 2.0);

    }
    else if(event->key() == Qt::Key_Minus)
    {
        scale(0.9, 0.9);
    }

    QGraphicsView::keyPressEvent(event);
}

