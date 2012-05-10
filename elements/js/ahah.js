/**
 * This overwrise the successful ahah behavoir such that the Drupal.settings object gets updated appropriately with the
 * AJAX response data.
 * 
 * @author nbanks
 */
$(document).ready(function() {
  Drupal.ahah.prototype.success = function (response, status) {
    var wrapper = $(this.wrapper);
    var form = $(this.element).parents('form');
    form.find('div.messages').remove(); // Clear old warnings.
    // Manually insert HTML into the jQuery object, using $() directly crashes
    // Safari with long string lengths. http://dev.jquery.com/ticket/1152
    var new_content = $('<div></div>').html(response.data);
    // Restore the previous action and target to the form.
    form.attr('action', this.form_action);
    this.form_target ? form.attr('target', this.form_target) : form.removeAttr('target');
    this.form_encattr ? form.attr('target', this.form_encattr) : form.removeAttr('encattr');

    // Remove the progress element.
    if (this.progress.element) {
      $(this.progress.element).remove();
    }
    if (this.progress.object) {
      this.progress.object.stopMonitoring();
    }
    $(this.element).removeClass('progress-disabled').attr('disabled', false);

    // Add the new content to the page.
    Drupal.freezeHeight();
    if (this.method == 'replace') {
      wrapper.empty().append(new_content);
    }
    else {
      wrapper[this.method](new_content);
    }

    // Immediately hide the new content if we're using any effects.
    if (this.showEffect != 'show') {
      new_content.hide();
    }

    // Determine what effect use and what content will receive the effect, then
    // show the new content.
    if ($('.ahah-new-content', new_content).size() > 0) {
      $('.ahah-new-content', new_content).hide();
      new_content.show();
      $(".ahah-new-content", new_content)[this.showEffect](this.showSpeed);
    }
    else if (this.showEffect != 'show') {
      new_content[this.showEffect](this.showSpeed);
    }

    // Attach all javascript behaviors to the new content, if it was successfully
    // added to the page, this if statement allows #ahah[wrapper] to be optional.
    if (new_content.parents('html').length > 0) {
      if(response.settings) { // @change 
        jQuery.extend(Drupal.settings, response.settings);
      }    
      Drupal.attachBehaviors(new_content);
    }

    Drupal.unfreezeHeight();
  };
});