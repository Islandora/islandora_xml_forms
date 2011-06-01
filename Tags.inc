<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_elements', 'Tag');

class Tags {

  /**
   *
   * @param array $element
   * @param type $edit
   * @param array $form_state
   * @param array $complete_form 
   */
  public static function Process(array $element, $edit, array &$form_state, array $complete_form = NULL) {
    $element['#id'] = $element['#hash'];
    $element['#prefix'] = "<div class='clear-block' id='{$element['#hash']}'>";
    $element['#suffix'] = '</div>';
    return $element;
  }

}

/**
 * Preprocess tabs theme hook.
 * 
 * @param array $vars 
 */
function xml_form_elements_preprocess_tags(&$vars) {
  $path = drupal_get_path('module', 'xml_form_elements');
  // Javascript
  drupal_add_js("$path/js/jquery-ui-1.8.7.custom.min.js");
  $vars['scripts'] = drupal_get_js();
  // CSS
  drupal_add_css("$path/stylesheets/tags.css");
  //var_dump($vars);
  $tabs = &$vars[0];
  $vars['input']['id'] = "input-{$tabs['#id']}";
  $vars['input']['name'] = $tabs['#name'];
  $vars['input']['value'] = $tabs['#value'];
  $vars['label'] = $tabs['#title'];
}