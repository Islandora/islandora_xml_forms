<?php

/**
 * @file
 *
 * Callbacks and functions for the List Forms Page.
 */

/**
 * Show the List Form.
 * 
 * @param array $form_state
 * @return array
 */
function xml_form_builder_list(array &$form_state) {
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
    //$path = drupal_get_path('module', 'xml_form_builder');
    //drupal_add_js($path . '/js/Export.js');
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
          '#name' => 'edit'
        ),
        'view' => array(
          '#type' => 'submit',
          '#value' => t('View'),
          '#name' => 'view'
        ),
        'export' => array(
          '#type' => 'submit', //'button',
          /*'#attributes' => array(
            'onclick' => 'return false;'
          ),*/
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
 * Submit list form. Redirect based on what button was clicked.
 * 
 * @param array $form
 * @param array $form_state 
 */
function xml_form_builder_list_submit(array $form, array &$form_state) {
  $clicked = $form_state['clicked_button']['#name'];
  $form_name = $form_state['values']['form_name'];
  $redirect = array(
    'create' => MENU_XML_FORM_BUILDER_CREATE,
    'copy' => xml_form_builder_get_copy_form_path($form_name),
    'edit' => xml_form_builder_get_edit_form_path($form_name),
    'view' => xml_form_builder_get_view_form_path($form_name),
    'export' => xml_form_builder_get_export_form_path($form_name),
    'delete' => xml_form_builder_get_delete_form_path($form_name),
  );
  $form_state['redirect'] = isset($redirect[$clicked]) ? $redirect[$clicked] : FALSE;
}