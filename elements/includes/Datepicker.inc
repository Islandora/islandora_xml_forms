<?php

/**
 * @file
 *
 */
class Datepicker {

  /**
   *
   * @param array $element
   * @param type $edit
   * @param array $form_state
   * @param array $complete_form
   */
  public static function Process(array $element, $edit, array &$form_state, array $complete_form = NULL) {
    static $add_files = TRUE;
    if ($add_files) {
      jquery_ui_add('ui.datepicker');
      drupal_add_js(PATH_XML_FORM_ELEMENTS_JS . 'XMLFormElements.js');
      drupal_add_js(PATH_XML_FORM_ELEMENTS_JS . 'Datepicker.js');
      drupal_add_css(PATH_XML_FORM_ELEMENTS_JQUERY_THEME . 'ui.core.css');
      drupal_add_css(PATH_XML_FORM_ELEMENTS_JQUERY_THEME . 'ui.datepicker.css');
      drupal_add_css(PATH_XML_FORM_ELEMENTS_JQUERY_THEME . 'ui.theme.css');
      $add_files = FALSE;
    }
    return $element;
  }

}

/**
 * Implements theme_hook().
 *
 * For the datepicker form element.
 *
 * @param array $vars
 */
function theme_datepicker($element) {
  $size = empty($element['#size']) ? '' : ' size="' . $element['#size'] . '"';
  $maxlength = empty($element['#maxlength']) ? '' : ' maxlength="' . $element['#maxlength'] . '"';
  $class = array('form-text', 'datepicker');
  $extra = '';
  $output = '';
  _form_set_class($element, $class);
  if (isset($element['#field_prefix'])) {
    $output .= '<span class="field-prefix">' . $element['#field_prefix'] . '</span> ';
  }
  $output .= '<input type="text"' . $maxlength . ' name="' . $element['#name'] . '" id="' . $element['#id'] . '"' . $size . ' value="' . check_plain($element['#value']) . '"' . drupal_attributes($element['#attributes']) . ' />';
  if (isset($element['#field_suffix'])) {
    $output .= ' <span class="field-suffix">' . $element['#field_suffix'] . '</span>';
  }
  return theme('form_element', $element, $output) . $extra;
}
