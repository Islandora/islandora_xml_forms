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
      $('.xml-form-elements-page-back').each(function() {
        $(this).click(function(event){
          var tab = $(this).parents('div.xml-form-elements-pages')[0];
          pages.back(tab);
          event.preventDefault();
        });
      });
      $('.xml-form-elements-page-next').each(function() {
        $(this).ajaxSuccess(function(event, response, options){
          if(options.extraData[this.name] && Drupal.settings.xmlFormElements.pages[this.name].valid) {
            var tab = $(this).parents('div.xml-form-elements-pages')[0];
            pages.next(tab);
            event.preventDefault(); 
          }
        });
      });
    },
    back: function(tab) {
      tab = $(tab);
      var selected = tab.tabs("option", "selected"); 
      selected = selected - 1;
      tab.tabs('select', selected);
    },
    next: function(tab) {
      tab = $(tab);
      var selected = tab.tabs("option", "selected"); 
      selected = selected + 1;
      tab.tabs('select', selected);
    }
  }
  pages.init();
}