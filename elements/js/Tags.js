/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

xml_form_elements.tag = {
    init: function() {
        $("span.add-tag").live("click", function() {
            var form_item = $(this).parent('div.xml-form-elements-tags');
            var value = $('input.tag-editor', form_item).val();
        });
        $(".remove-tag").live("click", function() {
            var form_item = $(this).parent('div.xml-form-elements-tags');
            $("#"+id).trigger("mousedown");
        });
    }
};

/**
     * On Load, listen for ajax requests and attempt to regenerate any new tabs.
     */
$(document).ready(function() {
    
    
    xml_form_elements.tags.attach(true);
    xml_form_elements.tabpanel.enableActions();
    $("body").ajaxComplete(function(event, request, settings) {
        xml_form_elements.tabpanel.loadPanels(false);
        var response = eval("(" + request.responseText + ")");
        jQuery.extend(Drupal.settings, response.settings);
        Drupal.attachBehaviors();
    });
});