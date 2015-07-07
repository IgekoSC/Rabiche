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
                        onPressed: tweet.userDetails()
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
                        onPressed: tweet.userDetails()
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

                    Image {
                        anchors.fill: parent
                        source: "qrc:/Resources/img/imageMask48.png"
                    }

                    MouseArea {
                        anchors.fill: parent
                        cursorShape: Qt.PointingHandCursor
                        onPressed: tweet.userDetails()
                    }
                }

                Text {
                    id: lblTweetText
                    color: "#292F33"
                    text: tweetData["text"]
                    font.pixelSize: 14
                    wrapMode: Text.WordWrap
                    width: tweet.width - avatarImg.width - tweetBody.spacing - background.anchors.margins * 2
                }
            }

            Row {
                id: tweetFooter
                height: 16
                spacing: 10

                Row {
                    spacing: 5

                    Image {
                        id: replyIcon
                        height: 16
                        width: 16
                        source: "qrc:///Resources/img/reply.png"

                        MouseArea {
                            anchors.fill: parent
                            cursorShape: Qt.PointingHandCursor
                            onPressed: tweet.reply()
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
                }

                Row {
                    spacing: 5

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
                            onPressed: tweet.retweet()
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
                }

                Row {
                    spacing: 5

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
                            onPressed: tweet.favorite()
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
}
