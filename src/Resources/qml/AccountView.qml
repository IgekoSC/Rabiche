import QtQuick 2.0
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import rabiche 1.0

GridLayout {
    anchors.fill: parent
    anchors.margins: 5
    rowSpacing: 5
    columnSpacing: 5
    columns: 3

    Item {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.columnSpan: parent.columns

        Rectangle {
            color: "white"
            anchors.fill: parent
        }

        ScrollView {
            anchors.fill: parent

            ListView {
                id: tweetsList
                anchors.fill: parent
                clip: true
                model: tweetsListModel
                spacing: 5

                delegate: Tweet {
                    width: parent.width

                    onFavorite: {
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
        }
    }

    //####################################################################################

    ListModel {
        id: tweetsListModel
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

    function showTweetCrtls() {
        var h = root.height;
        tweetControls.visible = true;
        root.height = h;
        btnShowTweetCrtls.text = "-";
    }

    function hideTweetCrtls() {
        var h = root.height;
        tweetControls.visible = false;
        root.height = h;
        btnShowTweetCrtls.text = "+";
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
