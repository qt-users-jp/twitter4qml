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

#include "directmessage.h"
#include "directmessagesshow.h"
#include "directmessagesnew.h"
#include "directmessagesdestroy.h"

#include "datamanager.h"
#include "user.h"

#include "utils.h"

#include <QtCore/QMetaProperty>

class DirectMessage::Private : public QObject
{
    Q_OBJECT
public:
    Private(DirectMessage *parent);

    bool loading;

    void create(const QVariantMap &parameters);
    void destroy();

private slots:
    void setLoading(bool loading);
    void id_strChanged(const QString &id_str);
    void dataChanged(const QVariant &data);

private:
    DirectMessage *q;
    QList<AbstractTwitterAction *> tasks;
};

DirectMessage::Private::Private(DirectMessage *parent)
    : QObject(parent)
    , loading(false)
    , q(parent)
{
    connect(q, SIGNAL(id_strChanged(QString)), this, SLOT(id_strChanged(QString)));
}

void DirectMessage::Private::setLoading(bool l)
{
    if (loading == l) return;
    loading = l;
    emit q->loadingChanged(l);
}

void DirectMessage::Private::id_strChanged(const QString &id)
{
    q->created_at(QString());
    q->entities(QVariantMap());
    q->recipient(QVariantMap());
    q->recipient_id(QString());
    q->recipient_screen_name(QString());
    q->sender(QVariantMap());
    q->sender_id(QString());
    q->sender_screen_name(QString());
    q->text(QString());
    q->plain_text(QString());
    q->rich_text(QString());
    q->media(QVariantList());
    if (id.isEmpty()) {
    } else {
        DirectMessagesShow *action = new DirectMessagesShow(this);
        action->id(id);
        connect(action, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
        if (loading) {
            tasks.append(action);
        } else {
            setLoading(true);
            action->exec();
        }
    }
}

void DirectMessage::Private::create(const QVariantMap &parameters)
{
    DirectMessagesNew *action = new DirectMessagesNew(this);
    action->user_id(parameters.value(QStringLiteral("user_id")).toString());
    action->text(parameters.value(QStringLiteral("text")).toString());
    connect(action, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
    if (loading) {
        tasks.append(action);
    } else {
       setLoading(true);
       action->exec();
    }
}

void DirectMessage::Private::destroy()
{
    DirectMessagesDestroy *action = new DirectMessagesDestroy(this);
    action->id(q->id_str());
    action->include_entities(true);
    connect(action, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)));
    if (loading) {
        tasks.append(action);
    } else {
       setLoading(true);
       action->exec();
    }
}

void DirectMessage::Private::dataChanged(const QVariant &data)
{
    if (qobject_cast<DirectMessagesDestroy *>(QObject::sender())) {
        DEBUG() << data;
        q->id_str(QString());
        emit q->dataChanged();
        qDeleteAll(tasks);
        tasks.clear();
        setLoading(false);
    }
    AbstractTwitterAction *action = qobject_cast<AbstractTwitterAction *>(QObject::sender());
    if (action) {
        QVariantMap directMessage = DirectMessage::parse(data.toMap());
        const QMetaObject *mo = q->metaObject();
        for (int i = 0; i < mo->propertyCount(); i++) {
            QMetaProperty prop = mo->property(i);
            if (!prop.isDesignable()) continue;
            const char *key = prop.name();
            if (directMessage.contains(QString::fromUtf8(key))) {
                q->setProperty(key, directMessage.value(QString::fromUtf8(key)));
            } else {
                q->setProperty(key, QVariant());
            }
        }
        DataManager::instance()->addData(DataManager::DirectMessageData, q->id_str(), directMessage);
        emit q->dataChanged();
        action->deleteLater();
    }
    if (tasks.isEmpty()) {
        setLoading(false);
    } else {
        tasks.takeFirst()->exec();
    }
}


DirectMessage::DirectMessage(QObject *parent)
    : QObject(parent)
    , d(new Private(this))
{
}

void DirectMessage::directMessagesNew(QVariantMap parameters)
{
    d->create(parameters);
}

void DirectMessage::directMessagesDestroy()
{
    d->destroy();
}

bool DirectMessage::loading() const
{
    return d->loading;
}

QVariantMap DirectMessage::data() const
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

bool DirectMessage::indicesGreaterThan(const QVariant &v1, const QVariant &v2)
{
    return v1.toMap().value(QStringLiteral("indices")).toList().first().toInt() > v2.toMap().value(QStringLiteral("indices")).toList().first().toInt();
}

void DirectMessage::debug() const
{
    const QMetaObject *mo = metaObject();
    for (int i = 0; i < mo->propertyCount(); i++) {
        QMetaProperty prop = mo->property(i);
        if (!prop.isDesignable()) continue;
        const char *key = prop.name();
        DEBUG() << key << property(key);
    }
}

QVariantMap DirectMessage::parse(const QVariantMap &directMessage)
{
    QVariantMap ret = directMessage;

    ret.insert(QStringLiteral("recipient"), User::parse(ret.value(QStringLiteral("recipient")).toMap()));
    ret.insert(QStringLiteral("sender"), User::parse(ret.value(QStringLiteral("sender")).toMap()));

    QString text = ret.value(QStringLiteral("text")).toString();
    if (ret.contains(QStringLiteral("entities")) && !ret.value(QStringLiteral("entities")).isNull()) {
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
        std::sort(entitiesSortedByIndices.begin(), entitiesSortedByIndices.end(), &DirectMessage::indicesGreaterThan);
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
        ret.insert(QStringLiteral("plain_text"), escapeHtml(plain_text));
        ret.insert(QStringLiteral("rich_text"), rich_text.replace(QStringLiteral("\n"), QStringLiteral("<br />")).replace(QStringLiteral("\t"), QStringLiteral("&nbsp;")).replace(QString::fromUtf8("　"), QStringLiteral("&nbsp;&nbsp;&nbsp;&nbsp;")));
        ret.insert(QStringLiteral("media"), media);
    } else {
        DEBUG() << text;
        if (!ret.contains(QStringLiteral("plain_text")))
            ret.insert(QStringLiteral("plain_text"), escapeHtml(text));
        if (!ret.contains(QStringLiteral("rich_text")))
            ret.insert(QStringLiteral("rich_text"), text.replace(QStringLiteral(" "), QStringLiteral("&nbsp;")).replace(QStringLiteral("\n"), QStringLiteral("<br />")).replace(QString::fromUtf8("　"), QStringLiteral("&nbsp;&nbsp;&nbsp;&nbsp;")));
    }
    return ret;
}

#include "directmessage.moc"
