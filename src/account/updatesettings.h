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

#ifndef UPDATESETTINGS_H
#define UPDATESETTINGS_H

#include "../tools/abstracttwitteraction.h"

class UpdateSettings : public AbstractTwitterAction
{
    Q_OBJECT
    Q_DISABLE_COPY(UpdateSettings)
    Q_PROPERTY(int trend_location_woeid READ trendLocationWoeid WRITE setTrendLocationWoeid NOTIFY trendLocationWoeidChanged)
    Q_PROPERTY(bool sleep_time_enabled READ sleepTimeEnabled WRITE setSleepTimeEnabled NOTIFY sleepTimeEnabledChanged)
    Q_PROPERTY(int start_sleep_time READ startSleepTime WRITE setStartSleepTime NOTIFY startSleepTimeChanged)
    Q_PROPERTY(int end_sleep_time READ endSleepTime WRITE setEndSleepTime NOTIFY endSleepTimeChanged)
    Q_PROPERTY(QString time_zone READ timeZone WRITE setTimeZone NOTIFY timeZoneChanged)
    Q_PROPERTY(QString lang READ lang WRITE setLang NOTIFY langChanged)
public:
    explicit UpdateSettings(QObject *parent = 0);
    ~UpdateSettings();

    int trendLocationWoeid() const;
    void setTrendLocationWoeid(int trendLocationWoeid);
    bool sleepTimeEnabled() const;
    void setSleepTimeEnabled(bool sleepTimeEnabled);
    int startSleepTime() const;
    void setStartSleepTime(int startSleepTime);
    int endSleepTime() const;
    void setEndSleepTime(int endSleepTime);
    const QString &timeZone() const;
    void setTimeZone(const QString &timeZone);
    const QString &lang() const;
    void setLang(const QString &lang);

signals:
    void trendLocationWoeidChanged(int trendLocationWoeid);
    void sleepTimeEnabledChanged(bool sleepTimeEnabled);
    void startSleepTimeChanged(int startSleepTime);
    void endSleepTimeChanged(int endSleepTime);
    void timeZoneChanged(const QString &timeZone);
    void langChanged(const QString &lang);

protected:
    QUrl api() const { return QUrl("http://api.twitter.com/1/account/settings.json"); }

private:
    class Private;
    Private *d;
};

#endif // UPDATESETTINGS_H
