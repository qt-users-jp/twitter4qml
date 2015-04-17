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

#ifndef USERSPROFILEBANNER_H
#define USERSPROFILEBANNER_H

#include "abstracttwitteraction.h"
#include "twitter4qml_global.h"

class TWITTER4QML_EXPORT UsersProfileBanner : public AbstractTwitterAction
{
    Q_OBJECT
    Q_PROPERTY(QString user_id READ user_id WRITE user_id NOTIFY user_idChanged)
    Q_PROPERTY(QString screen_name READ screen_name WRITE screen_name NOTIFY screen_nameChanged)
    Q_PROPERTY(QVariantMap ipad READ ipad WRITE ipad NOTIFY ipadChanged)
    Q_PROPERTY(QVariantMap ipad_retina READ ipad_retina WRITE ipad_retina NOTIFY ipad_retinaChanged)
    Q_PROPERTY(QVariantMap web READ web WRITE web NOTIFY webChanged)
    Q_PROPERTY(QVariantMap web_retina READ web_retina WRITE web_retina NOTIFY web_retinaChanged)
    Q_PROPERTY(QVariantMap mobile READ mobile WRITE mobile NOTIFY mobileChanged)
    Q_PROPERTY(QVariantMap mobile_retina READ mobile_retina WRITE mobile_retina NOTIFY mobile_retinaChanged)

    Q_DISABLE_COPY(UsersProfileBanner)
public:
    explicit UsersProfileBanner(QObject *parent = 0);
    static QVariantMap parse(const QVariantMap &usersProfileBanner);
    QVariantMap data() const;

signals:
    void user_idChanged(const QString &user_id);
    void screen_nameChanged(const QString &screen_name);
    void ipadChanged(const QVariantMap &ipad);
    void ipad_retinaChanged(const QVariantMap &ipad_retina);
    void webChanged(const QVariantMap &web);
    void web_retinaChanged(const QVariantMap &web_retina);
    void mobileChanged(const QVariantMap &mobile);
    void mobile_retinaChanged(const QVariantMap &mobile_retina);

protected:
    virtual AuthorizeBy authenticationMethod() const { return AuthorizeByHeader; }
    QUrl api() const { return QUrl(QStringLiteral("https://api.twitter.com/1.1/users/profile_banner.json")); }
    QString httpMethod() const { return QStringLiteral("GET"); }

private:
    class Private;
    Private *d;

    ADD_PROPERTY(const QString &, user_id, QString)
    ADD_PROPERTY(const QString &, screen_name, QString)
    ADD_PROPERTY(const QVariantMap &, ipad, QVariantMap)
    ADD_PROPERTY(const QVariantMap &, ipad_retina, QVariantMap)
    ADD_PROPERTY(const QVariantMap &, web, QVariantMap)
    ADD_PROPERTY(const QVariantMap &, web_retina, QVariantMap)
    ADD_PROPERTY(const QVariantMap &, mobile, QVariantMap)
    ADD_PROPERTY(const QVariantMap &, mobile_retina, QVariantMap)
};

#endif // USERSPROFILEBANNER_H
