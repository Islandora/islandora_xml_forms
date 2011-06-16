<?php

// $Id$

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
  module_load_include('inc', 'xml_form_builder', 'FormBuilder');
  $names = FormBuilder::GetFormNames();
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
        ),
      )
    );
  }
  else {
    $path = drupal_get_path('module', 'xml_form_builder');
    drupal_add_js($path.'/js/Export.js');
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
        ),
        'copy' => array(
          '#type' => 'submit',
          '#value' => t('Copy'),
        ),
        'edit' => array(
          '#type' => 'submit',
          '#value' => t('Edit'),
        ),
        'view' => array(
          '#type' => 'submit',
          '#value' => t('View'),
        ),
        'export' => array(
          '#type' => 'button',
          '#attributes' => array(
            'onclick' => 'return false;'
          ),
          '#value' => 'Export',
        ),
        'delete' => array(
          '#type' => 'submit',
          '#value' => t('Delete'),
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
  module_load_include('inc', 'xml_form_builder', 'FormBuilder');
  $clicked = $form_state['clicked_button']['#value'];
  if ($clicked == t('Create')) {
    $form_state['redirect'] = FormBuilder::CreatePath();
  }
  else {
    $form_name = $form_state['values']['form_name'];
    $redirect = array(
      t('Copy') => FormBuilder::CopyPath($form_name),
      t('Edit') => FormBuilder::EditPath($form_name),
      t('View') => FormBuilder::ViewPath($form_name),
      t('Delete') => FormBuilder::DeletePath($form_name),
    );
    $form_state['redirect'] = isset($redirect[$clicked]) ? $redirect[$clicked] : FALSE;
  }
}