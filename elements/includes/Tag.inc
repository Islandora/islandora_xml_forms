<?php

/**
 * @file
 *
 *  Allows forms to dynamically add new tabs to a tabpanel in a form.
 */
module_load_include('inc', 'xml_form_elements', 'includes/Element');

/**
 * A collection of static functions that allow for theming and processing tabpanels.
 */
class Tag {
  /**
   * Constants
   */
  const RemoveButton = 'remove-tag';
  const EditButton = 'edit-tag';

  /**
   * TabPanel's theme hook.
   *
   * @param array $element
   * @return string
   */
  public static function Theme($element) {
    $children = isset($element['#children']) ? $element['#children'] : '';
    $description = isset($element['#description']) ? "<div class='description'>{$element['#description']}</div>" : '';
    return "<div id='{$element['#hash']}'>{$description}{$children}</div>";
  }

  /**
   * The default #process function for tabpanel's.
   *
   * Adds elements that allow for adding/remove form elements.
   *
   * @param array $element
   * @param mixed $edit
   * @param array $form_state
   * @param array $complete_form
   * @return array
   */
  public static function Process(array $element, array &$form_state, array $complete_form = NULL) {
    $tags = &get_form_element_parent($element, $complete_form);
    $element['#id'] = $element['#hash'];
    $element['#title'] = isset($tags['#title']) ? $tags['#title'] : FALSE;
    return $element;
  }
}

/**
 * Theme hook for a tabpanel
 */
function theme_tag(&$vars) {
  return Tag::Theme($vars['element']);
}
