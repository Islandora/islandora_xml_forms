<?php



/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_elements', 'Tag');

class Tags {
  /**
   * Constants
   */
  const AddButton = 'add-tag';

  /**
   *
   * @param array $element
   * @param type $edit
   * @param array $form_state
   * @param array $complete_form 
   */
  public static function Process(array $element, $edit, array &$form_state, array $complete_form = NULL) {
    $path = drupal_get_path('module', 'xml_form_elements');
    drupal_add_css("$path/stylesheets/tags.css");
    drupal_add_js("$path/js/XMLFormElements.js");
    $element['#id'] = $element['#hash'];
    $element['#tree'] = TRUE;
    $element['#prefix'] = "<div class='clear-block' id='{$element['#hash']}'>";
    $element['#suffix'] = '</div>';
    foreach (element_children($element) as $key) {
      $child = $element[$key];
      $hash = $child['#hash'];
      $element['controls']['edit'][$hash] = self::createEditButton($element, $child);
      $element['controls']['remove'][$hash] = self::createRemoveButton($element, $child);
    }
    $element['controls']['add'] = self::createAddButton($element);
    return $element;
  }

  private static function createAddButton(array &$element) {
    $button['#type'] = 'submit';
    $button['#weight'] = 4;
    $button['#size'] = 30;
    $button['#id'] = $element['#hash'] . '-add';
    $button['#attributes'] = array('style' => 'display:none');
    $button['#value'] = 'Add';
    $button['#attributes'] = array('style' => 'display:none');
    $button['#ahah'] = array(
      'path' => "xml/form/elements/ahah/tag/add/{$element['#hash']}",
      'wrapper' => $element['#hash'], // The parents wrapper is set to the parents hash.
      'method' => 'replace',
      'effect' => 'fade'
    );
    return $button;
  }

  private static function createRemoveButton(array &$element, array &$child) {
    $button['#type'] = 'submit';
    $button['#weight'] = 4;
    $button['#size'] = 30;
    $button['#id'] = $child['#hash'] . '-remove';
    $button['#attributes'] = array('style' => 'display:none');
    $button['#ahah'] = array(
      'path' => "xml/form/elements/ahah/tag/remove/{$child['#hash']}",
      'wrapper' => $element['#hash'],
      'method' => 'replace',
      'effect' => 'fade'
    );
    return $button;
  }

  private static function createEditButton(array &$element, array &$child) {
    $button['#type'] = 'submit';
    $button['#weight'] = 4;
    $button['#size'] = 30;
    $button['#id'] = $child['#hash'] . '-edit';
    $button['#attributes'] = array('style' => 'display:none');
    $button['#ahah'] = array(
      'path' => "xml/form/elements/ahah/tag/edit/{$child['#hash']}",
      'wrapper' => $element['#hash'],
      'method' => 'replace',
      'effect' => 'fade'
    );
    return $button;
  }

}

/**
 * Preprocess tabs theme hook.
 * 
 * @param array $vars 
 */
function xml_form_elements_preprocess_tags_content(&$vars) {
  global $base_url;
  $tags = &$vars[0];
  $vars['image_path'] = "$base_url/{$vars['directory']}/images"; // Location of images.
  $vars['add'] = $tags['controls']['add']; // Visible Add Button
  $vars['edit'] = isset($tags['controls']['edit']) ? $tags['controls']['edit'] : NULL; // Hidden Edit Button
  $vars['remove'] = isset($tags['controls']['remove']) ? $tags['controls']['remove'] : NULL; // Hidden Remove Button
  $vars['tags'] = array(); // Used to create tag icons, and hidden input fields.
  $children = element_children($tags);
  foreach ($children as $key) {
    $child = $tags[$key];
    if (isset($child['#type']) && $child['#type'] == 'tag') {
      $vars['tags'][$key] = $child;
    }
  }
  $vars['input'] = array_shift($vars['tags']); // First tag is the textfield, all others show up as icons with hidden inputs.
}

/**
 * Theme Hook for tags elements.
 */
function xml_form_elements_theme_tags($element) {
  $content = theme('tags_content', $element);
  return theme('form_element', $element, $content);
}