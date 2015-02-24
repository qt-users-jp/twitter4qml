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

#include "oauthmanager.h"
#include "hmac_sha1.h"

#include <QtCore/QByteArray>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>
#if QT_VERSION >= 0x050000
#include <QtCore/QUrlQuery>
#endif
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

class OAuthManager::Private : public QObject {
    Q_OBJECT
public:
    Private(OAuthManager *parent);

    QNetworkReply *request(const QString &method, const QUrl &url, const QMultiMap<QString, QByteArray> &params, bool multiPart, const QByteArray &acceptEncoding = QByteArray());
    QNetworkReply *echo(const QString &method, const QUrl &url, const QMultiMap<QString, QByteArray> &params, const QUrl& serviceProvider, const QUrl &realm, bool multiPart = false);
    bool updateToken(QNetworkReply* rep);

    QMap<QString, QByteArray> requestParams;

    QNetworkAccessManager *networkAccessManager;
    AuthorizeBy authorizeBy;

    QString consumerKey;
    QString consumerSecret;
    QString token;
    QString tokenSecret;
    QString user_id;
    QString screen_name;
    bool isAuthorized;

private slots:
    void setToken(const QString &token);

private:
    OAuthManager *q;

    QByteArray parameterEncoding(const QString &str) const;
    QByteArray normalize(const QMultiMap<QString, QByteArray> &param) const;
    QByteArray signature(const QString &method, const QUrl &url, const QByteArray &params) const;
    QMultiMap<QString, QByteArray> signatureParams(const QMultiMap<QString, QByteArray> &params) const;
    QByteArray authHeader() const;
    QByteArray authHeader(const QMultiMap<QString, QByteArray> &params) const;
    QByteArray authHeader(const QMultiMap<QString, QByteArray> &params, const QUrl &realm) const;
    void getTokenCredential();
};

OAuthManager::Private::Private(OAuthManager *parent)
    : QObject(parent)
    , networkAccessManager(0)
    , authorizeBy(AuthorizeByHeader)
    , isAuthorized(false)
    , q(parent)
{
    connect(q, SIGNAL(tokenChanged(QString)), this, SLOT(setToken(QString)));
}

void OAuthManager::Private::setToken(const QString &token)
{
    if (token.isEmpty()) {
        requestParams.clear();
    } else if (!requestParams.isEmpty()){
        requestParams[QStringLiteral("oauth_token")] = token.toUtf8();
    }
}


