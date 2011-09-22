<?php

/**
 * @file
 *
 * Callbacks and functions for the Main Page.
 */

/**
 * Show the Main page.
 * 
 * Here the user can select which action they would like to do.
 * 
 * @param array $form_state
 *   The Drupal Form State.
 * 
 * @return array
 *   The Drupal form.
 */
function xml_form_builder_main(array &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormDatabase');
  $names = XMLFormDatabase::GetNames();
  if (count($names) == 0) { // No forms exist can only create.
    return array(
      array(
        '#type' => 'fieldset',
        '#title' => 'Forms',
        '#tree' => FALSE,
        'description' => array(
          '#type' => 'markup',
          '#value' => '<div>No forms are defined. Please create a new form.</div><br/>'
        ),
        'create' => array(
          '#type' => 'submit',
          '#value' => t('Create'),
          '#name' => 'create'
        ),
      )
    );
  }
  else {
    return array(
      array(
        '#type' => 'fieldset',
        '#title' => 'Forms',
        '#tree' => FALSE,
        'form_name' => array(
          '#type' => 'select',
          '#title' => 'Select a Form',
          '#options' => $names,
        ),
        'create' => array(
          '#type' => 'submit',
          '#value' => t('Create'),
          '#name' => 'create'
        ),
        'copy' => array(
          '#type' => 'submit',
          '#value' => t('Copy'),
          '#name' => 'copy'
        ),
        'edit' => array(
          '#type' => 'submit',
          '#value' => t('Edit'),
          /**
           * PHP is ridiculous. This was failing when the #name was edit due to this line 
           *  else if (isset($_REQUEST['edit']['destination'])) { #317 from common.inc
           * 
           * This like returned TRUE, despite the fact that $_REQUEST['edit'] was a string not an array.
           * I'm surpise any useful software can be build in such a enviroment.
           */
          '#name' => 'editor' 
        ),
        'view' => array(
          '#type' => 'submit',
          '#value' => t('View'),
          '#name' => 'view'
        ),
        'export' => array(
          '#type' => 'submit',
          '#value' => 'Export',
          '#name' => 'export'
        ),
        'delete' => array(
          '#type' => 'submit',
          '#value' => t('Delete'),
          '#name' => 'delete'
        ),
      ),
    );
  }
}

/**
 * Submit main form. Redirect based on what button was clicked.
 * 
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state 
 *   The Drupal Form State.
 */
function xml_form_builder_main_submit(array $form, array &$form_state) {
  $clicked = $form_state['clicked_button']['#name'];
  $form_name = isset($form_state['values']['form_name']) ? $form_state['values']['form_name'] : NULL;
  $redirect = array(
    'create' => MENU_XML_FORM_BUILDER_CREATE,
    'copy' => xml_form_builder_get_copy_form_path($form_name),
    'editor' => xml_form_builder_get_edit_form_path($form_name),
    'view' => xml_form_builder_get_view_form_path($form_name),
    'export' => xml_form_builder_get_export_form_path($form_name),
    'delete' => xml_form_builder_get_delete_form_path($form_name),
  );
  $form_state['redirect'] = isset($redirect[$clicked]) ? $redirect[$clicked] : FALSE;
}