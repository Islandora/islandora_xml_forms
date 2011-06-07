<?php

// $Id$

/**
 * @file
 *
 *  Allows forms to dynamically add new tabs to a tabpanel in a form.
 */
module_load_include('inc', 'xml_form_elements', 'Element');

/**
 * A collection of static functions that allow for theming and processing tabpanels.
 */
class Tag {

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
  public static function Process(array $element, $edit, array &$form_state, array $complete_form = NULL) {
    $element['#id'] = $element['#hash'];
    return $element;
  }

  /**
   *
   * @param hash $textfield_hash
   */
  public static function Add($textfield_hash) {
    // Get the value from the textfield hash
    // Duplicate the textfield hash
    // Set the textfield value to be blank
    // Render
  }

  /**
   *
   * @param hash $textfield_hash
   * @param hash $tag_hash 
   */
  public static function Edit($textfield_hash, $tag_hash) {
    // Get the value from the textfield hash
    // Duplicate the textfield hash
    // Set the textfield value to be blank
    // Render
  }

  /**
   *
   * @param hash $tag_hash 
   */
  public static function Remove($tag_hash) {
    // Get the value from the textfield hash
    // Duplicate the textfield hash
    // Set the textfield value to be blank
    // Render
  }

}

/**
 * Theme hook for a tabpanel
 */
function theme_tag($element) {
  return Tag::Theme($element);
}