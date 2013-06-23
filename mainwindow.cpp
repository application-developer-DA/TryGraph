#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dialog.h"
#include "edge.h"
#include "graphwidget.h"
#include "node.h"

#include <QtGui>
#include <QtXml>

#include <QFileDialog>
#include <QGraphicsView>
#include <QMessageBox>
#include <QRadioButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , newAction               (new QAction(tr("New"), this))
    , openAction              (new QAction(tr("Open"), this))
    , saveAction              (new QAction(tr("Save"), this))
    , dijkstraAction          (new QAction(tr("Dijkstra's algorithm"), this))
    , dfsAction               (new QAction(tr("DFS"), this))
    , bfsAction               (new QAction(tr("BFS"), this))
    , bellmanFordAction       (new QAction(tr("Bellman-Ford"), this))
    , kruskalsAction          (new QAction(tr("Kruskal's algorithm"), this))
    , primsAction             (new QAction(tr("Prim's algorithm"), this))
    , connectivityCheckAction (new QAction(tr("Check connectivity"), this))
    , graph                   (new GraphWidget(this))
{
    ui->setupUi(this);
    fileMenu = ui->menuBar->addMenu(tr("&File"));
    algorithmMenu = ui->menuBar->addMenu(tr("&Algorithms"));
    setCentralWidget(graph);

    edgesMode  = new QRadioButton(this);
    vertexMode = new QRadioButton(this);
    edgesMode->setText(tr("Add edges mode"));
    vertexMode->setText(tr("Add vertexes mode"));
    vertexMode->setChecked(true);
    ui->mainToolBar->addWidget(edgesMode);
    ui->mainToolBar->addWidget(vertexMode);

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();

    algorithmMenu->addAction(dijkstraAction);
    algorithmMenu->addAction(dfsAction);
    algorithmMenu->addAction(bfsAction);
    algorithmMenu->addAction(bellmanFordAction);
    algorithmMenu->addAction(kruskalsAction);
    algorithmMenu->addAction(primsAction);
    algorithmMenu->addAction(connectivityCheckAction);

    connect(newAction, SIGNAL(triggered()),
            this, SLOT(newFile()));
    connect(openAction, SIGNAL(triggered()),
            this, SLOT(openFile()));
    connect(saveAction, SIGNAL(triggered()),
            this, SLOT(saveFile()));
    connect(dijkstraAction, SIGNAL(triggered()),
            this, SLOT(dijkstraStart()));
    connect(dfsAction, SIGNAL(triggered()),
            this, SLOT(dfsStart()));
    connect(bfsAction, SIGNAL(triggered()),
            this, SLOT(bfsStart()));
    connect(bellmanFordAction, SIGNAL(triggered()),
            this, SLOT(bellmanFordStart()));
    connect(kruskalsAction, SIGNAL(triggered()),
            this, SLOT(kruskalsStart()));
    connect(primsAction, SIGNAL(triggered()),
            this, SLOT(primsStart()));
    connect(connectivityCheckAction, SIGNAL(triggered()),
            this, SLOT(connectivityStart()));
    connect(edgesMode, SIGNAL(clicked()),
            this, SLOT(edgeModeChanged()));
    connect(vertexMode, SIGNAL(clicked()),
            this, SLOT(edgeModeChanged()));
}

MainWindow::~MainWindow()
{
    graph->clean();
    delete ui;
    delete edgesMode;
    delete vertexMode;
    delete fileMenu;
    delete newAction;
    delete openAction;
    delete saveAction;
    delete algorithmMenu;
    delete dijkstraAction;
    delete dfsAction;
    delete bellmanFordAction;
    delete kruskalsAction;
    delete primsAction;
    delete connectivityCheckAction;
    delete graph;
}

void MainWindow::setStatusBarMessage(const QString &message)
{
    statusBar()->showMessage(message);
}

void MainWindow::newFile()
{
    graph->clean();
    Dialog dialog(this, graph);
    dialog.show();
    dialog.setModal(true);
}

