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

#include "abstractstatusesidsmodel.h"
#include <QtCore/QTimer>

class AbstractStatusesIdsModel::Private
{
public:
    QTimer timer;
};

AbstractStatusesIdsModel::AbstractStatusesIdsModel(QObject *parent)
    : AbstractTwitterModel(parent)
    , d(new Private)
    , m_stringify_ids(true)
    , m_next_cursor(0)
    , m_previous_cursor(0)
{
    QHash<int, QByteArray> roles;
    roles[id_role] = "id";
    roles[id_str_role] = "id_str";
    setRoleNames(roles);

    connect(&d->timer, SIGNAL(timeout()), this, SLOT(reload()));
    d->timer.setSingleShot(true);
    d->timer.setInterval(0);

    connect(this, SIGNAL(idChanged(QString)), &d->timer, SLOT(start()));
}

AbstractStatusesIdsModel::~AbstractStatusesIdsModel()
{
    delete d;
}

void AbstractStatusesIdsModel::reload() {
    if (!id().isEmpty()) {
        AbstractTwitterModel::reload();
    }
}

void AbstractStatusesIdsModel::parseDone(const QVariant &data)
{
    if (data.type() == QVariant::Map) {
        QVariantMap object = data.toMap();
        if (object.contains(QStringLiteral("ids")) && object.value(QStringLiteral("ids")).type() == QVariant::List) {
            QVariantList results = object.value(QStringLiteral("ids")).toList();
            foreach (const QVariant &result, results) {
                QVariantMap map;
                switch(result.type()) {
                case QVariant::Int:
                    map.insert(QStringLiteral("id"), result);
                    addData(map);
                    break;
                case QVariant::String:
                    map.insert(QStringLiteral("id_str"), result);
                    addData(map);
                    break;
                default:
                    break;
                }
            }
        } else {
            DEBUG() << data;
        }
        if (object.contains(QStringLiteral("next_cursor")))
            next_cursor(object.value(QStringLiteral("next_cursor")).toInt());
        if (object.contains(QStringLiteral("next_cursor_str")))
            next_cursor_str(object.value(QStringLiteral("next_cursor_str")).toString());
        if (object.contains(QStringLiteral("previous_cursor")))
            previous_cursor(object.value(QStringLiteral("previous_cursor")).toInt());
        if (object.contains(QStringLiteral("previous_cursor_str")))
            previous_cursor_str(object.value(QStringLiteral("previous_cursor_str")).toString());
    }
}
