/* Copyright (c) 2012 Twitter4QML Project.
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

#ifndef SEARCH_H
#define SEARCH_H

#include "../tools/abstracttwittermodel.h"

class Search : public AbstractTwitterModel
{
    Q_OBJECT
    Q_PROPERTY(QString q READ q WRITE setQ NOTIFY qChanged)
    Q_PROPERTY(QString geocode READ geocode WRITE setGeocode NOTIFY geocodeChanged)
    Q_PROPERTY(QString lang READ lang WRITE setLang NOTIFY langChanged)
    Q_PROPERTY(QString locale READ locale WRITE setLocale NOTIFY localeChanged)
    Q_PROPERTY(int page READ page WRITE setPage NOTIFY pageChanged)
    Q_PROPERTY(QString result_type READ resultType WRITE setResultType NOTIFY resultTypeChanged)
    Q_PROPERTY(int rpp READ rpp WRITE setRpp NOTIFY rppChanged)
    Q_PROPERTY(bool show_user READ showUser WRITE setShowUser NOTIFY showUserChanged)
    Q_PROPERTY(QString until READ until WRITE setUntil NOTIFY untilChanged)
    Q_PROPERTY(QString since_id READ sinceId WRITE setSinceId NOTIFY sinceIdChanged)
    Q_PROPERTY(bool include_entities READ includeEntities WRITE setIncludeEntities NOTIFY includeEntitiesChanged)

    Q_PROPERTY(float completed_in READ completedIn NOTIFY completedInChanged DESIGNABLE false)
    Q_PROPERTY(QString max_id READ maxIdStr NOTIFY maxIdStrChanged DESIGNABLE false)
    Q_PROPERTY(QString next_page READ nextPage NOTIFY nextPageChanged DESIGNABLE false)
    Q_PROPERTY(QString refresh_url READ refreshUrl NOTIFY refreshUrlChanged DESIGNABLE false)

public:
    enum Roles {
        CreatedAtRole = Qt::UserRole + 1
        , FromUserRole
        , FromUserIdRole
        , FromUserNameRole
        , IdRole
        , IdStrRole
        , InReplyToStatusIdRole
        , IsoLanguageCodeRole
        , MetadataRole
        , ProfileImageUrlRole
        , ProfileImageUrlHttpsRole
        , SourceRole
        , TextRole
        , PlainTextRole
        , ToUserRole
        , ToUserIdRole
        , ToUserNameRole
    };
    explicit Search(QObject *parent = 0);
    ~Search();

    static QVariantMap parse(const QVariantMap &status);
    static bool indicesGreaterThan(const QVariant &v1, const QVariant &v2);

    const QString &q() const;
    const QString &geocode() const;
    const QString &lang() const;
    const QString &locale() const;
    int page() const;
    const QString &resultType() const;
    int rpp() const;
    bool showUser() const;
    const QString &until() const;
    const QString &sinceId() const;
    bool includeEntities() const;

    float completedIn() const;
    const QString &maxIdStr() const;
    const QString &nextPage() const;
    const QString &refreshUrl() const;

    DataManager::DataType dataType() const { return DataManager::SearchData; }

public slots:
    virtual void reload();

    void setQ(const QString &q);
    void setGeocode(const QString &geocode);
    void setLang(const QString &lang);
    void setLocale(const QString &locale);
    void setPage(int page);
    void setResultType(const QString &resultType);
    void setRpp(int rpp);
    void setShowUser(bool showUser);
    void setUntil(const QString &until);
    void setSinceId(const QString &sinceId);
    void setIncludeEntities(bool includeEntities);

    void setCompletedIn(float completedIn);
    void setMaxIdStr(const QString &maxIdStr);
    void setNextPage(const QString &nextPage);
    void setRefreshUrl(const QString &refreshUrl);

signals:
    void qChanged(const QString &q);
    void geocodeChanged(const QString &geocode);
    void langChanged(const QString &lang);
    void localeChanged(const QString &locale);
    void pageChanged(int page);
    void resultTypeChanged(const QString &resultType);
    void rppChanged(int rpp);
    void showUserChanged(bool showUser);
    void untilChanged(const QString &until);
    void sinceIdChanged(const QString &sinceId);
    void includeEntitiesChanged(bool includeEntities);

    void completedInChanged(float completedIn);
    void maxIdStrChanged(const QString &maxIdStr);
    void nextPageChanged(const QString &nextPage);
    void refreshUrlChanged(const QString &refreshUrl);

protected:
    QUrl api() const { return QUrl("http://search.twitter.com/search.json"); }
    AuthorizeBy authenticationMethod() const { return AuthorizeByNothing; }
    QString httpMethod() const { return "GET"; }
    void parseDone(const QVariant &result);
    void dataAdded(const QString &key, const QVariantMap &value);

private:
    Q_DISABLE_COPY(Search)
    class Private;
    Private *d;
};

#endif // SEARCH_H
