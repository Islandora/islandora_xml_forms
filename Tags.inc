<?php

// $Id$

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
    $element['#id'] = $element['#hash'];
    $element['#tree'] = TRUE;
    $element['#prefix'] = "<div class='clear-block' id='{$element['#hash']}'>";
    $element['#suffix'] = '</div>';
    $element[self::AddButton] = self::createAddButton($element, $complete_form);
    return $element;
  }

  private static function createAddButton(array &$element, array &$complete_form) {
    $add['#type'] = 'submit';
    $add['#weight'] = 4;
    $add['#size'] = 30;
    $add['#value'] = 'Add';
    $add['#attributes'] = array('style' => 'display:none');
    $add['#ahah'] = array(
      'path' => "xml/form/elements/ahah/tag/add/{$element['#hash']}",
      'wrapper' => $element['#hash'], // The parents wrapper is set to the parents hash.
      'method' => 'replace',
      'effect' => 'fade'
    );
    return $add;
  }

}

/**
 * Preprocess tabs theme hook.
 * 
 * @param array $vars 
 */
function xml_form_elements_preprocess_tags(&$vars) {
  global $base_url;
  $tags = &$vars[0];
  $vars['add'] = $tags[Tags::AddButton]; 
  // Create tags.
  $vars['tags'] = array();
  $children = element_children($tags);
  foreach ($children as $key) {
    $child = $tags[$key];
    if ($child['#type'] == 'tag') {
      $vars['tags'][$key] = $child;
    }
  }
  // First tag is the textfield, all other's show up as icons.
  $vars['input'] = array_shift($vars['tags']);
  $vars['label'] = $tags['#title'];
  $vars['image_path'] = "$base_url/{$vars['directory']}/images";
}