import QtQuick 2.0

Item {
    id: tweet
    height: root.height + background.anchors.margins * 2

    signal retweet  ()
    signal favorite ()
    signal reply ()
    signal userDetails ()

    Rectangle {
        id: background
        color: "white"
        anchors.fill: parent
        anchors.margins: 10

        Column {
            id: root
            spacing: 10

            Row {
                id: tweetHead
                spacing: 10

                Text {
                    id: tweetUser
                    color: "#292F33"
                    text: tweetData["user"]["name"]
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    height: 20
                    font.pixelSize: 14

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: tweet.userDetails()
                    }
                }

                Text {
                    id: tweetUserName
                    color: "#A9B5BE"
                    text: "@" + tweetData["user"]["screen_name"]
                    verticalAlignment: Text.AlignVCenter
                    height: 20
                    font.pixelSize: 13

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: tweet.userDetails()
                    }
                }
            }

            Row {
                id: tweetBody
                spacing: 10

                Image {
                    id: avatarImg
                    source: tweetData["user"]["profile_image_url"]
                    width: 48
                    height: 48

                    BorderImage {
                        source: "qrc:/Resources/img/borderRoundedCorners_5.png"
                        anchors.fill: parent
                        border {top: 5; bottom: 5; left: 5; right: 5}
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: tweet.userDetails()
                    }
                }

                Text {
                    id: lblTweetText
                    color: "#292F33"
                    text: tweetData["text"]
                    font.pixelSize: 14
                    wrapMode: Text.Wrap
                    width: tweet.width - avatarImg.width - tweetBody.spacing - background.anchors.margins * 2
                }
            }

            Column {
                id: media
                spacing: 5

                Repeater {
                    model: ListModel {
                        id: mediaListModel
                    }
                    delegate: Image {
                            id: img
                            source: media_url
                            fillMode: Image.PreserveAspectCrop
                            height: Math.min(253, sourceSize.height)

                            BorderImage {
                                id: img_border
                                source: "qrc:/Resources/img/borderRoundedCorners_5.png"
                                anchors.fill: parent
                                border {top: 5; bottom: 5; left: 5; right: 5}
                            }

                            MouseArea {
                                anchors.fill: parent
                                cursorShape: Qt.PointingHandCursor
                                onClicked: {
                                    if (img.sourceSize.height == img.height) {
                                        img.height = Math.min(253, sourceSize.height);
                                        img_border.visible = true;
                                    } else {
                                        img.height = img.sourceSize.height;
                                        img_border.visible = false;
                                    }
                                }
                            }
                        }
                }

                Component.onCompleted: {
                    if (tweetData["entities"]["media"] != undefined) {
                        for (var i = (tweetData["entities"]["media"].length - 1); i >= 0; i--) {
                            mediaListModel.append(tweetData["entities"]["media"][i]);
                        }
                    }
                }
            }

            Row {
                id: tweetFooter
                height: 16
                spacing: 10

                Image {
                    id: replyIcon
                    height: 16
                    width: 16
                    source: "qrc:///Resources/img/reply.png"

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: tweet.reply()
                        hoverEnabled: true
                        onEntered: replyIcon.source = "qrc:///Resources/img/reply_hover.png"
                        onExited: replyIcon.source = "qrc:///Resources/img/reply.png"
                    }
                }

                Text {
                    id: numberReplies
                    color: "#8899A6"
                    height: 16
                    width: 35
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 14
                    font.bold: true
                    text: replyCount
                }

                Image {
                    id: retweetIcon
                    height: 16
                    width: 16
                    source: {
                        if (tweetData["retweeted"] === true) {
                            "qrc:///Resources/img/retweet_on.png"
                        } else {
                            "qrc:///Resources/img/retweet.png"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: tweet.retweet()
                        hoverEnabled: true
                        onEntered: retweetIcon.source = "qrc:///Resources/img/retweet_hover.png"
                        onExited: retweetIcon.source = "qrc:///Resources/img/retweet.png"
                    }
                }

                Text {
                    id: numberRetweets
                    color: "#8899A6"
                    height: 16
                    width: 35
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 14
                    font.bold: true
                    text: tweetData["retweet_count"]
                }

                Image {
                    id: favoriteIcon
                    height: 16
                    width: 16
                    source: {
                        if (tweetData["favorited"] === true) {
                            "qrc:///Resources/img/favorite_on.png"
                        } else {
                            "qrc:///Resources/img/favorite.png"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onClicked: tweet.favorite()
                        hoverEnabled: true
                        onEntered: favoriteIcon.source = "qrc:///Resources/img/favorite_hover.png"
                        onExited: favoriteIcon.source = "qrc:///Resources/img/favorite.png"
                    }
                }

                Text {
                    id: numberFavorites
                    color: "#8899A6"
                    height: 16
                    width: 35
                    verticalAlignment: Text.AlignVCenter
                    font.pixelSize: 14
                    font.bold: true
                    text: tweetData["favorite_count"]
                }
            }
        }
    }
}
