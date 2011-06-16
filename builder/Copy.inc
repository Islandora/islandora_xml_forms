<?php

// $Id$

/**
 * @file
 *
 * Callbacks and functions for the Copy Form Page.
 */

/**
 * Show the Copy Form.
 * 
 * @param array $form_state
 * @return array
 */
function xml_form_builder_copy(array &$form_state, $original) {
  if (isset($_POST['op']) && $_POST['op'] == t('Cancel')) {
    module_load_include('inc', 'xml_form_builder', 'FormBuilder');
    drupal_goto(FormBuilder::BASE_PATH);
  }
  return array(
    'original' => array(
      '#type' => 'hidden',
      '#value' => $original,
    ),
    'form_name' => array(
      '#type' => 'textfield',
      '#title' => t('Form Name'),
      '#required' => TRUE,
    ),
    'copy' => array(
      '#type' => 'submit',
      '#value' => t('Copy'),
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    ),
  );
}

/**
 * Validate the copy form. 
 * 
 * Form names must be unique.
 * 
 * @param array $form
 * @param array $form_state 
 */
function xml_form_builder_copy_validate(array $form, array &$form_state) {
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
 * Copy form submitted. Redirect based on what button was clicked.
 * 
 * @param array $form
 * @param array $form_state 
 */
function xml_form_builder_copy_submit(array $form, array &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'FormBuilder');
  $clicked = $form_state['clicked_button']['#value'];
  if ($clicked == t('Copy')) {
    $original = $form_state['values']['original'];
    $form_name = $form_state['values']['form_name'];
    if (FormBuilder::CopyForm($original, $form_name)) {
      $form_state['redirect'] = FormBuilder::EditPath($form_name);
      drupal_set_message("Successfully copied form: $form_name.");
      return;
    }
    drupal_set_message("Failed to copy form: $form_name.", 'error');
  }
  $form_state['redirect'] = FormBuilder::BASE_PATH;
}