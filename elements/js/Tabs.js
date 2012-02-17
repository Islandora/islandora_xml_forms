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
  attachToolTips: function() { //TODO:  Fix this; nested stuff needs to work better...
    $('.tool_tip_trigger').each(function(i, eel) {
      $(this).hover(function(e) {
        var html = '';// + i + '<br/>';
        var id = $(this).children('a[href]').attr('href');
        $('#' + id + ' div.form-item').each(function() {
          var item = $(this);
          $('> input[type~="text"]', item).each(function(i, text) {
            var id = $(text).attr('id');
            var label = $('label[for="' + id + '"]', item);
            if(label.length > 0) {
              label = label.text();
              var textOut = $(text).val();
              $('input[class~="form-tag"]', $(text).parent()).each(function() {
                var tag = $(this);
                textOut += ' ' + tag.val();
              });
              textOut = jQuery.trim(textOut);
              if(textOut.length > 0) {
                html += label + ' ' + textOut + '<br/>';
              }
            }
          });
          
          $('> select', item).each(function(index, select) {
            var id = $(select).attr('id');
            var label = $('label[for=' + id + ']');
            if(label.length > 0) {
              label = label.text().trim();
              html += label + ' ';
            }
            $('option:selected', select).each(function(idx, selected) {
              html += $(selected).text().trim() + '<br/>';
            });
          });
        });
        html = jQuery.trim(html);
        if(html == "") {
          html = "Empty";
        }
        
        if (Drupal.settings.xml_form_elements.tabpanel.tool_tip != null) {
          Drupal.settings.xml_form_elements.tabpanel.tool_tip.remove();
        }
        else {
          Drupal.settings.xml_form_elements.tabpanel.tool_tip = $(document.createElement('span')).addClass('tool_tip');
        }
        
        Drupal.settings.xml_form_elements.tabpanel.tool_tip.html(html);
        
        var x = e.pageX + 20,
          y = e.pageY + 20,
          w = Drupal.settings.xml_form_elements.tabpanel.tool_tip.width(),
          h = Drupal.settings.xml_form_elements.tabpanel.tool_tip.height(),
          dx = $(window).width() - (x + w),
          dy = $(window).height() - (y + h);
        if ( dx < 20 ) x = e.pageX - w - 20;
        if ( dy < 20 ) y = e.pageY - h - 20;
        Drupal.settings.xml_form_elements.tabpanel.tool_tip.css({
          'left': x, 
          'top': y
        });
        
        Drupal.settings.xml_form_elements.tabpanel.tool_tip.appendTo('body');
      },
      function() {
        if(Drupal.settings.xml_form_elements.tabpanel.tool_tip != null) {
          Drupal.settings.xml_form_elements.tabpanel.tool_tip.remove();
        }
      });      
    });
  },
  enableActions: function () {
    $(".ui-icon-close").live("click", function() {
      var id = $(this).text();
      $("#"+id).trigger("mousedown");
      if (Drupal.settings.xml_form_elements.tabpanel.tool_tip != null) {
        Drupal.settings.xml_form_elements.tabpanel.tool_tip.remove();
      }
    });
  },
  addTab: function(id) {
    $('#' + id).trigger("mousedown");
    
    if (Drupal.settings.xml_form_elements.tabpanel.tool_tip != null) {
      Drupal.settings.xml_form_elements.tabpanel.tool_tip.remove();
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
    if (Drupal.settings.xml_form_elements.tabpanel.tool_tip != null) {
      Drupal.settings.xml_form_elements.tabpanel.tool_tip.remove();
      Drupal.settings.xml_form_elements.tabpanel.tool_tip = null;
    }
    Drupal.settings.xml_form_elements.tabpanel.loadPanels(false);
  });
});

