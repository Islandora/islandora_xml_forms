Drupal.behaviors.xmlFormElementTabs = {
  tabs: {
    tabs: null, // Collection of all tabpanels.
    collapsibleTabs: null,
    nonCollapsibleTabs: null,
    loadPanels: function (collapse) {
      var load = '.xml-form-elements-tabs:not(.processed)';
      var collapsible = '.xml-form-elements-tabs-collapsible';
      var collapsed = '.xml-form-elements-tabs-collapsed';
      this.tabs = jQuery(load);
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
      var icon = jQuery('span.expand-tabpanel-icon:first', this);
      if(jQuery(ui.panel).hasClass('ui-tabs-hide')) {
        icon.removeClass('ui-icon-circle-triangle-e');
        icon.addClass('ui-icon-circle-triangle-s');
      }
      else {
        icon.removeClass('ui-icon-circle-triangle-s');
        icon.addClass('ui-icon-circle-triangle-e');
      }
    },
    setCollapsibleIconOnCreate: function(event, ui) {
      var icon = jQuery('span.expand-tabpanel-icon:first', this);
      if(jQuery('div.ui-tabs-panel:not(.ui-tabs-hide)', this).length > 0) {
        icon.removeClass('ui-icon-circle-triangle-e');
        icon.addClass('ui-icon-circle-triangle-s');
      }
      else {
        icon.removeClass('ui-icon-circle-triangle-s');
        icon.addClass('ui-icon-circle-triangle-e');
      }
    },
    attachToolTips: function() {
      jQuery('.tool_tip_trigger').each(function() {
        var tip = jQuery(this).find('.tool_tip');
        jQuery(this).hover(function() {
          var html = '';
          var id = jQuery(this).children('a[href]').attr('href');
          jQuery('#' + id + ' div.form-item').each(function() {
            var item = jQuery(this);
            var text = jQuery('input[type~="text"]', item);
            if(text.length) {
              var id = text.attr('id');
              var label = jQuery('label[for=' + id + ']', item);
              if(label.length) {
                label = label.text();
                text = text.val();
                jQuery('input[class~="form-tag"]', item).each(function() {
                  var tag = jQuery(this);
                  text += ' ' + tag.val();
                });
                text = jQuery.trim(text);
                if(text != "") {
                  html += label + ' ' + text + '<br/>';
                }
              }
            }
          });
          html = jQuery.trim(html);
          if(html == "") {
            html = "Empty";
          }
          tip.html(html);
          tip.appendTo('body');
        },
        function() {
          tip.appendTo(this);
        }).mousemove(function(e) {
          var x = e.pageX + 20,
          y = e.pageY + 20,
          w = tip.width(),
          h = tip.height(),
          dx = jQuery(window).width() - (x + w),
          dy = jQuery(window).height() - (y + h);
          if ( dx < 20 ) x = e.pageX - w - 20;
          if ( dy < 20 ) y = e.pageY - h - 20;
          tip.css({
            left: x,
            top: y
          });
        });
      });
    },
    enableActions: function () {
      var icons = jQuery(".ui-icon-close:not(.processed)");
      icons.click(function() {
    	  jQuery("#" + jQuery(this).text()).trigger("mousedown");
      });
      icons.addClass('processed');
    }
  },
  attach: function (context, settings) {
	  this.tabs.loadPanels(true);
	  this.tabs.enableActions();
  }
}