#ifndef REQUESTDROPBOXDIRLIST_H
#define REQUESTDROPBOXDIRLIST_H

#include <QObject>
#include <QNetworkRequest>
#include <QDateTime>


class QNetworkAccessManager;
class QNetworkReply;
class QJsonArray;

struct Entrie_str {
    QString id;
    QString Pathd;
    QString rev;
    //"2015-05-12T15:50:38Z",
    QDateTime myModTimestamp;
    QDateTime srvModTimestamp;
    int size;
    bool ReadOnly;
};

class RequestDropboxDirList : public QObject
{
    Q_OBJECT
public:
    explicit RequestDropboxDirList(QString token, QObject *parent = 0);
    void DoRequest();
    QList<Entrie_str> getDirEntries() { return ListOfEntries;};
signals:
    void RequestFinished();

public slots:

private slots:
    void handleRequestDone(QNetworkReply *reply);

private:
    void DoRequestContinued();
    bool parseJson(QByteArray response_data);
    QNetworkAccessManager* networkmanager;
    QNetworkReply * networkreply;
    QJsonArray* _Entries;
    QString cursor;
    QList<Entrie_str> ListOfEntries;
    QString AuthToken;
    static bool compareByPath(const Entrie_str &b1, const Entrie_str &b2) {return b1.Pathd < b2.Pathd;};
};

#endif // REQUESTDROPBOXDIRLIST_H
