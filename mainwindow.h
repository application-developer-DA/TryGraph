#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QRadioButton;
class GraphWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setStatusBarMessage(const QString& message);

private slots:
    void newFile();
    void saveFile();
    void openFile();
    void dijkstraStart();
    void dfsStart();
    void bfsStart();
    void bellmanFordStart();
    void kruskalsStart();
    void primsStart();
    void connectivityStart();
    void edgeModeChanged();

private:
    Ui::MainWindow *ui;

    QMenu*   fileMenu;
    QAction* newAction;
    QAction* openAction;
    QAction* saveAction;

    QMenu*   algorithmMenu;
    QAction* dijkstraAction;
    QAction* dfsAction;
    QAction* bfsAction;
    QAction* bellmanFordAction;
    QAction* kruskalsAction;
    QAction* primsAction;
    QAction* connectivityCheckAction;

    GraphWidget* graph;

    QRadioButton* edgesMode;
    QRadioButton* vertexMode;
};

#endif // MAINWINDOW_H
