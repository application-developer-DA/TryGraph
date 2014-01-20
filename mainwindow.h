#pragma once

#include <QMainWindow>

#include <memory>

class QRadioButton;
class GraphWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void setStatusBarMessage(const QString& message);

private slots:
    void newFile();
    void saveFile();
    void openFile();
    void edgeModeChanged();

private:
    Ui::MainWindow* ui;

    QRadioButton* edgesMode;
    QRadioButton* vertexMode;

    GraphWidget*  graph;

    QMenu*    fileMenu;
    QAction*  newAction;
    QAction*  openAction;
    QAction*  saveAction;

    QMenu*    algorithmMenu;
    QAction*  dijkstraAction;
    QAction*  dfsAction;
    QAction*  bfsAction;
    QAction*  bellmanFordAction;
    QAction*  kruskalsAction;
    QAction*  primsAction;
    QAction*  connectivityCheckAction;
};

