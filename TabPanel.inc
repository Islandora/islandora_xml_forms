<?php

// $Id$

/* @file
 *
 *  Allows forms to dynamically add new tabs to a tabpanel in a form.
 */
module_load_include('inc', 'xml_form_elements', 'Utils');
module_load_include('inc', 'xml_form_api', 'Form');
module_load_include('inc', 'xml_form_api', 'Utils');

class TabPanel {
  /**
   * 
   */
  const AddButton = 'add-tabpanel';
  const DeleteButton = 'delete-tabpanel';

  public static function Process(array $element, $edit, array &$form_state, array $complete_form = NULL) {
    $tabpanel = self::findTabPanel($element['#hash'], $form_state);
    $add = self::createAddButton($tabpanel);
    $delete = self::createRemoveButton($tabpanel);
    $element[self::AddButton] = $add->toDrupalForm();
    $element[self::DeleteButton] = $delete->toDrupalForm();
    return $element;
  }

  private static function findTabPanel($hash, array &$form_state) {
    $form = new Form($form_state);
    $tabpanel = $form->findElement($hash);
    return $tabpanel;
  }

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
      'path' => 'xml_form_elements/ahah/tabpanel/duplicate/' . $tabpanel->hash,
      'wrapper' => $tabs->hash, // The parents wrapper is set to the parents hash.
      'method' => 'replace',
      'effect' => 'fade'
    );
    $tabpanel[self::AddButton] = $add; // Add to the definition.
    return $add;
  }

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
      'path' => 'xml_form_elements/ahah/tabpanel/delete/' . $tabpanel->hash,
      'wrapper' => $tabs->hash, // The parents wrapper is set to the parents hash.
      'method' => 'replace',
      'effect' => 'fade'
    );
    $tabpanel[self::DeleteButton] = $delete; // Add to the definition.
    return $delete;
  }

  public static function Duplicate($hash) {
    if (!isset($_REQUEST['form_build_id']))
      return;
    $form_build_id = $_REQUEST['form_build_id'];
    list($drupal_form, $form_state) = get_form_and_form_state($form_build_id);
    $form_id = $drupal_form['#parameters'][0];
    // Find the tabpanel in question.
    $form = new Form($form_state);
    $tabpanel = $form->findElement($hash);
    // Duplicate the tabpanel.
    $tabs = $tabpanel->parent;
    $form->duplicateElement($hash);    
    // Store Changes
    cache_form_and_form_state($form_build_id, $drupal_form, $form_state);
    // Populate the form
    $drupal_form = $form->toDrupalForm();
    $drupal_form = $form->populate($drupal_form, $form_state);
    // Render the changed bit.
    $drupal_form = form_builder($form_id, $drupal_form, $form_state); // Runs process functions and other needed bits.
    $tabs = Utils::getDrupalFormElement($tabs->hash, $drupal_form);
    ahah_return_html_via_json($tabs);
  }

  /**
   *
   * @param hash $hash 
   */
  public static function Remove($hash) {
    list($form_id, $form_build_id, $drupal_form, $form_state) = get_form_info($form_build_id);
    // Find the tabpanel in question.
    $form = new Form($form_state);
    $tabpanel = $form->findElement($hash);
    // Remove the tabpanel.
    $tabs = $tabpanel->parent;
    $form->removeElement($hash);    
    // Store Changes
    cache_form_and_form_state($form_build_id, $drupal_form, $form_state);
    // Populate the form
    $drupal_form = $form->toDrupalForm();
    $drupal_form = $form->populate($drupal_form, $form_state);
    // Render the changed bit.
    $drupal_form = form_builder($form_id, $drupal_form, $form_state); // Runs process functions and other needed bits.
    $tabs = Utils::getDrupalFormElement($tabs->hash, $drupal_form);
    ahah_return_html_via_json($tabs);
  }

  public static function Theme($element) {
    return "<div id='{$element['#hash']}'>{$element['#children']}</div>";
  }

}

/**
 * Theme hook for a tabpanel
 */
function theme_tabpanel($element) {
  return TabPanel::Theme($element);
}

