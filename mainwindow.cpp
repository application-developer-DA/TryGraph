#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "edge.h"
#include "graphwidget.h"
#include "node.h"

#include <QtXml>

#include <QFileDialog>
#include <QGraphicsView>
#include <QMessageBox>
#include <QRadioButton>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , edgesMode               (new QRadioButton(this))
    , vertexMode              (new QRadioButton(this))
    , graph                   (new GraphWidget(this))
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
{
    ui->setupUi(this);
    setCentralWidget(graph);

    edgesMode->setText(tr("Add edges mode"));
    vertexMode->setText(tr("Add vertexes mode"));
    vertexMode->setChecked(true);
    ui->mainToolBar->addWidget(edgesMode);
    ui->mainToolBar->addWidget(vertexMode);

    fileMenu = ui->menuBar->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addSeparator();

    algorithmMenu = ui->menuBar->addMenu(tr("&Algorithms"));
    algorithmMenu->addAction(dijkstraAction);
    algorithmMenu->addAction(dfsAction);
    algorithmMenu->addAction(bfsAction);
    algorithmMenu->addAction(bellmanFordAction);
    algorithmMenu->addAction(kruskalsAction);
    algorithmMenu->addAction(primsAction);
    algorithmMenu->addAction(connectivityCheckAction);

    connect(newAction,               SIGNAL(triggered()), SLOT(newFile()));
    connect(openAction,              SIGNAL(triggered()), SLOT(openFile()));
    connect(saveAction,              SIGNAL(triggered()), SLOT(saveFile()));
    connect(edgesMode,               SIGNAL(clicked()),   SLOT(edgeModeChanged()));
    connect(vertexMode,              SIGNAL(clicked()),   SLOT(edgeModeChanged()));

    dijkstraAction->setProperty("Algorithm", DijkstraAlgorithm);
    dfsAction->setProperty("Algorithm", DfsAlgorithm);
    bfsAction->setProperty("Algorithm", BfsAlgorithm);
    bellmanFordAction->setProperty("Algorithm", BellmanFordAlgorithm);
    kruskalsAction->setProperty("Algorithm", KruskalAlgorithm);
    primsAction->setProperty("Algorithm", PrimAlgorithm);

    connect(dijkstraAction,          SIGNAL(triggered()), graph, SLOT(applyAlgorithm()));
    connect(dfsAction,               SIGNAL(triggered()), graph, SLOT(applyAlgorithm()));
    connect(bfsAction,               SIGNAL(triggered()), graph, SLOT(applyAlgorithm()));
    connect(bellmanFordAction,       SIGNAL(triggered()), graph, SLOT(applyAlgorithm()));
    connect(kruskalsAction,          SIGNAL(triggered()), graph, SLOT(applyAlgorithm()));
    connect(primsAction,             SIGNAL(triggered()), graph, SLOT(applyAlgorithm()));
    connect(connectivityCheckAction, SIGNAL(triggered()), graph, SLOT(applyAlgorithm()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setStatusBarMessage(const QString& message = "Choose the vertex please")
{
    statusBar()->showMessage(message);
}

void MainWindow::newFile()
{
    /*
    Dialog dialog(this, graph);
    dialog.show();
    dialog.setModal(true);
    */
}

void MainWindow::saveFile()
{
    // Get fileName from user
    /*
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
        QTextStream textStream(&file);
        textStream << doc.toString();

        file.close();
    }
    */
}

void MainWindow::openFile()
{
    // Get fileName from user

    /*
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
    */
}

void MainWindow::edgeModeChanged()
{
    if (edgesMode->isChecked())
        graph->pressAction = GraphWidget::AddEdgeAction;
    else if (vertexMode->isChecked())
        graph->pressAction = GraphWidget::EditAction;
}

