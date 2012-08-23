QT       += network sql declarative

INCLUDEPATH += $$PWD

SOURCES += $$PWD/twitter4qml.cpp \
    $$PWD/statuses/hometimeline.cpp \
    $$PWD/oauth/oauth.cpp \
    $$PWD/statuses/mentions.cpp \
    $$PWD/statuses/publictimeline.cpp \
    $$PWD/statuses/retweetedbyme.cpp \
    $$PWD/statuses/abstractstatusesmodel.cpp \
    $$PWD/statuses/retweetedtome.cpp \
    $$PWD/statuses/retweetsofme.cpp \
    $$PWD/statuses/usertimeline.cpp \
    $$PWD/statuses/retweetedtouser.cpp \
    $$PWD/statuses/retweetedbyuser.cpp \
    $$PWD/statuses/retweetedby.cpp \
    $$PWD/statuses/retweetedbyids.cpp \
    $$PWD/statuses/abstractidsmodel.cpp \
    $$PWD/statuses/retweets.cpp \
    $$PWD/search/search.cpp \
    $$PWD/tools/jsonparser.cpp \
    $$PWD/directmessages/directmessages.cpp \
    $$PWD/directmessages/abstractdirectmessagesmodel.cpp \
    $$PWD/directmessages/directmessagessent.cpp \
    $$PWD/directmessages/abstractdirectmessageaction.cpp \
    $$PWD/statuses/destroystatus.cpp \
    $$PWD/directmessages/destroydirectmessage.cpp \
    $$PWD/directmessages/newdirectmessage.cpp \
    $$PWD/tools/abstracttwittermodel.cpp \
    $$PWD/tools/oauthmanager.cpp \
    $$PWD/tools/hmac_sha1.cpp \
    $$PWD/statuses/abstractstatusaction.cpp \
    $$PWD/statuses/retweetstatus.cpp \
    $$PWD/statuses/showstatus.cpp \
    $$PWD/statuses/updatestatus.cpp \
    $$PWD/friendships/abstractfriendshipidsmodel.cpp \
    $$PWD/friendships/followers.cpp \
    $$PWD/friendships/friends.cpp \
    $$PWD/friendships/incoming.cpp \
    $$PWD/friendships/outgoing.cpp \
    $$PWD/friendships/updatefriendship.cpp \
    $$PWD/tools/abstracttwitteraction.cpp \
    $$PWD/users/lookupusers.cpp \
    $$PWD/favorites/favorites.cpp \
    $$PWD/favorites/abstractfavoriteaction.cpp \
    $$PWD/favorites/destroyfavorite.cpp \
    $$PWD/favorites/createfavorite.cpp \
    $$PWD/lists/abstractlistsmodel.cpp \
    $$PWD/lists/listsall.cpp \
    $$PWD/lists/lists.cpp \
    $$PWD/lists/listssubscriptions.cpp \
    $$PWD/streaming/userstream.cpp \
    $$PWD/streaming/filterstream.cpp \
    $$PWD/savedsearches/savedsearches.cpp \
    $$PWD/savedsearches/destroysavedsearch.cpp \
    $$PWD/savedsearches/createsavedsearch.cpp \
    $$PWD/statuses/status.cpp \
    $$PWD/tools/tweetmanager.cpp \
    $$PWD/tools/datamanager.cpp \
    $$PWD/users/user.cpp \
    $$PWD/users/showuser.cpp \
    $$PWD/users/abstractuseraction.cpp \
    $$PWD/friendships/createfriendship.cpp \
    $$PWD/friendships/abstractfriendshipaction.cpp \
    $$PWD/friendships/destroyfriendship.cpp \
    $$PWD/directmessages/directmessage.cpp \
    $$PWD/directmessages/showdirectmessage.cpp \
    $$PWD/lists/liststatuses.cpp \
    $$PWD/lists/listsmemberships.cpp \
    $$PWD/users/abstractusersmodel.cpp \
    $$PWD/lists/listmembers.cpp \
    $$PWD/statuses/updatestatuswithmedia.cpp \
    $$PWD/help/configuration.cpp \
    $$PWD/tools/abstracttwittermultipartaction.cpp \
    $$PWD/lists/list.cpp \
    $$PWD/lists/showlist.cpp \
    $$PWD/lists/abstractlistaction.cpp \
    $$PWD/lists/destroylist.cpp \
    $$PWD/lists/createlist.cpp \
    $$PWD/lists/updatelist.cpp \
    $$PWD/lists/subscribelist.cpp \
    $$PWD/lists/unsubscribelist.cpp \
    $$PWD/extra/unionmodel.cpp \
    $$PWD/search/searchstatuses.cpp \
    $$PWD/blocks/abstractblockaction.cpp \
    $$PWD/blocks/createblock.cpp \
    $$PWD/blocks/destroyblock.cpp \
    $$PWD/blocks/blockexists.cpp \
    $$PWD/blocks/blockingids.cpp \
    $$PWD/blocks/reportforspam.cpp \
    $$PWD/help/test.cpp \
    $$PWD/legal/privacy.cpp \
    $$PWD/legal/tos.cpp \
    $$PWD/account/verifycredentials.cpp \
    $$PWD/account/updateprofile.cpp \
    $$PWD/account/totals.cpp \
    $$PWD/users/suggestions.cpp \
    $$PWD/users/slugs.cpp \
    $$PWD/account/settings.cpp \
    $$PWD/trends/trends.cpp \
    $$PWD/users/searchusers.cpp \
    $$PWD/trends/available.cpp \
    $$PWD/trends/daily.cpp \
    $$PWD/account/ratelimitstatus.cpp \
    $$PWD/help/languages.cpp \
    $$PWD/trends/weekly.cpp \
    $$PWD/account/updatesettings.cpp \
    $$PWD/geo/geosearch.cpp \
    $$PWD/geo/reversegeocode.cpp \
    $$PWD/utils.cpp \
    $$PWD/related_results/showrelatedresults.cpp \
    $$PWD/statuses/activitysummary.cpp \
    $$PWD/friendships/showfriendships.cpp \
    $$PWD/friendships/noretweetids.cpp

