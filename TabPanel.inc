<?php

// $Id$

/**
 * @file
 *
 *  Allows forms to dynamically add new tabs to a tabpanel in a form.
 */
module_load_include('inc', 'xml_form_elements', 'Element');
module_load_include('inc', 'xml_form_api', 'Form');


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
    return "<div id='{$element['#hash']}'>{$element['#children']}</div>";
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
    $tabpanel = Element::find($element['#hash'], $form_state);
    $add = self::createAddButton($tabpanel);
    $delete = self::createRemoveButton($tabpanel);
    $element[self::AddButton] = $add->toDrupalForm();
    $element[self::DeleteButton] = $delete->toDrupalForm();
    return $element;
  }

  /**
   * Creates a button that allows tabpanels to duplicate.
   * 
   * @param FormElement $tabpanel
   * @return FormElement 
   */
  private static function createAddButton(FormElement $tabpanel) {
    $tabs = $tabpanel->parent;
    $add = new FormElement();
    $add->type = 'submit';
    $add->weight = 4;
    $add->size = 30;
    $add->value = 'Add';
    $add->prefix = '<div class="ui-tabpanel-add-button">';
    $add->suffix = '</div>';
    $add->ahah = array(
      'path' => "xml_form_elements/ahah/duplicate/{$tabpanel->hash}/{$tabs->hash}",
      'wrapper' => $tabs->hash, // The parents wrapper is set to the parents hash.
      'method' => 'replace',
      'effect' => 'fade'
    );
    $tabpanel[self::AddButton] = $add; // Add to the definition.
    return $add;
  }

  /**
   * Creates a remove button that allows the user to remove this tabpanel.
   * 
   * @param FormElement $tabpanel
   * @return FormElement 
   */
  private static function createRemoveButton(FormElement $tabpanel) {
    $tabs = $tabpanel->parent;
    $delete = new FormElement();
    $delete->type = 'submit';
    $delete->weight = 4;
    $delete->size = 30;
    $delete->value = 'Delete';
    $delete->attributes = array(
      'style' => 'display:none'
    );
    $delete->prefix = '<div class="ui-tabpanel-delete-button">';
    $delete->suffix = '</div>';
    $delete->ahah = array(
      'path' => "xml_form_elements/ahah/remove/{$tabpanel->hash}/{$tabs->hash}",
      'wrapper' => $tabs->hash, // The parents wrapper is set to the parents hash.
      'method' => 'replace',
      'effect' => 'fade'
    );
    $tabpanel[self::DeleteButton] = $delete; // Add to the definition.
    return $delete;
  }

}

/**
 * Theme hook for a tabpanel
 */
function theme_tabpanel($element) {
  return TabPanel::Theme($element);
}

