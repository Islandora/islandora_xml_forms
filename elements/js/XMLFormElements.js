/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
xml_form_elements = {}
/* This is required for AHAH to correctly modify the behavior of new elements. */
$(document).ready(function() {
  $("body").ajaxComplete(function(event, request, settings) {
    var response = eval("(" + request.responseText + ")");
    jQuery.extend(Drupal.settings, response.settings);
    Drupal.attachBehaviors();
  });
});