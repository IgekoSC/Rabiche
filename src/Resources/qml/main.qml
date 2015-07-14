import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import rabiche 1.0

ApplicationWindow {
    id: mainWindow
    title: qsTr("Rabiche")
    width: 800
    height: 800
    visible: true

    property bool isPortrait: Screen.primaryOrientation === Qt.PortraitOrientation

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: messageDialog.show(qsTr("Open action triggered"));
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
    }

    ListModel {
        id: tweetsListModel
    }

    Rectangle {
        color: "white"
        anchors.fill: parent
        anchors.margins: 10

        ListView {
            id: tweetsList
            objectName: "tweetsList"
            anchors.fill: parent
            clip: true
            model: tweetsListModel
            spacing: 5

            delegate: Tweet {
                width: parent.width

                onFavorite: {
//                    var data = tweetData;
//                    data["favorite_count"] += 1;
//                    tweetsListModel.set(index, {"tweetData": data});
                    console.warn(qsTr("Unimplemented functionality"));
                }

                onRetweet: {
                    console.warn(qsTr("Unimplemented functionality"));
                }

                onReply: {
                    console.warn(qsTr("Unimplemented functionality"));
                }

                onUserDetails: {
                    console.warn(qsTr("Unimplemented functionality"));
                }
            }
        }

        ScrollBar {
            id: listScrollBar
            orientation: isPortrait ? Qt.Horizontal : Qt.Vertical
            height: isPortrait ? 8 : tweetsList.height;
            width: isPortrait ? tweetsList.width : 8
            scrollArea: tweetsList;
            anchors.right: tweetsList.right
        }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("May I have your attention, please?")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }

    Rabiche {
        id: rabiche
        onNewTweets: {
            insertNewTweets();
        }
        onNewOfflineTweets: {
            insertOfflineTweets();
        }
    }

    property var tweetsIndex: ({});
    property var replies: ({});

    Component.onCompleted: {
        insertNewTweets()
    }

    Component.onDestruction: {
        rabiche.end()
    }

    function insertOfflineTweets() {
        var offlineTweets = rabiche.getNewOfflineTweets();

        var index = 0;
        for (index = (tweetsListModel.count - 1); (index >= 0) && (tweetsListModel.get(index)["tweetData"]["id_str"] < offlineTweets[0]["id_str"]); index--) ;

        index++;

        for (var i = (offlineTweets.length - 1); i >= 0; i--) {
            var data = offlineTweets[i];
            insertTweet(index, data);
        }
    }

    function insertNewTweets() {
        var newTweets = rabiche.getNewTweets();
        for (var i = (newTweets.length - 1); i >= 0; i--) {
            var data = newTweets[i];
            insertTweet(0, data);
        }
    }

    function insertTweet(idx, data) {
        var idStr = data["id_str"];

        if (data["retweeted_status"] !== undefined) {
            data = data["retweeted_status"];
        }

        if (tweetsIndex[idStr]) {
            //Update existing tweet
            var index = tweetsListModel.count - tweetsIndex[idStr];
            tweetsListModel.set(index, {"tweetData": data, replyCount: 0});
        } else {
            //Insert new tweet
            var inReplyToStatusIdStr = data["in_reply_to_status_id_str"];
            if (inReplyToStatusIdStr === null) {
                tweetsListModel.insert(idx, {tweetData: data, replyCount: 0});
                tweetsIndex[idStr] = tweetsListModel.count;
            } else {
                tweetsListModel.insert(idx, {tweetData: data, replyCount: 1});
                tweetsIndex[idStr] = tweetsListModel.count;
                if (!replies[inReplyToStatusIdStr])
                    replies[inReplyToStatusIdStr] = {};
                replies[inReplyToStatusIdStr][idStr] = {tweetData: data, replyCount: 0};
            }
        }
    }

    function deleteTweet(idStr) {
        var index = tweetsIndex[idStr];

        for (var id in tweetsIndex) {
            if (tweetsIndex[id] > index)
                tweetsIndex[id]--;
        }

        tweetsIndex[idStr] = undefined;

        tweetsListModel.remove(tweetsListModel.count - index);
    }

}
