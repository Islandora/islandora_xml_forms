/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
$(document).ready(function() {
    $('form').parent().append('\n\
    <form id="download-form" method="GET" action="" style="display:none">\n\
        <input id="download-button" name="op" type="submit" value="Export">\n\
    </form>');
    $('#edit-export').click(function() {
        var form_name = $('#edit-form-name').val();
        var url = window.location.pathname + '/' + form_name + '/export';
        $('#download-form').attr('action', url);
        $('#download-button').click();
    });
});