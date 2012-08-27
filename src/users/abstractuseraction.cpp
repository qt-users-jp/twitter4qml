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

#include "abstractuseraction.h"

class AbstractUserAction::Private
{
public:
    Private();

    QString userId;
    QString screenName;
    bool includeEntities;
};

AbstractUserAction::Private::Private()
    : includeEntities(true)
{
}

AbstractUserAction::AbstractUserAction(QObject *parent)
    : AbstractTwitterAction(parent)
    , d(new Private)
{
}

AbstractUserAction::~AbstractUserAction()
{
    delete d;
}

const QString &AbstractUserAction::userId() const
{
    return d->userId;
}

void AbstractUserAction::setUserId(const QString &userId)
{
    if (d->userId == userId) return;
    d->userId = userId;
    emit userIdChanged(userId);
}

const QString &AbstractUserAction::screenName() const
{
    return d->screenName;
}

void AbstractUserAction::setScreenName(const QString &screenName)
{
    if (d->screenName == screenName) return;
    d->screenName = screenName;
    emit screenNameChanged(screenName);
}

bool AbstractUserAction::includeEntities() const
{
    return d->includeEntities;
}

void AbstractUserAction::setIncludeEntities(bool includeEntities)
{
    if (d->includeEntities == includeEntities) return;
    d->includeEntities = includeEntities;
    emit includeEntitiesChanged(includeEntities);
}
