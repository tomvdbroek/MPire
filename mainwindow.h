#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>

#include "qtdropbox.h"
#include "requestdropboxdirlist.h"
#include "QTimer"

#include <QTreeWidgetItem>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QDropbox* Dropbox;
    QTcpServer* TCPserver;
    QTimer timer;
    RequestDropboxDirList* dbDirList;

    /**/
    void ConnectAllDropboxSignals();
    void connectToDropbox();
    void DumpDropBoxData();

    void buildTree(QString fileName);

private slots:
    void handleCheckboxEvent(QTreeWidgetItem* wi,int column);
    void WebPageLoadFinished(QNetworkReply*);
    void handle_dirEntriesReceived();

};

#endif // MAINWINDOW_H