QNetworkReply *OAuthManager::Private::request(const QString &method, const QUrl &url, const QMultiMap<QString, QByteArray> &params, bool multiPart, const QByteArray &acceptEncoding)
{
//    DEBUG() << method << url << params << multiPart;
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
    request.setRawHeader("Accept-Encoding",acceptEncoding);

    QNetworkReply *ret = 0;

    QDateTime dt = QDateTime::currentDateTimeUtc();
    QString timestamp;
    QString nonce;
    timestamp.setNum(dt.toTime_t());
    nonce.setNum(dt.toMSecsSinceEpoch());

    if (requestParams.isEmpty()) {
        requestParams[QStringLiteral("oauth_version")] = "1.0";
        requestParams[QStringLiteral("oauth_signature_method")] = "HMAC-SHA1";
        requestParams[QStringLiteral("oauth_consumer_key")] = consumerKey.toUtf8();
        if (!token.isEmpty())
            requestParams[QStringLiteral("oauth_token")] = token.toUtf8();
    }
    requestParams[QStringLiteral("oauth_nonce")] = nonce.toUtf8();
    requestParams[QStringLiteral("oauth_timestamp")] = timestamp.toUtf8();
//    DEBUG() << url.host();
    if ((url.path().endsWith(QStringLiteral("update_with_media.json"), Qt::CaseInsensitive))
        || (url.path().endsWith(QStringLiteral("upload.json"), Qt::CaseInsensitive))) {
        requestParams[QStringLiteral("oauth_signature")] =  signature(method,
                                                      url,
                                                      normalize(signatureParams(QMultiMap<QString, QByteArray>())));
    } else {
        requestParams[QStringLiteral("oauth_signature")] =  signature(method,
                                                      url,
                                                      normalize(signatureParams(params)));
    }
    if (method == QStringLiteral("POST")) {
        switch (authorizeBy) {
        case AuthorizeByHeader:
            if (multiPart) {
                request.setRawHeader("Authorization", authHeader());
                request.setUrl(url);
                QString boundary = QStringLiteral("--------------------%1").arg(nonce);
                QByteArray body;
                QList<QString> keys = params.keys();
                foreach(const QString &key, keys) {
                    QList<QByteArray> vals = params.values(key);
                    foreach(const QByteArray &val, vals) {
                        body.append(QStringLiteral("--%1\r\n").arg(boundary).toUtf8());
                        if (key == QStringLiteral("media[]") || key == QStringLiteral("media")) {
                            QUrl url;
                            if (val.startsWith(":")) {
                                url = QUrl(QStringLiteral("qrc%1").arg(QString::fromUtf8(val)));
                            } else {
                                url = QUrl::fromLocalFile(QString::fromUtf8(val));
                            }
                            QFileInfo info(url.path());
                            QFile file(QString::fromUtf8(val));
                            QByteArray data;
                            if (file.open(QFile::ReadOnly)) {
                                data = file.readAll();
                                DEBUG() << data.size();
                                file.close();
                            } else {
                                DEBUG() << val << file.errorString();
                            }
                            body.append(QStringLiteral("Content-Disposition: form-data; name=\"%1\"; filename=\"%2\"\r\n").arg(key).arg(info.fileName()).toUtf8());
                            body.append("Content-Type: application/octet-stream\r\n");
//                            body.append("Content-Transfer-Encoding: binary\r\n");
                            body.append("\r\n");
                            body.append(data);
                            body.append("\r\n");
                        } else {
                            body.append(QStringLiteral("Content-Disposition: form-data; name=\"%1\"\r\n").arg(key).toUtf8());
                            body.append("\r\n");
                            body.append(val);
                            body.append("\r\n");
                        }
                        DEBUG() << key << val << body.size();
                    }
                }
                body.append(QStringLiteral("\r\n--%1--\r\n").arg(boundary).toUtf8());
                DEBUG() << body.data();
                request.setHeader(QNetworkRequest::ContentLengthHeader, QString::number(body.length()).toUtf8());
                request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("multipart/form-data; boundary=%1").arg(boundary).toUtf8());

                foreach (const QByteArray &rawHeader, request.rawHeaderList()) {
                    DEBUG() << rawHeader << request.rawHeader(rawHeader);
                }

                ret = q->networkAccessManager()->post(request, body);
            } else {
                request.setHeader(QNetworkRequest::ContentTypeHeader, QLatin1String("application/x-www-form-urlencoded"));
                request.setRawHeader("Authorization", authHeader());
                request.setUrl(url);
                ret = q->networkAccessManager()->post(request, normalize(params));
            }
            break;
        case AuthorizeByBody: {
            QUrl qurl(url);
#if QT_VERSION >= 0x050000
            QUrlQuery query;
#endif
            QList<QString> keys = requestParams.keys();
            foreach(const QString &key, keys) {
                QList<QByteArray> vals = requestParams.values(key);
                foreach(const QByteArray &val, vals) {
#if QT_VERSION >= 0x050000
                    query.addQueryItem(key, QString::fromUtf8(val));
#else
                    qurl.addEncodedQueryItem(parameterEncoding(key), val.toPercentEncoding());
#endif
                }
            }
#if QT_VERSION >= 0x050000
            qurl.setQuery(query);
#endif
            request.setUrl(qurl);
            ret = q->networkAccessManager()->post(request, normalize(params));
            DEBUG() << qurl << normalize(params);
            break;
        }
        case AuthorizeByUrl: {
            QMultiMap<QString, QByteArray> allParams(requestParams);
            allParams += params;
            QUrl qurl(url);
#if QT_VERSION >= 0x050000
            QUrlQuery query;
#endif
            QList<QString> keys = allParams.keys();
            foreach(const QString &key, keys) {
                QList<QByteArray> vals = allParams.values(key);
                foreach(const QByteArray &val, vals) {
#if QT_VERSION >= 0x050000
                    query.addQueryItem(key, QString::fromUtf8(val));
#else
                    qurl.addEncodedQueryItem(parameterEncoding(key), val.toPercentEncoding());
#endif
                }
            }
#if QT_VERSION >= 0x050000
            qurl.setQuery(query);
#endif
            request.setUrl(qurl);
            request.setHeader(QNetworkRequest::ContentTypeHeader, QByteArrayLiteral("application/x-www-form-urlencoded"));
            ret = q->networkAccessManager()->post(request, QByteArray());
            break;
        }
        }

    } else if (method == QStringLiteral("GET")) {
        switch (authorizeBy) {
        case AuthorizeByHeader: {
            request.setRawHeader("Authorization", authHeader());
            QUrl qurl(url);
#if QT_VERSION >= 0x050000
            QUrlQuery query;
#endif
            QList<QString> keys = params.keys();
            foreach(const QString &key, keys) {
                QList<QByteArray> vals = params.values(key);
                foreach(const QByteArray &val, vals) {
#if QT_VERSION >= 0x050000
                    query.addQueryItem(key, QString::fromUtf8(val));
#else
                    qurl.addEncodedQueryItem(parameterEncoding(key), val.toPercentEncoding());
#endif
                }
            }
#if QT_VERSION >= 0x050000
            qurl.setQuery(query);
#endif
            request.setUrl(qurl);
            DEBUG() << request.url();
            ret = q->networkAccessManager()->get(request);
            break; }
        case AuthorizeByBody:
            qWarning() << "GET doesn't support AuthorizeByBody.";
            break;
        case AuthorizeByUrl: {
            QMultiMap<QString, QByteArray> allParams(requestParams);
            allParams += params;
            QUrl qurl(url);
#if QT_VERSION >= 0x050000
            QUrlQuery query;
#endif
            QList<QString> keys = allParams.keys();
            foreach(const QString &key, keys) {
                QList<QByteArray> vals = allParams.values(key);
                foreach(const QByteArray &val, vals) {
#if QT_VERSION >= 0x050000
                    query.addQueryItem(key, QString::fromUtf8(val));
#else
                    qurl.addEncodedQueryItem(parameterEncoding(key), val.toPercentEncoding());
#endif
                }
            }
#if QT_VERSION >= 0x050000
            qurl.setQuery(query);
#endif
            request.setUrl(qurl);
            ret = q->networkAccessManager()->get(request);
            break;
        }
        }
    }
