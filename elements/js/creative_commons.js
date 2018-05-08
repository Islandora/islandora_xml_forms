/**
 * @file
 * Prevent page submit while Creative Commons AJAX is still processing.
 */

(function($) {
  Drupal.behaviors.xmlFormElementCreativeCommons = {
    attach: function (context, settings) {
      $('[id|=license-fieldset]', context).parents('form').once(function(){
        // Get form id.
        var $id = this.id;
        // When AJAX starts, disable the submit buttons.
        $(document).ajaxStart(function(){
          // Disable all submit elements.
          $('#' + $id + ' .form-submit').attr('disabled', 'disabled');
        });
        // When AJAX completes, re-enable the submit buttons.
        $(document).ajaxComplete(function(){
          // Re-enable all submit elements.
          $('#' + $id + ' .form-submit').removeAttr('disabled');
        });
      });
    }
  }
})(jQuery);
