$(document).ready(function() {
  var pages = xml_form_elements.pages = {
    init: function () {
      var load = '.xml-form-elements-pages';
      this.tabs = $(load).tabs({
        fx: {
          slide: 'toggle'
        }
      });
      $('.xml-form-elements-page-back').each(function() {
        $(this).click(function(event){
          var tab = $(this).parent().parent();
          var selected = tab.tabs("option", "selected"); 
          selected = selected - 1;
          tab.tabs('select', selected);
          event.preventDefault();
        });
      });
    }
  }
  pages.init(true);
  $("body").ajaxComplete(function(event, request, settings) {
    pages.init();
  });
});