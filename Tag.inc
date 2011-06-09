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
  public static function Add($tags) {
    // Get the value from the textfield hash
    // Duplicate the textfield hash
    // Set the textfield value to be blank
    // Render
    
    // Get Form and Submitted value. Set the input field value to ''.
    list($form_id, $form_build_id, $drupal_form, $form_state) = Ahah::getFormInfo();
    $drupal_form = Ahah::processForm($form_id, $drupal_form, $form_state);
    $tags = find_element($drupal_form, $tags);
    $children = get_element_children($tags, 'tag');
    // 
    $input = &$tags[array_shift($children)];
    $input_hash = $input['#hash'];
    $input_value = $input['#value'];
    form_set_value($input, '', $form_state); // Set input to ''.
    
    // Duplicate Element, set its default value to the submitted value.
    $form = new XMLForm($form_state);
    $input = $form->findElement($input_hash);
    $new_element = $form->duplicateOriginal($input_hash);
    $new_element['#default_value'] = $input_value; // Get the value from input.
    $input->parent[] = $new_element; // Add to the form.
    // Rebuild.
    $drupal_form = Ahah::rebuildForm($form_id, $form_build_id, $drupal_form, $form_state);
    // Find the parent element to render.
    $element = find_element($drupal_form, $tags['#hash']);
    // Respond to the request with JSON.
    Ahah::respond($element);
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