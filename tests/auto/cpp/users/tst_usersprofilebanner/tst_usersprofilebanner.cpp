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

#include "abstracttwitter4qmltest.h"

#include <usersprofilebanner.h>

class UsersProfileBannerTest : public AbstractTwitter4QMLTest
{
    Q_OBJECT

private Q_SLOTS:
    void profile_banner_by_user_id();
    void profile_banner_by_user_id_data();
    void profile_banner_by_screen_name();
    void profile_banner_by_screen_name_data();

private:
    void data();
};

void UsersProfileBannerTest::data()
{
    QTest::addColumn<QString>("user_id");
    QTest::addColumn<QString>("screen_name");
    QTest::addColumn<bool>("profile_banner_is_Empty");

    QTest::newRow("Tasuku Suzuki") << "119039167" << "task_jp" << false;
    QTest::newRow("Kenya 888") << "15394706" << "kenya888" << true;
}

void UsersProfileBannerTest::profile_banner_by_user_id()
{
    QFETCH(QString, user_id);
    QFETCH(bool, profile_banner_is_Empty);

    UsersProfileBanner usersProfileBanner;
    usersProfileBanner.user_id(user_id);
    QCOMPARE(usersProfileBanner.user_id(), user_id);
    QVERIFY2(wait(&usersProfileBanner), "UsersProfileBanner::load");
    QVERIFY(usersProfileBanner.web().keys().isEmpty() == profile_banner_is_Empty);
    QVERIFY(usersProfileBanner.web().value(QStringLiteral("url")).isNull() == profile_banner_is_Empty);
}

void UsersProfileBannerTest::profile_banner_by_user_id_data()
{
    data();
}

void UsersProfileBannerTest::profile_banner_by_screen_name()
{
    QFETCH(QString, screen_name);
    QFETCH(bool, profile_banner_is_Empty);

    UsersProfileBanner usersProfileBanner;
    usersProfileBanner.screen_name(screen_name);
    QCOMPARE(usersProfileBanner.screen_name(), screen_name);
    QVERIFY2(wait(&usersProfileBanner), "UsersProfileBanner::load");
    QVERIFY(usersProfileBanner.web().keys().isEmpty() == profile_banner_is_Empty);
    QVERIFY(usersProfileBanner.web().value(QStringLiteral("url")).isNull() == profile_banner_is_Empty);
}

void UsersProfileBannerTest::profile_banner_by_screen_name_data()
{
    data();
}

QTEST_MAIN(UsersProfileBannerTest)

#include "tst_usersprofilebanner.moc"
