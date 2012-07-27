#ifndef RETWEETEDBYME_H
#define RETWEETEDBYME_H

#include "abstractstatusesmodel.h"

class RetweetedByMe : public AbstractStatusesModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(QString since_id READ sinceId WRITE setSinceId NOTIFY sinceIdChanged)
    Q_PROPERTY(QString max_id READ maxId WRITE setMaxId NOTIFY maxIdChanged)
    Q_PROPERTY(int page READ page WRITE setPage NOTIFY pageChanged)
    Q_PROPERTY(bool trim_user READ trimUser WRITE setTrimUser NOTIFY trimUserChanged)
    Q_PROPERTY(bool include_entities READ includeEntities WRITE setIncludeEntities NOTIFY includeEntitiesChanged)
    Q_DISABLE_COPY(RetweetedByMe)
public:
    explicit RetweetedByMe(QObject *parent = 0);

signals:
    void countChanged(int count);
    void sinceIdChanged(const QString &sinceId);
    void maxIdChanged(const QString &maxId);
    void pageChanged(int page);
    void trimUserChanged(bool trimUser);
    void includeEntitiesChanged(bool includeEntities);

protected:
    QUrl api() const { return QUrl("http://api.twitter.com/1/statuses/retweeted_by_me.json"); }
};

#endif // RETWEETEDBYME_H
