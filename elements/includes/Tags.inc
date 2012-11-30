<?php

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_elements', 'includes/Tag');


class Tags {
  /**
   * Constants
   */
  const AddButton = 'add-tag';

  /**
   * Loads the required resources for displaying the Tabs element.
   *
   * @staticvar boolean $load
   *   Keeps us from loading the same files multiple times, while not required it just saves some time.
   */
  public static function addRequiredResources(&$form_state) {
    static $load = TRUE;
    if ($load) {
      form_load_include($form_state, 'inc', 'xml_form_elements', 'includes/Element');
      form_load_include($form_state, 'inc', 'xml_form_elements', 'includes/Tag');
      Element::addRequiredResources();
      Element::addJS('tags.js');
      Element::addCSS('tags.css');
      $load = FALSE;
    }
  }

  /**
   *
   * @param array $element
   * @param type $edit
   * @param array $form_state
   * @param array $complete_form
   */
  public static function Process(array $element, array &$form_state, array $complete_form = NULL) {
    self::addRequiredResources($form_state);
    $element['#id'] = $element['#hash'];
    $element['#size'] = isset($element['#size']) ? $element['#size'] : 60;
    $element['#tree'] = TRUE;
    $element['#after_build'] = array('xml_form_elements_after_build_tags');
    $element['#prefix'] = "<div class='clear-block' id='ahah-{$element['#id']}-wrapper'>";
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

  public static function AfterBuild(array $form_element, array &$form_state) {
    if ($form_element['#required']) {
      $require = (count(element_children($form_element)) == 2) ? TRUE : FALSE; // 2 to include the controls element
      foreach (element_children($form_element) as $key) {
        $child = &$form_element[$key];
        $child['#required'] = $require;
      }
    }
    return $form_element;
  }

  private static function createAddButton(array &$element) {
    $button['#type'] = 'button';
    $button['#weight'] = 4;
    $button['#size'] = 30;
    $button['#id'] = $button['#name'] = $element['#hash'] . '-add';
    $button['#attributes'] = array('style' => 'display:none');
    $button['#value'] = t('Add');
    $button['#limit_validation_errors'][] = $element['#parents'];
    $button['#ajax'] = array(
      'callback' => 'xml_form_elements_ajax_callback',
      'params' => array(
        'render' => $element['#hash'],
        'target' => $element['#hash'],
	'action' => 'add'
      ),
      'wrapper' => "ahah-{$element['#id']}-wrapper",
      'method' => 'replaceWith',
      'effect' => 'fade',
    );
    return $button;
  }

  private static function createRemoveButton(array &$element, array &$child) {
    $button['#type'] = 'button';
    $button['#weight'] = 4;
    $button['#size'] = 30;
    $button['#id'] = $button['#name'] = $child['#hash'] . '-remove';
    $button['#attributes'] = array('style' => 'display:none');
    $button['#limit_validation_errors'][] = $element['#parents'];
    $button['#ajax'] = array(
      'callback' => 'xml_form_elements_ajax_callback',
      'params' => array(
        'render' => $element['#hash'],
        'target' => $element['#hash'],
	'child' => $child['#hash'],
	'action' => 'delete'
      ),
      'wrapper' => "ahah-{$element['#id']}-wrapper",
      'method' => 'replaceWith',
      'effect' => 'fade',
    );
    return $button;
  }

  private static function createEditButton(array &$element, array &$child) {
    $button['#type'] = 'button';
    $button['#weight'] = 4;
    $button['#size'] = 30;
    $button['#id'] = $button['#name'] = $child['#hash'] . '-edit';
    $button['#attributes'] = array('style' => 'display:none');
    $button['#limit_validation_errors'][] = $element['#parents'];
    $button['#ajax'] = array(
      'callback' => 'xml_form_elements_ajax_callback',
      'params' => array(
        'render' => $element['#hash'],
        'target' => $element['#hash'],
        'child' => $child['#hash'],
	'action' => 'edit'
      ),
      'wrapper' => "ahah-{$element['#id']}-wrapper",
      'method' => 'replaceWith',
      'effect' => 'fade',
    );
    return $button;
  }

}

/**
 * Preprocess tabs theme hook.
 *
 * @param array $vars
 */
function template_preprocess_tags(&$vars) {
  $tags = &$vars['element'];
  $vars['image_path'] = url(PATH_XML_FORM_ELEMENTS_IMAGES); // Location of images.
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
 * Wrapper for Tags::AfterBuild, used as Drupal doesn't support static calls in this situation.
 *
 * Modifies the provided tag element.
 *
 * @param array $form_element
 *   A tag element to be modified.
 * @param array $form_state
 *   The Drupal form state.
 * @return array
 *   The modified $form_element.
 */
function xml_form_elements_after_build_tags(array $form_element, array &$form_state) {
  return Tags::AfterBuild($form_element, $form_state);
}
