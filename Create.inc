<?php

// $Id$

/**
 * @file
 *
 * Callbacks and functions for the Create Form Page.
 */

/**
 * Show the Create Form.
 * 
 * @param array $form_state
 * @return array
 */
function xml_form_builder_create(array &$form_state) {
  if (isset($_POST['op']) && $_POST['op'] == t('Cancel')) {
    module_load_include('inc', 'xml_form_builder', 'FormBuilder');
    drupal_goto(FormBuilder::BASE_PATH);
  }
  return array(
    'form_name' => array(
      '#type' => 'textfield',
      '#title' => 'Form Name',
      '#required' => TRUE,
    ),
    'create' => array(
      '#type' => 'submit',
      '#value' => t('Create'),
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    ),
  );
}

/**
 * Validate the create form. 
 * 
 * Form names must be unique.
 * 
 * @param array $form
 * @param array $form_state 
 */
function xml_form_builder_create_validate(array $form, array &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'FormBuilder');
  $form_name = &$form_state['values']['form_name'];
  if (!FormBuilder::IsValidFormName($form_name)) {
    form_set_error('form_name', "The given form name '$form_name' is invalid. Form names must start with a letter or underscore and they can not contain spaces or special charcters.");
  }
  else if (FormBuilder::FormExists($form_name)) {
    form_set_error('form_name', "The given form name '$form_name' is already in use. Form names must be unique.");
  }
}

/**
 * Create form submitted. Redirect based on what button was clicked.
 * 
 * @param array $form
 * @param array $form_state 
 */
function xml_form_builder_create_submit(array $form, array &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'FormBuilder');
  $form_name = $form_state['values']['form_name'];
  $clicked = $form_state['clicked_button']['#value'];
  if ($clicked == t('Create')) {
    if (FormBuilder::CreateForm($form_name)) {
      drupal_set_message("Successfully created form: $form_name.");
      $form_state['redirect'] = FormBuilder::EditPath($form_name);
      return;
    }
    else {
      drupal_set_message("Failed to create form: $form_name.", 'error');
    }
  }
  $form_state['redirect'] = FormBuilder::BASE_PATH;
}