Drupal.behaviors.xmlFormElementDatepicker = function(context) {
    $( "input.datepicker" ).datepicker({
	    changeMonth: true,
	    changeYear: true,
	    dateFormat: 'yy-mm-dd'
	});
}