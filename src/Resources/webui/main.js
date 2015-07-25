function lazyLoad() {
    $("img.lazy").lazyload({
        effect : "fadeIn"
    });
}

function onNewTweets() {
    var htmlTweets = twitter.newHtmlTweets();
    for (var key in htmlTweets) {
        if($("#" + key).length == 0)
            $(".row").first().before(htmlTweets[key]);
        else
            $("#" + key).replaceWith(htmlTweets[key]);
    }

    lazyLoad();
}

function onNewOfflineTweets() {
    var htmlTweets = twitter.newOfflineHtmlTweets();

    var first;
    for (first in htmlTweets) break;

    var id;
    var isFirst = true;
    $(".row").each(function () {
        if ((parseInt(id) >= parseInt(first)) || (isFirst)) {
            id = this.id;
            isFirst = false;
        }
    });

    for (var key in htmlTweets) {
        $("#" + id).before(htmlTweets[key]);
        id = key;
    }

    lazyLoad();
}

function init() {
    try {
        twitter.newTweets.connect(onNewTweets);
        twitter.newOfflineTweets.connect(onNewOfflineTweets);
    } catch(e) {
        alert(e);
    }
}

$(function() {
   lazyLoad();
});
