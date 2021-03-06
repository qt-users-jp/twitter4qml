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

#ifndef APPLICATIONRATELIMITSTATUS_H
#define APPLICATIONRATELIMITSTATUS_H

#include "abstracttwitteraction.h"

class TWITTER4QML_EXPORT ApplicationRateLimitStatus : public AbstractTwitterAction
{
    Q_OBJECT
    Q_DISABLE_COPY(ApplicationRateLimitStatus)
    Q_PROPERTY(QVariantMap rate_limit_context READ rate_limit_context WRITE rate_limit_context NOTIFY rate_limit_contextChanged DESIGNABLE false USER true)
    Q_PROPERTY(QVariantMap resources READ resources WRITE resources NOTIFY resourcesChanged)
public:
    explicit ApplicationRateLimitStatus(QObject *parent = 0);

signals:
    void rate_limit_contextChanged(const QVariantMap &rate_limit_context);
    void resourcesChanged(const QVariantMap &resources);

protected:
    QUrl api() const { return QUrl(QStringLiteral("https://api.twitter.com/1.1/application/rate_limit_status.json")); }
    QString httpMethod() const { return QStringLiteral("GET"); }

private:
    ADD_PROPERTY(const QVariantMap &, rate_limit_context, QVariantMap)
    ADD_PROPERTY(const QVariantMap &, resources, QVariantMap)
};

#endif // APPLICATIONRATELIMITSTATUS_H
