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

#include "searchtweets.h"
#include "statusesshow.h"
#include "status.h"
#include "../utils.h"
#include <QtCore/QQueue>

SearchTweets::SearchTweets(QObject *parent)
    : AbstractStatusesModel(parent)
    , m_result_type(QStringLiteral("mixed"))
{
}

void SearchTweets::parseDone(const QVariant &result)
{
    if (result.type() == QVariant::Map) {
        QVariantMap object = result.toMap();
        if (object.contains(QStringLiteral("search_metadata")))
            search_metadata(object.value(QStringLiteral("search_metadata")).toMap());
//        if (object.contains("query"))
//            setQ(object.value("query").toString());
        if (object.contains(QStringLiteral("statuses")) && object.value(QStringLiteral("statuses")).type() == QVariant::List) {
            QVariantList results = object.value(QStringLiteral("statuses")).toList();
            if (results.isEmpty()) {
                emit loadingChanged(false);
            } else {
                foreach (const QVariant &result, results) {
                    if (result.type() == QVariant::Map) {
                        addData(SearchTweets::parse(result.toMap()));
                    }
                }
            }
        } else {
            DEBUG() << object;
        }
    }
}

void SearchTweets::dataAdded(const QString &key, const QVariantMap &value)
{
    Q_UNUSED(key)
    if (value.value(QStringLiteral("text")).toString().contains(QString::fromUtf8(QByteArray::fromPercentEncoding(q().toUtf8())), Qt::CaseInsensitive)) {
        addData(value);
    }
}

QVariantMap SearchTweets::parse(const QVariantMap &status)
{
    return Status::parse(status);
}
