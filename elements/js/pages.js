Drupal.behaviors.xmlFormElementPages = function(context) {
  var pages = {
    tabs: null,
    init: function () {
      var load = '.xml-form-elements-pages';
      this.tabs = $(load).tabs({
        fx: {
          slide: 'toggle'
        }
      });
      this.disable(this.tabs);
      $('.xml-form-elements-page-back').each(function() {
        $(this).unbind('click'); // Prevent multiple binds... @todo implement a better solution.
        $(this).click(function(event){
          var tabs = $(this).parents('div.xml-form-elements-pages')[0];
          pages.back($(tabs));
          event.preventDefault();
        });
      });
      $('.xml-form-elements-page-next').each(function() {
        $(this).ajaxSuccess(function(event, response, options){
          if(options.extraData[this.name] && !pages.isDisabled(this.name)) {
            var tabs = $(this).parents('div.xml-form-elements-pages')[0];
            pages.next($(tabs));
            event.preventDefault(); 
          }
        });
      });
      
    },
    disable: function(tabs) {
      var disabled = [];
      var pages = Drupal.settings.xmlFormElements.pages;
      var i = 0;
      for(var name in pages) {
        i++; // Validation prevents access to the following one.        
        if(this.isDisabled(name)) {
          disabled.push(i);
        }
      }
      tabs.tabs("option", "disabled", disabled);
    },
    isDisabled: function(name) {
      var pages = Drupal.settings.xmlFormElements.pages;
      if(Array.isArray(pages[name].disabled)) {
        var last = pages[name].disabled.length - 1;
        return pages[name].disabled[last];
      }
      return pages[name].disabled;
    },
    back: function(tabs) {
      var selected = tabs.tabs("option", "selected"); 
      selected = selected - 1;
      tabs.tabs('select', selected);
    },
    next: function(tabs) {
      var selected = tabs.tabs("option", "selected"); 
      selected = selected + 1;
      tabs.tabs('select', selected);
    }
  }
  pages.init();
}