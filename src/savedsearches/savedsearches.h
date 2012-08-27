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

#ifndef SAVEDSEARCHES_H
#define SAVEDSEARCHES_H

#include "../tools/abstracttwittermodel.h"

class SavedSearches : public AbstractTwitterModel
{
    Q_OBJECT
public:
    enum Roles {
        CreatedAtRole = Qt::UserRole + 1
        , IdRole
        , IdStrRole
        , NameRole
        , PositionRole
        , QueryRole
    };
    explicit SavedSearches(QObject *parent = 0);

    Q_INVOKABLE void createSavedSearch(QVariantMap parameters);
    Q_INVOKABLE void destroySavedSearch(QVariantMap parameters);

    bool isLoading() const;
    DataManager::DataType dataType() const { return DataManager::SavedSearchData; }

protected:
    QUrl api() const { return QUrl("http://api.twitter.com/1/saved_searches.json"); }
    AuthorizeBy authenticationMethod() const { return AuthorizeByUrl; }
    QString httpMethod() const { return "GET"; }
    void parseDone(const QVariant &result);

    void dataAdded(const QString &key, const QVariantMap &value);
    void dataAboutToBeRemoved(const QString &key, const QVariantMap &value);

private:
    Q_DISABLE_COPY(SavedSearches)
    class Private;
    Private *d;
};

#endif // SAVEDSEARCHES_H
