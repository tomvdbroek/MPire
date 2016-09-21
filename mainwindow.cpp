#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->treeWidget,SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(handleCheckboxEvent(QTreeWidgetItem*, int)));
    connectToDropbox();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//********************************************************************************************************************************//
//    helpers for access to dropbox
//********************************************************************************************************************************

void MainWindow::connectToDropbox()
{

    QNetworkRequest request(QUrl("https://jira.vdbroek.mine.nu"));
    connect (dbDirList, SIGNAL(RequestFinished()), this, SLOT(handle_dirEntriesReceived()));
    connect (ui->webView->page()->networkAccessManager(), SIGNAL(finished(QNetworkReply*)), this, SLOT(WebPageLoadFinished(QNetworkReply*)));

    ui->webView->load(QString("https://www.dropbox.com/1/oauth2/authorize?client_id=qvmt29yar7lzexu&response_type=token&redirect_uri=http://127.0.0.1:5000/callback"));
}


void  MainWindow::handle_dirEntriesReceived()
{
     qDebug() << __FUNCTION__;
     QList<Entrie_str> theList = dbDirList->getDirEntries();
     for (int i = 0; i < theList.count(); i++)
     {
        buildTree(theList.at(i).Pathd);
     }
     qDebug() << "Knows!!!";
}

void MainWindow::WebPageLoadFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        switch (statusCode) {
            case 301:
            case 302:
            case 307:
            {
                QString NewLocation = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString();

                qDebug() << NewLocation;
                NewLocation.truncate(NewLocation.indexOf("&token_type"));
                QString AccessToken = NewLocation.right(NewLocation.length() - ( NewLocation.indexOf("#access_token=") + QString("#access_token=").length()));
                qDebug() << AccessToken;

                dbDirList = new RequestDropboxDirList(AccessToken, this);
                connect (dbDirList, SIGNAL(RequestFinished()), this, SLOT(handle_dirEntriesReceived()));

                dbDirList->DoRequest();
            }
            break;

            case 200:
                break;
        }
    }
}

void MainWindow::handleCheckboxEvent(QTreeWidgetItem* wi,int column)
{
   qDebug() << "call triggered " << wi->text(0);
   for (int i = 0; i < wi->childCount(); i++ )
   {
        QTreeWidgetItem* Kind = wi->child(i);
        Kind->setCheckState(0,wi->checkState(0));
        handleCheckboxEvent(Kind, 0);
   }
}

void MainWindow::buildTree(QString fileName)
{

      QString fn = fileName;
      if (fileName.at(0) == '/')
      {
         fn = fileName.right(fileName.length()-1);
      }
      QStringList fnDiv = fn.split("/");
      QString root = fnDiv.first();
      fnDiv.pop_front();
      QTreeWidgetItem* Parent = NULL;
      QTreeWidgetItemIterator it(ui->treeWidget);


      for (int i = 0; i < ui->treeWidget->topLevelItemCount(); i++ )
      {
           QTreeWidgetItem* Kind = ui->treeWidget->topLevelItem( i );
           if (Kind->text(0) == root)
           {
              Parent = Kind;
           }
      }

      if (Parent == NULL)
      {
          Parent = new QTreeWidgetItem(ui->treeWidget);
          Parent->setCheckState(0,Qt::Unchecked);
          Parent->setText(0, root);
      }

      QTreeWidgetItem* wi = NULL;
      foreach (QString Node, fnDiv)
      {
          wi = NULL;

          for (int i = 0; i < Parent->childCount(); i++ )
          {
               QTreeWidgetItem* Kind = Parent->child( i );
               if (Kind->text(0) == Node)
               {
                  wi = Kind;
               }
          }

          if (wi == NULL)
          {
              wi = new QTreeWidgetItem();
              wi->setText(0, Node);
              wi->setCheckState(0,Qt::Unchecked);
              Parent->addChild(wi);
          }
          Parent = wi;
      }
}
