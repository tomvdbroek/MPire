#include "requestdropboxdirlist.h"

#include <QApplication>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

RequestDropboxDirList::RequestDropboxDirList(QString token, QObject *parent) : QObject(parent)
{
   AuthToken = token;
   networkmanager = new QNetworkAccessManager(this);
   connect(networkmanager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleRequestDone(QNetworkReply*)));
}


void RequestDropboxDirList::DoRequest()
{
    QJsonObject Params;
    Params["recursive"] = true;
    Params["include_media_info"] = true;
    Params["include_deleted"] = true;
    Params["include_has_explicit_shared_members"] = true;
    Params["path"] = "";

    QNetworkRequest request;
    request.setUrl(QUrl("https://api.dropboxapi.com/2/files/list_folder"));
    request.setRawHeader("Content-Type", "application/json");
    QString Bearer = QString("Bearer ") + AuthToken;
    request.setRawHeader("Authorization", Bearer.toUtf8());

    networkreply = networkmanager->post(request, QJsonDocument(Params).toJson());
}

void RequestDropboxDirList::DoRequestContinued()
{
    QJsonObject Params;
    Params["cursor"] = QJsonValue(cursor);

    QNetworkRequest request;
    request.setUrl(QUrl("https://api.dropboxapi.com/2/files/list_folder/continue"));
    request.setRawHeader("Content-Type", "application/json");
    QString Bearer = QString("Bearer ") + AuthToken;
    request.setRawHeader("Authorization", Bearer.toUtf8());

    networkreply = networkmanager->post(request, QJsonDocument(Params).toJson());
}

void RequestDropboxDirList::handleRequestDone(QNetworkReply* reply)
{
    QByteArray response_data = reply->readAll();
    bool HasMore = parseJson(response_data);
    if (HasMore)
    {
        DoRequestContinued();
    }
    else
    {
        std::sort(ListOfEntries.begin(), ListOfEntries.end(), compareByPath);
        emit RequestFinished();
    }
}

bool RequestDropboxDirList::parseJson(QByteArray response_data)
{
    bool ResponseComplete = false;
    QJsonDocument JSonDoc = QJsonDocument().fromJson(response_data);
    QJsonObject JSonObj= JSonDoc.object();

    QJsonArray DirEntries = JSonObj["entries"].toArray();
    foreach (const QJsonValue & entry, DirEntries)
    {
        QJsonObject obj = entry.toObject();

        QString tag = obj[".tag"].toString();
        if (tag != "folder")
        {
            Entrie_str fe;
            fe.id = obj["id"].toString();
            fe.rev = obj["rev"].toString();
            //"2015-05-12T15:50:38Z",
            QString TimeAsString = obj["client_modified"].toString();
            fe.myModTimestamp = QDateTime().fromString(TimeAsString,"yyyy-MM-dd'T'HH:mm:ssZ");
            TimeAsString = obj["server_modified"].toString();
            fe.srvModTimestamp = QDateTime().fromString(TimeAsString,"yyyy-MM-dd'T'HH:mm:ssZ");
            fe.size = obj["size"].toInt();
            fe.Pathd = obj["path_display"].toString();
            QJsonObject sharingInfo = obj["sharing_info"].toObject();
            fe.ReadOnly = sharingInfo["read_only"].toBool();
            ListOfEntries.append(fe);
        }
    }

    cursor = JSonObj["cursor"].toString();
    ResponseComplete = JSonObj["has_more"].toBool();

    return ResponseComplete;
}
