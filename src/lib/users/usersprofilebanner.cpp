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

#include "usersprofilebanner.h"
#include <QtCore/QMetaProperty>

class UsersProfileBanner::Private : public QObject
{
    Q_OBJECT
public:
    Private(UsersProfileBanner *parent);

private slots:
    void user_idChanged();
    void screen_nameChanged();
    void dataChanged(const QVariant &data);

private:
    UsersProfileBanner *q;
};

UsersProfileBanner::Private::Private(UsersProfileBanner *parent)
    : QObject(parent)
    , q(parent)
{
    connect(q, SIGNAL(user_idChanged(QString)), this, SLOT(user_idChanged()), Qt::QueuedConnection);
    connect(q, SIGNAL(screen_nameChanged(QString)), this, SLOT(screen_nameChanged()), Qt::QueuedConnection);
    connect(q, SIGNAL(dataChanged(QVariant)), this, SLOT(dataChanged(QVariant)), Qt::QueuedConnection);
}

void UsersProfileBanner::Private::user_idChanged()
{
    if (q->isLoading()) return;
    q->exec();
}

void UsersProfileBanner::Private::screen_nameChanged()
{
    if (q->isLoading()) return;
    q->exec();
}

void UsersProfileBanner::Private::dataChanged(const QVariant &data)
{
    QVariantMap map = parse(data.toMap());
    if (!map.keys().isEmpty()) {
        foreach (const QString &size, map.keys()) {
            const QByteArray key = size.toLocal8Bit();
            q->setProperty(key, map.value(size).toMap());
        }
    }
}

UsersProfileBanner::UsersProfileBanner(QObject *parent)
    : AbstractTwitterAction(parent)
    , d(new Private(this))
{
}

QVariantMap UsersProfileBanner::data() const
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

QVariantMap UsersProfileBanner::parse(const QVariantMap &usersProfileBanner)
{
    QVariantMap ret = usersProfileBanner.value(QStringLiteral("sizes")).toMap();
    return ret;
}

#include "usersprofilebanner.moc"