void MainWindow::saveFile()
{
    // Get fileName from user
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Animation"), "",
                                                    tr("(*.ae)"));
    if (fileName.isEmpty())
        return;
    else {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::information(this, tr("Unable to open file"),
                                     file.errorString());
            return;
        }

        QDomDocument doc;
        QDomElement graphElement = doc.createElement("graph");
        graphElement.setAttribute("isOriented", graph->isOriented);
        doc.appendChild(graphElement);

        QDomElement nodesElem = doc.createElement("nodes");
        graphElement.appendChild(nodesElem);
        QDomElement edgesElem = doc.createElement("edges");
        graphElement.appendChild(edgesElem);

        // Get nodes and edges and save them one after another
        QList<Node*> nodes;
        QList<Edge*> edges;
        foreach (QGraphicsItem *item, graph->items) {
            if (Node *node = qgraphicsitem_cast<Node*>(item))
                nodes << node;
            else if (Edge *edge = qgraphicsitem_cast<Edge*>(item))
                edges << edge;
        }

        foreach (Node *node, nodes){
            QDomElement nodeElem = doc.createElement("node");
            nodeElem.setAttribute("id", node->id);
            nodeElem.setAttribute("x", node->x());
            nodeElem.setAttribute("y", node->y());
            nodesElem.appendChild(nodeElem);
        }

        foreach (Edge *edge, edges){
            QDomElement edgeElem = doc.createElement("edge");
            edgeElem.setAttribute("node1", edge->getSourceNode()->id);
            edgeElem.setAttribute("node2", edge->getDestinationNode()->id);
            edgeElem.setAttribute("cost", edge->costLink());
            edgesElem.appendChild(edgeElem);
        }

        // Save doc to selected file
        QTextStream TextStream(&file);
        TextStream << doc.toString();

        file.close();
    }
}

void MainWindow::openFile()
{
    // Get fileName from user
    QString fileName = QFileDialog::getOpenFileName(this, tr("Load Animation"));
    if (fileName.isEmpty())
        return;

    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, tr("Unable to open file"), file.errorString());
        return;
    }

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    graph->clean();
    QDomElement docElem = doc.documentElement();
    bool val = docElem.attribute("isOriented").toInt();
    graph->isOriented = val;

    int greatestID = -9999;

    // For each node in frame
    QDomNode nodesNode = docElem.firstChild();
    if(!nodesNode.isNull()){
        QDomNode nodeNode = nodesNode.firstChild();
        while(!nodeNode.isNull()){
            QDomElement nodeElem = nodeNode.toElement();

            // Get info about the node
            qreal x = nodeElem.attribute("x").toDouble();
            qreal y = nodeElem.attribute("y").toDouble();
            int id = nodeElem.attribute("id").toInt();
            if(id > greatestID)
                greatestID = id;

            Node *node = new Node(graph, x, y, id);         // Create new node
            graph->createNode(node->x(), node->y(), id);    // Add node to the frame
            nodeNode = nodeNode.nextSibling();              // Go to next node
        }

    }
    Node::setUIDcounter(greatestID+1);

    // For each edge in frame
    QDomNode edgesNode = nodesNode.nextSibling();
    if(!edgesNode.isNull()){
        QDomNode edgeNode = edgesNode.firstChild();
        while(!edgeNode.isNull()){
            QDomElement edgeElem = edgeNode.toElement();

            // Get info about the edge
            int id1 = edgeElem.attribute("node1").toInt();
            int cost = edgeElem.attribute("cost").toInt();
            int id2 = edgeElem.attribute("node2").toInt();

            graph->addEdgeById(id1, id2, cost);             // create new edge from id's
            edgeNode = edgeNode.nextSibling();              // Go to next edge
        }
    }
}

void MainWindow::dijkstraStart()
{
    setStatusBarMessage("Choose the vertex please");
    graph->isDijkstraStart = true;
}

void MainWindow::dfsStart()
{
    setStatusBarMessage("Choose the vertex please");
    graph->isSearchDraw = true;
}

void MainWindow::bfsStart()
{
    setStatusBarMessage("Choose the vertex please");
    graph->isBfs = true;
}

void MainWindow::bellmanFordStart()
{
    setStatusBarMessage("Choose the vertex please");
    graph->isBellmanFord = true;
}

void MainWindow::kruskalsStart()
{
    graph->kruskalsAlgorithm();
}

void MainWindow::primsStart()
{
    graph->primsAlgorithm();
}

void MainWindow::connectivityStart()
{
    graph->checkConnectivity(true);
}

void MainWindow::edgeModeChanged()
{
    if(edgesMode->isChecked())
        graph->changeType(true);
    else
        graph->changeType(false);
}