//    DEBUG() << ret->url();
    return ret;
}

QNetworkReply *OAuthManager::Private::echo(const QString &method, const QUrl &url, const QMultiMap<QString, QByteArray> &params, const QUrl& serviceProvider, const QUrl &realm, bool multiPart)
{
    QNetworkRequest request;
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork);
    request.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);

    QNetworkReply *ret = 0;

    QDateTime dt = QDateTime::currentDateTimeUtc();
    QString timestamp;
    QString nonce;
    timestamp.setNum(dt.toTime_t());
    nonce.setNum(dt.toMSecsSinceEpoch());

    if (requestParams.isEmpty()) {
        requestParams[QStringLiteral("oauth_version")] = "1.0";
        requestParams[QStringLiteral("oauth_signature_method")] = "HMAC-SHA1";
        requestParams[QStringLiteral("oauth_consumer_key")] = consumerKey.toUtf8();
        if (!token.isEmpty())
            requestParams[QStringLiteral("oauth_token")] = token.toUtf8();
    }
    requestParams[QStringLiteral("oauth_nonce")] = nonce.toUtf8();
    requestParams[QStringLiteral("oauth_timestamp")] = timestamp.toUtf8();

    requestParams[QStringLiteral("oauth_signature")] = signature(QStringLiteral("GET"),
                                                  serviceProvider,
                                                  normalize(signatureParams(QMultiMap<QString, QByteArray>())));

    if (method == QStringLiteral("POST")) {
        switch (authorizeBy) {
        case AuthorizeByHeader:
//            DEBUG() << url;
            if (multiPart) {
                request.setRawHeader("X-Auth-Service-Provider", serviceProvider.toString().toUtf8());
                request.setRawHeader("X-Verify-Credentials-Authorization", authHeader(QMultiMap<QString, QByteArray>(), realm));
                request.setUrl(url);
                QString boundary = QStringLiteral("--------------------%1").arg(nonce);
                QByteArray body;
                QList<QString> keys = params.keys();
                foreach(const QString &key, keys) {
                    QList<QByteArray> vals = params.values(key);
                    foreach(const QByteArray &val, vals) {
                        body.append(QStringLiteral("--%1\r\n").arg(boundary).toUtf8());
                        if (key == QStringLiteral("media")) {
                            QUrl url(QString::fromUtf8(val));
                            QFileInfo info(url.path());
                            QFile file(info.filePath());
                            body.append(QStringLiteral("Content-Disposition: file; name=\"%1\"; filename=\"%2\"\r\n").arg(key).arg(info.fileName()).toUtf8());
                            body.append("Content-Type: application/octetstream\r\n");
//                            body.append("Content-Transfer-Encoding: binary\r\n");
                            body.append("\r\n");
                            if (file.open(QFile::ReadOnly)) {
                                body.append(file.readAll());
                                file.close();
                            } else {
                                DEBUG() << file.fileName() << file.errorString();
                            }
                            body.append("\r\n");
                        } else {
                            body.append(QStringLiteral("Content-Disposition: form-data; name=\"%1\"\r\n").arg(key).toUtf8());
                            body.append("\r\n");
                            body.append(val);
                            body.append("\r\n");
                        }
//                        DEBUG() << key << val << body.size();
                    }
                }
                body.append(QStringLiteral("\r\n--%1--\r\n").arg(boundary).toUtf8());
//                DEBUG() << body.data();
                request.setHeader(QNetworkRequest::ContentLengthHeader, QString::number(body.length()).toUtf8());
                request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("multipart/form-data; boundary=%1").arg(boundary).toUtf8());

//                foreach (const QByteArray &rawHeader, request.rawHeaderList()) {
//                    DEBUG() << rawHeader << request.rawHeader(rawHeader);
//                }

                ret = q->networkAccessManager()->post(request, body);
            } else {
                request.setRawHeader("Authorization", authHeader(params));
                request.setUrl(url);
                ret = q->networkAccessManager()->post(request, normalize(QMultiMap<QString, QByteArray>()));
            }

            break;
        default:
            DEBUG() << authorizeBy << "not handled";
            break;
        }
    } else if (method == QStringLiteral("GET")) {

    }
    return ret;
}

