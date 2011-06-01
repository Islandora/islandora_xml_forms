$(document).ready(function () { 
    var initLists = function() {
	$('.list').each(function() {
	    $(this).tagEditor( {
		separator: ';', 
		confirmRemoval: false,
		completeOnBlur: true,
		continuousOutputBuild: true
	    });
	});
    };
    initLists();
    $("body").ajaxComplete(function(event, request, settings) {
	initLists();
    });
});