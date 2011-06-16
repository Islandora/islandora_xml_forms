/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
xml_form_elements.tabpanel = {
    tabs: null, // Collection of all tabpanels.
    collapsibleTabs: null,
    nonCollapsibleTabs: null,
    loadPanels: function (collapse) {
        var load = '.xml-form-elements-tabs';
        var collapsible = '.xml-form-elements-tabs-collapsible';
        var collapsed = '.xml-form-elements-tabs-collapsed';
        this.tabs = $(load);
        this.collapsibleTabs = this.tabs.filter(collapsible);
        this.nonCollapsibleTabs = this.tabs.not(collapsible);
        var expandedTabs = this.collapsibleTabs.not(collapsed);
        var collapsedTabs = this.collapsibleTabs.filter(collapsed);
        if(collapsedTabs.length > 0) {
            collapsedTabs.tabs({
                collapsible: true,
                selected: collapse ? -1 : undefined,
                select: this.setCollapsibleIconOnSelect,
                create: this.setCollapsibleIconOnCreate
            });
        }
        if(expandedTabs.length > 0) {
            expandedTabs.tabs({
                collapsible: true,
                select: this.setCollapsibleIconOnSelect,
                create: this.setCollapsibleIconOnCreate
            });
        }
        if(this.nonCollapsibleTabs.length > 0) {
            this.nonCollapsibleTabs.tabs({});
        }
    },
    setCollapsibleIconOnSelect: function(event, ui) {
        var icon = $('span.expand-tabpanel-icon:first', this);
        if($(ui.panel).hasClass('ui-tabs-hide')) {
            icon.removeClass('ui-icon-circle-triangle-e');
            icon.addClass('ui-icon-circle-triangle-s');
        }
        else {
            icon.removeClass('ui-icon-circle-triangle-s');
            icon.addClass('ui-icon-circle-triangle-e');
        }
    },
    setCollapsibleIconOnCreate: function(event, ui) {
        var icon = $('span.expand-tabpanel-icon:first', this);
        if($('div.ui-tabs-panel:not(.ui-tabs-hide)', this).length > 0) {
            icon.removeClass('ui-icon-circle-triangle-e');
            icon.addClass('ui-icon-circle-triangle-s');
        }
        else {
            icon.removeClass('ui-icon-circle-triangle-s');
            icon.addClass('ui-icon-circle-triangle-e');
        }
    },
    enableActions: function () {
        $(".ui-icon-close").live("click", function() {
            var id = $(this).text();
            $("#"+id).trigger("mousedown");
        });
    },
    addTab: function(id) {
        $('#' + id).trigger("mousedown");
        return false;
    }
}; 

/**
 * On Load, listen for ajax requests and attempt to regenerate any new tabs.
 */
$(document).ready(function() {
    xml_form_elements.tabpanel.loadPanels(true);
    xml_form_elements.tabpanel.enableActions();
    $("body").ajaxComplete(function(event, request, settings) {
        xml_form_elements.tabpanel.loadPanels(false);
        var response = eval("(" + request.responseText + ")");
        jQuery.extend(Drupal.settings, response.settings);
        Drupal.attachBehaviors();
    });
});

