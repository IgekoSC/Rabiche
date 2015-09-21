function resizeContent() {
    var imgContHeight = window.innerHeight * 0.5;
    if (imgContHeight < 253)
        imgContHeight = 253;

    $('.image', '.container').each(function() {
        $(this).css('height', imgContHeight);
        $(this).children('img').css('top', '50%');
        $(this).children('img').css('-webkit-transform', 'translate(0, -50%)');
    });
}

function imgBindings(element) {
    $('.image', element).each(function() {
        $(this).children("img.lazy").lazyload({
                                                  effect : "fadeIn"
                                              });
        $(this).click(function(e) {
            var divHeight = $(this).height();
            var imgHeight = $(this).children('img').height();
            if (divHeight !== imgHeight) {
                $(this).css('height', imgHeight);
                $(this).children('img').css('top', 0);
                $(this).children('img').css('-webkit-transform', 'translate(0, 0)');
            } else {
                var imgContHeight = window.innerHeight * 0.5;
                if (imgContHeight < 253)
                    imgContHeight = 253;

                $(this).css('height', imgContHeight);
                $(this).children('img').css('top', '50%');
                $(this).children('img').css('-webkit-transform', 'translate(0, -50%)');
            }
        });
    });
}

function onNewTweets() {
//    alert("onNewTweets()");
    var htmlTweets = twitter.newHtmlTweets();

    resizeContent();

    for (var key in htmlTweets) {
        if($("#" + key).length) {
//            alert(key + " exists.");
            $("#" + key).replaceWith(htmlTweets[key]);
        } else {
//            alert(key + " is new.");
            $(".row").first().before(htmlTweets[key]);
        }
        imgBindings($("#" + key));
    }
}

function onNewOfflineTweets() {
    var htmlTweets = twitter.newOfflineHtmlTweets();

    var first;
    for (first in htmlTweets) break;

    var id = "none";
    var isFirst = true;
    $(".row").each(function () {
        if ((parseInt(id) >= parseInt(first)) || (isFirst)) {
            id = this.id;
            isFirst = false;
        }
    });

    resizeContent();

    for (var key in htmlTweets) {
        $("#" + id).before(htmlTweets[key]);

        imgBindings($("#" + key));
        id = key;
    }
}

function init() {
    resizeContent();
    imgBindings($(".container"));

    $('#toolbox-head').click(function(e) {
        var bottom = (window.innerHeight - $('#toolbox').position().top - $('#toolbox').height());
        if (bottom < 0)
            $('#toolbox').css('bottom', 0);
        else
            $('#toolbox').css('bottom', $('#toolbox-content').height() * -1);
    });
}

function countChar(val){
    var chars = 140 - val.value.length;
    $('#char-count').text(chars);

    if (chars > 0) {
        $('#char-count').css('color', 'green');
    } else if (chars < -140) {
        $('#char-count').css('color', 'red');
    } else {
        $('#char-count').css('color', 'orange');
    }
};

$(window).resize(function() {
    resizeContent();
});