QByteArray OAuthManager::Private::parameterEncoding(const QString &str) const
{
    return str.toUtf8().toPercentEncoding();
}

QByteArray OAuthManager::Private::normalize(const QMultiMap<QString, QByteArray> &param) const
{
    QByteArray ret;
    QList<QString> keys = param.keys();
    foreach(const QString &key, keys) {
        QList<QByteArray> vals = param.values(key);
        foreach(const QByteArray &val, vals) {
            ret += parameterEncoding(key) + '=' + val.toPercentEncoding() + '&';
        }
    }
    ret.chop(1);
    return ret;
}

QByteArray OAuthManager::Private::signature(const QString &method, const QUrl &url, const QByteArray &params) const
{
    QByteArray key = parameterEncoding(consumerSecret) + "&" +
                     parameterEncoding(tokenSecret);
    QByteArray base = parameterEncoding(method) + "&" +
                      parameterEncoding(url.toString()) + "&" +
                      params.toPercentEncoding();
    return hmac_sha1(key,base).toBase64();
}

QMultiMap<QString, QByteArray> OAuthManager::Private::signatureParams(const QMultiMap<QString, QByteArray> &params) const
{
    QMultiMap<QString, QByteArray> ret(requestParams);
    ret.remove(QStringLiteral("oauth_signature"));
    ret += params;
    return ret;
}

QByteArray OAuthManager::Private::authHeader() const
{
    QMultiMap<QString, QByteArray> params;
    return authHeader(params);
}

QByteArray OAuthManager::Private::authHeader(const QMultiMap<QString, QByteArray> &params) const
{
    QUrl realm;
    return authHeader(params, realm);
}

QByteArray OAuthManager::Private::authHeader(const QMultiMap<QString, QByteArray> &params, const QUrl &realm) const
{
    QByteArray ret = "OAuth ";
    if (realm.isValid()) {
        ret += "realm=\"" + realm.toString().toUtf8() + "\",";
    }
    QMultiMap<QString, QByteArray> map(requestParams);
    map += params;
    QMapIterator<QString, QByteArray> i(map);
    while (i.hasNext()) {
        i.next();
        ret += parameterEncoding(i.key());
        ret += "=";
        ret += '"';
        ret += i.value().toPercentEncoding();
        ret += '"';
        if (i.hasNext()) {
            ret += ",";
        }
    }
    return ret;
}

bool OAuthManager::Private::updateToken(QNetworkReply* reply)
{
    if (!reply) {
        return false;
    }
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "error : " << reply->errorString();
        return false;
    }
    QRegExp exp(QStringLiteral("(([^=&?]+)=([^=&?]+))(&([^=&?]+)=([^=&?]+))*"));
    QByteArray result = reply->readAll();
    DEBUG() << result;
    if (!exp.exactMatch(QString::fromUtf8(result))) {
        return false;
    }
    QList<QByteArray> params = result.split('&');
    foreach (const QByteArray &param, params) {
        QList<QByteArray> tmp = param.split('=');
        QByteArray key = QByteArray::fromPercentEncoding(tmp.at(0));
        QByteArray val = QByteArray::fromPercentEncoding(tmp.at(1));
        DEBUG() << key << val;
        if (key == "oauth_token_secret") {
            q->setTokenSecret(QString::fromUtf8(val));
        } else if (key == "user_id") {
            q->user_id(QString::fromUtf8(val));
        } else if (key == "screen_name") {
            q->screen_name(QString::fromUtf8(val));
        } else {
            if (key == "oauth_token") {
                q->setToken(QString::fromUtf8(val));
            }
            requestParams[QString::fromUtf8(key)] = val;
        }
    }
    return true;
}