HEADERS += $$PWD/twitter4qml.h \
    $$PWD/twitter4qml_global.h \
    $$PWD/statuses/hometimeline.h \
    $$PWD/oauth/oauth.h \
    $$PWD/statuses/mentions.h \
    $$PWD/statuses/publictimeline.h \
    $$PWD/statuses/retweetedbyme.h \
    $$PWD/statuses/abstractstatusesmodel.h \
    $$PWD/statuses/retweetedtome.h \
    $$PWD/statuses/retweetsofme.h \
    $$PWD/statuses/usertimeline.h \
    $$PWD/statuses/retweetedtouser.h \
    $$PWD/statuses/retweetedbyuser.h \
    $$PWD/statuses/retweetedby.h \
    $$PWD/statuses/retweetedbyids.h \
    $$PWD/statuses/abstractidsmodel.h \
    $$PWD/statuses/retweets.h \
    $$PWD/search/search.h \
    $$PWD/tools/jsonparser.h \
    $$PWD/directmessages/directmessages.h \
    $$PWD/directmessages/abstractdirectmessagesmodel.h \
    $$PWD/directmessages/directmessagessent.h \
    $$PWD/directmessages/abstractdirectmessageaction.h \
    $$PWD/statuses/destroystatus.h \
    $$PWD/directmessages/destroydirectmessage.h \
    $$PWD/directmessages/newdirectmessage.h \
    $$PWD/tools/abstracttwittermodel.h \
    $$PWD/tools/hmac_sha1.h \
    $$PWD/tools/oauthmanager.h \
    $$PWD/statuses/abstractstatusaction.h \
    $$PWD/statuses/retweetstatus.h \
    $$PWD/statuses/showstatus.h \
    $$PWD/statuses/updatestatus.h \
    $$PWD/friendships/abstractfriendshipidsmodel.h \
    $$PWD/friendships/followers.h \
    $$PWD/friendships/friends.h \
    $$PWD/friendships/incoming.h \
    $$PWD/friendships/outgoing.h \
    $$PWD/friendships/updatefriendship.h \
    $$PWD/tools/abstracttwitteraction.h \
    $$PWD/users/lookupusers.h \
    $$PWD/favorites/favorites.h \
    $$PWD/favorites/abstractfavoriteaction.h \
    $$PWD/favorites/destroyfavorite.h \
    $$PWD/favorites/createfavorite.h \
    $$PWD/lists/abstractlistsmodel.h \
    $$PWD/lists/listsall.h \
    $$PWD/lists/lists.h \
    $$PWD/lists/listssubscriptions.h \
    $$PWD/streaming/userstream.h \
    $$PWD/streaming/filterstream.h \
    $$PWD/savedsearches/savedsearches.h \
    $$PWD/savedsearches/destroysavedsearch.h \
    $$PWD/savedsearches/createsavedsearch.h \
    $$PWD/statuses/status.h \
    $$PWD/tools/tweetmanager.h \
    $$PWD/tools/datamanager.h \
    $$PWD/users/user.h \
    $$PWD/users/showuser.h \
    $$PWD/users/abstractuseraction.h \
    $$PWD/friendships/createfriendship.h \
    $$PWD/friendships/abstractfriendshipaction.h \
    $$PWD/friendships/destroyfriendship.h \
    $$PWD/directmessages/directmessage.h \
    $$PWD/directmessages/showdirectmessage.h \
    $$PWD/lists/liststatuses.h \
    $$PWD/lists/listsmemberships.h \
    $$PWD/users/abstractusersmodel.h \
    $$PWD/lists/listmembers.h \
    $$PWD/statuses/updatestatuswithmedia.h \
    $$PWD/help/configuration.h \
    $$PWD/tools/abstracttwittermultipartaction.h \
    $$PWD/lists/list.h \
    $$PWD/lists/showlist.h \
    $$PWD/lists/abstractlistaction.h \
    $$PWD/lists/destroylist.h \
    $$PWD/lists/createlist.h \
    $$PWD/lists/updatelist.h \
    $$PWD/lists/subscribelist.h \
    $$PWD/lists/unsubscribelist.h \
    $$PWD/extra/unionmodel.h \
    $$PWD/search/searchstatuses.h \
    $$PWD/blocks/abstractblockaction.h \
    $$PWD/blocks/createblock.h \
    $$PWD/blocks/destroyblock.h \
    $$PWD/blocks/blockexists.h \
    $$PWD/blocks/blockingids.h \
    $$PWD/blocks/reportforspam.h \
    $$PWD/help/test.h \
    $$PWD/legal/privacy.h \
    $$PWD/legal/tos.h \
    $$PWD/account/verifycredentials.h \
    $$PWD/account/updateprofile.h \
    $$PWD/account/totals.h \
    $$PWD/users/suggestions.h \
    $$PWD/users/slugs.h \
    $$PWD/account/settings.h \
    $$PWD/trends/trends.h \
    $$PWD/users/searchusers.h \
    $$PWD/trends/available.h \
    $$PWD/trends/daily.h \
    $$PWD/account/ratelimitstatus.h \
    $$PWD/help/languages.h \
    $$PWD/trends/weekly.h \
    $$PWD/account/updatesettings.h \
    $$PWD/geo/geosearch.h \
    $$PWD/geo/reversegeocode.h \
    $$PWD/utils.h \
    $$PWD/related_results/showrelatedresults.h \
    $$PWD/statuses/activitysummary.h \
    $$PWD/friendships/showfriendships.h \
    $$PWD/friendships/noretweetids.h

OTHER_FILES += \
    $$PWD/tools/datamanager.sql \
    $$PWD/tools/createdb/000.sql \
    $$PWD/tools/createdb/001.sql \
    $$PWD/tools/createdb/002.sql \
    $$PWD/tools/createdb/003.sql \
    $$PWD/tools/createdb/004.sql \
    $$PWD/tools/createdb/005.sql \
    $$PWD/tools/createdb/006.sql

