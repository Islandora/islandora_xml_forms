$(document).ready(function() {
  var pages = xml_form_elements.pages = {
    init: function () {
      var load = '.xml-form-elements-pages';
      this.tabs = $(load).tabs({
//        fx: {
//          opacity: 'toggle'
//        }
      });
    }
  }
  pages.init(true);
  $("body").ajaxComplete(function(event, request, settings) {
    pages.init();
  });
});