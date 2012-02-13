/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
Drupal.settings.xml_form_elements.tabpanel = {
  tabs: null, // Collection of all tabpanels.
  collapsibleTabs: null,
  nonCollapsibleTabs: null,
  tool_tip: null,
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
    this.attachToolTips();
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
  attachToolTips: function() {
    $('.tool_tip_trigger').each(function() {
      var tip = Drupal.settings.xml_form_elements.tabpanel.tool_tip;
      if (tip != null) {
        tip.remove();
      }
      tip = $(document.createElement('span')).addClass('tool_tip');
      $(this).hover(function(e) {
        var html = '';
        var id = $(this).children('a[href]').attr('href');
        $('#' + id + ' div.form-item').each(function() {
          var item = $(this);
          var text = $('input[type~="text"]', item);
          if(text.length) {
            var id = text.attr('id');
            var label = $('label[for=' + id + ']', item);
            if(label.length) {
              label = label.text();
              text = text.val();
              $('input[class~="form-tag"]', item).each(function() {
                var tag = $(this);
                text += ' ' + tag.val();
              });
              text = jQuery.trim(text);
              if(text != "") {
                html += label + ' ' + text + '<br/>';
              }
            }
          }
          var select = $('select', item);
          if(select) {
            var id = select.attr('id');
            var label = $('label[for=' + id + ']', item);
            if(label.length) {
              label = label.text();
              html += label + ' ';
            }
            var selected = $('option:selected', select);
            html += selected.text(); + '<br/>';
          }
        });
        html = jQuery.trim(html);
        if(html == "") {
          html = "Empty";
        }
        
        tip.html(html);
        
        var x = e.pageX + 20,
          y = e.pageY + 20,
          w = tip.width(),
          h = tip.height(),
          dx = $(window).width() - (x + w),
          dy = $(window).height() - (y + h);
        if ( dx < 20 ) x = e.pageX - w - 20;
        if ( dy < 20 ) y = e.pageY - h - 20;
        tip.css({
          'left': x, 
          'top': y
        });
        
        tip.appendTo('body');
        Drupal.settings.xml_form_elements.tabpanel.tool_tip = tip;
      },
      function() {
        tip.remove();
      });      
    });
  },
  enableActions: function () {
    $(".ui-icon-close").live("click", function() {
      var id = $(this).text();
      $("#"+id).trigger("mousedown");
      if (Drupal.settings.xml_form_elements.tabpanel.tool_tip != null) {
        Drupal.settings.xml_form_elements.tabpanel.tool_tip.remove();
        Drupal.settings.xml_form_elements.tabpanel.tool_tip = null;
      }
    });
  },
  addTab: function(id) {
    $('#' + id).trigger("mousedown");
    
    if (Drupal.settings.xml_form_elements.tabpanel.tool_tip != null) {
      Drupal.settings.xml_form_elements.tabpanel.tool_tip.remove();
      Drupal.settings.xml_form_elements.tabpanel.tool_tip = null;
    }
    return false;
  }
}; 

/**
 * On Load, listen for ajax requests and attempt to regenerate any new tabs.
 */
$(document).ready(function() {
  Drupal.settings.xml_form_elements.tabpanel.loadPanels(true);
  Drupal.settings.xml_form_elements.tabpanel.enableActions();
  $("body").ajaxComplete(function(event, request, settings) {
    Drupal.settings.xml_form_elements.tabpanel.loadPanels(false);
  });
});

