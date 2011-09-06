<?php

// $Id$

/**
 * @file
 *
 *  Allows forms to dynamically add new tabs to a tabpanel in a form.
 */
//module_load_include('inc', 'xml_form_elements', 'Element');
//module_load_include('inc', 'xml_form_api', 'XMLForm');
module_load_include('inc', 'php_lib', 'DrupalFormHelpers');

/**
 * A collection of static functions that allow for theming and processing tabpanels.
 */
class TabPanel {
  /**
   * Constants
   */
  const AddButton = 'add-tabpanel';
  const DeleteButton = 'delete-tabpanel';

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
    if ($element['#user_data']['add']) {
      $element[self::AddButton] = self::createAddButton($element, $complete_form);
    }
    if ($element['#user_data']['delete']) {
      $element[self::DeleteButton] = self::createRemoveButton($element, $complete_form);
    }
    return $element;
  }

  /**
   * Creates a button that allows tabpanels to duplicate.
   * 
   * @param FormElement $tabpanel
   * @return FormElement 
   */
  private static function createAddButton(array &$element, array& $complete_form) {
    $tabs = get_form_element_parent($element, $complete_form);
    $add['#type'] = 'submit';
    $add['#weight'] = 4;
    $add['#size'] = 30;
    $add['#id'] = $element['#hash'] . '-add';
    $add['#value'] = 'Add';
    $add['#prefix'] = '<div class="ui-tabpanel-add-button">';
    $add['#suffix'] = '</div>';
    $add['#ahah'] = array(
      'path' => "xml/form/elements/ahah/duplicate/{$element['#hash']}/{$tabs['#hash']}",
      'wrapper' => $tabs['#hash'], // The parents wrapper is set to the parents hash.
      'method' => 'replace',
      'effect' => 'fade'
    );
    return $add;
  }

  /**
   * Creates a remove button that allows the user to remove this tabpanel.
   * 
   * @param FormElement $tabpanel
   * @return FormElement 
   */
  private static function createRemoveButton(array &$element, array& $complete_form) {
    $tabs = get_form_element_parent($element, $complete_form);
    $delete['#type'] = 'submit';
    $delete['#weight'] = 4;
    $delete['#size'] = 30;
    $delete['#id'] = $element['#hash'] . '-remove';
    $delete['#value'] = 'Delete';
    $delete['#attributes'] = array(
      'style' => 'display:none' // Triggered via javascript.
    );
    $delete['#prefix'] = '<div class="ui-tabpanel-delete-button">';
    $delete['#suffix'] = '</div>';
    $delete['#ahah'] = array(
      'path' => "xml/form/elements/ahah/remove/{$element['#hash']}/{$tabs['#hash']}",
      'wrapper' => $tabs['#hash'], // The parents wrapper is set to the parents hash.
      'method' => 'replace',
      'effect' => 'fade'
    );
    return $delete;
  }

}

/**
 * Theme hook for a tabpanel
 */
function theme_tabpanel($element) {
  return TabPanel::Theme($element);
}

