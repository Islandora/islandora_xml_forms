<?php

/**
 * @file
 *
 * Defines the various callbacks and resources needed for the 'datepicker' form element.
 */
class Datepicker {

  /**
   * Loads the required resources for displaying the Datepicker element.
   *
   * @staticvar boolean $load
   *   Keeps us from loading the same files multiple times, while not required it just saves some time.
   */
  public static function addRequiredResources() {
    static $load = TRUE;
    if ($load) {
      module_load_include('inc', 'xml_form_elements', 'includes/Element');
      Element::addRequiredResources();
      Element::addUIWidgets('ui.datepicker');
      Element::addUIThemeStyles(array('ui.core.css', 'ui.datepicker.css', 'ui.theme.css'));
      Element::addJS('datepicker.js');
      $load = FALSE;
    }
  }

  /**
   * The '#process' callback for the datepicker form element.
   *
   * @param array $element
   *   The datepicker form element.
   * @param boolean $edit
   *   Not used.
   * @param array $form_state
   *   The Drupal form state.
   * @param array $complete_form
   *   The complete Drupal form definition.
   */
  public static function process(array $element, $edit, array &$form_state, array $complete_form = NULL) {
    self::addRequiredResources();
    return $element;
  }

}

/**
 * Implements theme_hook().
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
