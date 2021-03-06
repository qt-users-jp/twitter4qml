/* Copyright (c) 2012-2013 Twitter4QML Project.
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Twitter4QML nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL TWITTER4QML BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "status.h"
#include "statusesshow.h"
#include "statusesupdate.h"
#include "statusesupdatewithmedia.h"
#include "statusesretweet.h"
#include "statusesdestroy.h"

#include "datamanager.h"
#include "../favorites/favoritescreate.h"
#include "../favorites/favoritesdestroy.h"
#include "../media/mediaupload.h"
#include "user.h"

#include "../utils.h"

#include <QtCore/QMetaProperty>

class Status::Private : public QObject
{
    Q_OBJECT
public:
    Private(Status *parent);

    void update(const QVariantMap &parameters);
    void retweet(const QVariantMap &parameters);
    void upload(const QVariantMap &parameters);
    void destroy();
    void favorite();
    void unfavorite();

private slots:
    void id_strChanged(const QString &id_str);
    void dataChanged(const QVariant &data);

private:
    Status *q;
    QList<AbstractTwitterAction *> tasks;
};

Status::Private::Private(Status *parent)
    : QObject(parent)
    , q(parent)
{
    connect(q, SIGNAL(id_strChanged(QString)), this, SLOT(id_strChanged(QString)));
}

void Status::Private::id_strChanged(const QString &id)
{
    q->contributors(QVariantList());
    q->coordinates(QVariantMap());
    q->current_user_retweet(QVariantList());
    q->entities(QVariantMap());
    q->favorite_count(0);
    q->favorited(false);
    q->geo(QVariantMap());
    q->in_reply_to_screen_name(QString());
    q->in_reply_to_status_id(0);
    q->in_reply_to_status_id_str(QString());
    q->in_reply_to_user_id(0);
    q->in_reply_to_user_id_str(QString());
    q->lang(QString());
    q->place(QVariantMap());
    q->possibly_sensitive(false);
    q->retweet_count(0);
    q->retweeted(false);
    q->retweeted_status(QVariantMap());
    q->source(QString());
    q->text(QString());
    q->plain_text(QString());
    q->rich_text(QString());
    q->truncated(false);
    q->user(QVariantMap());
    q->media(QVariantList());
    q->extended_media(QVariantList());

    if (id.isEmpty()) {
    } else {
        StatusesShow *action = new StatusesShow(this);
        action->id(id);
        connect(action, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
        if (q->m_loading) {
            tasks.append(action);
        } else {
            q->loading(true);
            action->exec();
        }
    }
}

void Status::Private::update(const QVariantMap &parameters)
{
    AbstractTwitterAction *action = 0;
    if (parameters.contains(QStringLiteral("media")) && !parameters.value(QStringLiteral("media")).toList().isEmpty()) {
        StatusesUpdateWithMedia *act = new StatusesUpdateWithMedia(this);
        act->status(parameters.value(QStringLiteral("status")).toString());
        act->latitude(parameters.value(QStringLiteral("_lat")).toDouble());
        act->longitude(parameters.value(QStringLiteral("_long")).toDouble());
        act->place_id(parameters.value(QStringLiteral("place_id")).toString());
        act->media(parameters.value(QStringLiteral("media")).toList());
        act->in_reply_to_status_id(parameters.value(QStringLiteral("in_reply_to_status_id")).toString());
        act->trim_user(false);
        act->include_entities(true);
        connect(act, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
        action = act;
    } else {
        StatusesUpdate *act = new StatusesUpdate(this);
        act->status(parameters.value(QStringLiteral("status")).toString());
        act->latitude(parameters.value(QStringLiteral("_lat")).toDouble());
        act->longitude(parameters.value(QStringLiteral("_long")).toDouble());
        act->place_id(parameters.value(QStringLiteral("place_id")).toString());
        act->in_reply_to_status_id(parameters.value(QStringLiteral("in_reply_to_status_id")).toString());
        act->trim_user(false);
        act->include_entities(true);
        act->media_ids(parameters.value(QStringLiteral("media_ids")).toStringList());
        connect(act, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
        action = act;
    }
    if (q->loading()) {
        tasks.append(action);
    } else {
       q->loading(true);
       action->exec();
    }
}

void Status::Private::retweet(const QVariantMap &parameters)
{
    StatusesRetweet *action = new StatusesRetweet(this);
    action->id(parameters.value(QStringLiteral("id")).toString());
    action->trim_user(true);
    action->include_entities(true);
    connect(action, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
    if (q->loading()) {
        tasks.append(action);
    } else {
       q->loading(true);
       action->exec();
    }
}

void Status::Private::upload(const QVariantMap &parameters)
{
    MediaUpload *action = new MediaUpload(this);
    action->media(parameters.value(QStringLiteral("media")).toString());
    connect(action, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
    if (q->loading()) {
        tasks.append(action);
    } else {
       q->loading(true);
       action->exec();
    }
}

void Status::Private::destroy()
{
    StatusesDestroy *action = new StatusesDestroy(this);
    action->id(q->m_id_str);
    action->include_entities(true);
    connect(action, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
    if (q->loading()) {
        tasks.append(action);
    } else {
       q->loading(true);
       action->exec();
    }
}

void Status::Private::favorite()
{
    AbstractFavoritesAction *action = new FavoritesCreate(this);
    action->id(q->m_id_str);
    action->include_entities(true);
    connect(action, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
    if (q->loading()) {
        tasks.append(action);
    } else {
       q->loading(true);
       action->exec();
    }
}

void Status::Private::unfavorite()
{
    AbstractFavoritesAction *action = new FavoritesDestroy(this);
    action->id(q->m_id_str);
    action->include_entities(true);
    connect(action, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
    if (q->loading()) {
        tasks.append(action);
    } else {
       q->loading(true);
       action->exec();
    }
}

void Status::Private::dataChanged(const QVariant &data)
{
    if (qobject_cast<StatusesDestroy *>(sender())) {
        DEBUG() << data;
        q->id_str(QString());
        emit q->dataChanged();
        qDeleteAll(tasks);
        tasks.clear();
        q->loading(false);
    }
    QObject *action = qobject_cast<QObject *>(sender());
    if (action) {
        QVariantMap status = Status::parse(data.toMap());
        const QMetaObject *mo = q->metaObject();
        for (int i = 0; i < mo->propertyCount(); i++) {
            QMetaProperty prop = mo->property(i);
            if (!prop.isDesignable()) continue;
            const char *key = prop.name();
            if (status.contains(QString::fromUtf8(key))) {
                q->setProperty(key, status.value(QString::fromUtf8(key)));
            } else {
                q->setProperty(key, QVariant());
            }
        }
        if (!qobject_cast<StatusesRetweet *>(action)) {
            DataManager::instance()->addData(DataManager::StatusData, q->id_str(), status);
        }
        emit q->dataChanged();
        action->deleteLater();
    }
    if (tasks.isEmpty()) {
        q->loading(false);
    } else {
        tasks.takeFirst()->exec();
    }
}


Status::Status(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
    , m_loading(false)
    , m_favorited(false)
    , m_possibly_sensitive(false)
    , m_retweet_count(0)
    , m_retweeted(false)
    , m_truncated(false)
{
}

void Status::statusesUpdate(QVariantMap parameters)
{
    d->update(parameters);
}

void Status::statusesRetweet(QVariantMap parameters)
{
    d->retweet(parameters);
}

void Status::mediaUpload(QVariantMap parameters)
{
    d->upload(parameters);
}

void Status::statusesDestroy()
{
    d->destroy();
}

void Status::favorite()
{
    d->favorite();
}

void Status::unfavorite()
{
    d->unfavorite();
}

QVariantMap Status::data() const
{
    QVariantMap ret;
    const QMetaObject *mo = metaObject();
    for (int i = 0; i < mo->propertyCount(); i++) {
        QMetaProperty prop = mo->property(i);
        if (!prop.isDesignable()) continue;
        const char *key = prop.name();
        ret.insert(QString::fromUtf8(key), property(key));
    }
    return ret;
}

bool Status::indicesGreaterThan(const QVariant &v1, const QVariant &v2)
{
    return v1.toMap().value(QStringLiteral("indices")).toList().first().toInt() > v2.toMap().value(QStringLiteral("indices")).toList().first().toInt();
}

void Status::debug() const
{
    const QMetaObject *mo = metaObject();
    for (int i = 0; i < mo->propertyCount(); i++) {
        QMetaProperty prop = mo->property(i);
        if (!prop.isDesignable()) continue;
        const char *key = prop.name();
        DEBUG() << key << property(key);
    }
}

QVariantMap Status::parse(const QVariantMap &status)
{
    QVariantMap ret = status;

    ret.insert(QStringLiteral("user"), User::parse(ret.value(QStringLiteral("user")).toMap()));

    QString text = ret.value(QStringLiteral("text")).toString();
    if (ret.contains(QStringLiteral("entities")) && !ret.value(QStringLiteral("entities")).isNull()) {
//        DEBUG() << text;
        QString plain_text = text;
        QString rich_text = text.replace(QStringLiteral(" "), QStringLiteral("\t"));
        QVariantList entitiesSortedByIndices;
        QVariantMap entities = ret.value(QStringLiteral("entities")).toMap();
        foreach (const QString &key, entities.keys()) {
            QVariant entity = entities.value(key);
            if (entity.type() == QVariant::List) {
                QVariantList e = entity.toList();
                foreach (const QVariant &ee, e) {
                    QVariantMap eee = ee.toMap();
                    eee.insert(QStringLiteral("type"), key);
                    entitiesSortedByIndices.append(eee);
                }
            } else {
                DEBUG() << entity;
            }
        }
        std::sort(entitiesSortedByIndices.begin(), entitiesSortedByIndices.end(), &Status::indicesGreaterThan);
        QVariantList media;
        foreach (const QVariant &item, entitiesSortedByIndices) {
            QVariantMap entity = item.toMap();
            QVariantList indices = entity.value(QStringLiteral("indices")).toList();
            int start = indices.first().toInt();
            int end = indices.last().toInt();
            QString type = entity.value(QStringLiteral("type")).toString();
            QString plain_textAfter;
            QString rich_textAfter;
            if (type == QStringLiteral("urls")) {
                if (entity.contains(QStringLiteral("display_url"))) {
                    plain_textAfter = entity.value(QStringLiteral("display_url")).toString();
                    rich_textAfter = QStringLiteral("<a class=\"link\" href=\"")
                            .append(entity.value(QStringLiteral("expanded_url")).toString())
                            .append(QStringLiteral("\" title=\""))
                            .append(entity.value(QStringLiteral("url")).toString())
                            .append(QStringLiteral("\">"))
                            .append(entity.value(QStringLiteral("display_url")).toString())
                            .append(QStringLiteral("</a>"));
                } else {
                    plain_textAfter = entity.value(QStringLiteral("url")).toString();
                    rich_textAfter = QStringLiteral("<a class=\"link\" href=\"")
                            .append(entity.value(QStringLiteral("url")).toString())
                            .append(QStringLiteral("\" title=\""))
                            .append(entity.value(QStringLiteral("url")).toString())
                            .append(QStringLiteral("\">"))
                            .append(entity.value(QStringLiteral("url")).toString())
                            .append(QStringLiteral("</a>"));
                }
            } else if (type == QStringLiteral("user_mentions")) {
                rich_textAfter = QStringLiteral("<a class=\"screen_name\" href=\"user://%1\" title=\"@%2\">@%2</a>")
                        .arg(entity.value(QStringLiteral("id_str")).toString())
                        .arg(entity.value(QStringLiteral("screen_name")).toString());
            } else if (type == QStringLiteral("hashtags")) {
                rich_textAfter = QStringLiteral("<a class=\"hash_tag\" href=\"search://#%1\" title=\"#%2\">#%2</a>")
                        .arg(entity.value(QStringLiteral("text")).toString())
                        .arg(entity.value(QStringLiteral("text")).toString());
            } else if (type == QStringLiteral("media")) {
                plain_textAfter = entity.value(QStringLiteral("display_url")).toString();
                rich_textAfter = QStringLiteral("<a class=\"media\" href=\"")
                        .append(entity.value(QStringLiteral("media_url")).toString())
                        .append(QStringLiteral("\" title=\""))
                        .append(entity.value(QStringLiteral("url")).toString())
                        .append(QStringLiteral("\">"))
                        .append(entity.value(QStringLiteral("display_url")).toString())
                        .append(QStringLiteral("</a>"));
                media.append(entity.value(QStringLiteral("media_url")));
            } else {
                DEBUG() << type << item;
            }
            if (!plain_textAfter.isNull())
                plain_text.replace(start, end - start, plain_textAfter);
            if (!rich_textAfter.isNull())
                rich_text.replace(start, end - start, rich_textAfter);
        }

        QVariantList extended_entitiesSortedByIndices;
        QVariantMap extended_entities = ret.value(QStringLiteral("extended_entities")).toMap();
        foreach (const QString &key, extended_entities.keys()) {
            QVariant extended_entity = extended_entities.value(key);
            if (extended_entity.type() == QVariant::List) {
                QVariantList e = extended_entity.toList();
                foreach (const QVariant &ee, e) {
                    QVariantMap eee = ee.toMap();
                    eee.insert(QStringLiteral("type"), key);
                    extended_entitiesSortedByIndices.append(eee);
                }
            } else {
                DEBUG() << extended_entity;
            }
        }
        std::sort(extended_entitiesSortedByIndices.begin(), extended_entitiesSortedByIndices.end(), &Status::indicesGreaterThan);
        QVariantList extended_media;
        foreach (const QVariant &item, extended_entitiesSortedByIndices) {
            QVariantMap extended_entity = item.toMap();
            QString type = extended_entity.value(QStringLiteral("type")).toString();
            if (type == QStringLiteral("media")) {
                extended_media.append(extended_entity.value(QStringLiteral("media_url")));
            } else {
                DEBUG() << type << item;
            }
        }


//        DEBUG() << ret.value("text").toString();
        ret.insert(QStringLiteral("plain_text"), escapeHtml(plain_text));
//        DEBUG() << ret.value("plain_text").toString();
        ret.insert(QStringLiteral("rich_text"), rich_text.replace(QStringLiteral("\n"), QStringLiteral("<br />")).replace(QStringLiteral("\t"), QStringLiteral("&nbsp;")).replace(QString::fromUtf8("　"), QStringLiteral("&nbsp;&nbsp;&nbsp;&nbsp;")));
//        DEBUG() << ret.value("rich_text").toString();
        ret.insert(QStringLiteral("media"), media);
        ret.insert(QStringLiteral("extended_media"), extended_media);
    } else {
        DEBUG() << text;
        if (!ret.contains(QStringLiteral("plain_text")))
            ret.insert(QStringLiteral("plain_text"), escapeHtml(text));
        if (!ret.contains(QStringLiteral("rich_text")))
            ret.insert(QStringLiteral("rich_text"), text.replace(QStringLiteral(" "), QStringLiteral("&nbsp;")).replace(QStringLiteral("\n"), QStringLiteral("<br />")).replace(QString::fromUtf8("　"), QStringLiteral("&nbsp;&nbsp;&nbsp;&nbsp;")));
    }

    if (ret.contains(QStringLiteral("retweeted_status")) && !ret.value(QStringLiteral("retweeted_status")).isNull()) {
        ret.insert(QStringLiteral("retweeted_status"), Status::parse(ret.value(QStringLiteral("retweeted_status")).toMap()));
    }
    return ret;
}

#include "status.moc"