OAuthManager &OAuthManager::instance()
{
    static OAuthManager ret;
    return ret;
}

OAuthManager::OAuthManager(QObject *parent)
    : QObject(parent)
{
    d = new Private(this);
}

QNetworkAccessManager *OAuthManager::networkAccessManager() const
{
    if (!d->networkAccessManager)
        d->networkAccessManager = new QNetworkAccessManager(d);
    return d->networkAccessManager;
}

void OAuthManager::setNetworkAccessManager(QNetworkAccessManager *networkAccessManager)
{
    if (d->networkAccessManager == networkAccessManager) return;
    d->networkAccessManager = networkAccessManager;
    emit networkAccessManagerChanged(networkAccessManager);
}

OAuthManager::AuthorizeBy OAuthManager::authorizeBy() const
{
    return d->authorizeBy;
}

void OAuthManager::setAuthorizeBy(AuthorizeBy authorizeBy)
{
    d->authorizeBy = authorizeBy;
}

const QString &OAuthManager::consumerKey() const
{
    return d->consumerKey;
}

void OAuthManager::setConsumerKey(const QString &consumerKey)
{
    if (d->consumerKey == consumerKey) return;
    d->consumerKey = consumerKey;
    emit consumerKeyChanged(consumerKey);
}

const QString &OAuthManager::consumerSecret() const
{
    return d->consumerSecret;
}

void OAuthManager::setConsumerSecret(const QString &consumerSecret)
{
    if (d->consumerSecret == consumerSecret) return;
    d->consumerSecret = consumerSecret;
    emit consumerSecretChanged(consumerSecret);
}

const QString &OAuthManager::token() const
{
    return d->token;
}

void OAuthManager::setToken(const QString &token)
{
    if (d->token == token) return;
    d->token = token;
    emit tokenChanged(token);
}

const QString &OAuthManager::tokenSecret() const
{
    return d->tokenSecret;
}

void OAuthManager::setTokenSecret(const QString &tokenSecret)
{
    if (d->tokenSecret == tokenSecret) return;
    d->tokenSecret = tokenSecret;
    emit tokenSecretChanged(tokenSecret);
}

const QString &OAuthManager::user_id() const
{
    return d->user_id;
}

void OAuthManager::user_id(const QString &user_id)
{
    if (d->user_id == user_id) return;
    d->user_id = user_id;
    emit user_idChanged(user_id);
}

const QString &OAuthManager::screen_name() const
{
    return d->screen_name;
}

void OAuthManager::screen_name(const QString &screen_name)
{
    if (d->screen_name == screen_name) return;
    d->screen_name = screen_name;
    emit screen_nameChanged(screen_name);
}

bool OAuthManager::isAuthorized() const
{
    return d->isAuthorized;
}

void OAuthManager::setAuthorized(bool isAuthorized)
{
    if (d->isAuthorized == isAuthorized) return;
    d->isAuthorized = isAuthorized;
    emit authorizedChanged(isAuthorized);
}

QNetworkReply *OAuthManager::request(const QString &method, const QUrl &url, const QMultiMap<QString, QByteArray> &params, bool multiPart)
{
    return d->request(method, url, params, multiPart);
}

QNetworkReply *OAuthManager::request(const QString &method, const QUrl &url, const QMultiMap<QString, QByteArray> &params, const QByteArray &acceptEncoding)
{
    return d->request(method, url, params, false, acceptEncoding);
}

QNetworkReply *OAuthManager::echo(const QString &method, const QUrl &url, const QMultiMap<QString, QByteArray> &params, const QUrl& serviceProvider, const QUrl &realm, bool multiPart)
{
    return d->echo(method, url, params, serviceProvider, realm, multiPart);
}

bool OAuthManager::updateToken(QNetworkReply *reply)
{
    return d->updateToken(reply);
}

#include "oauthmanager.moc"
