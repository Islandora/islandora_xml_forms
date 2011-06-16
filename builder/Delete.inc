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
function xml_form_builder_delete(array &$form_state, $form_name) {
  module_load_include('inc', 'xml_form_builder', 'FormBuilder');
  if (!FormBuilder::FormExists($form_name)) {
    drupal_set_message(t("Form '$form_name' does not exist."));
    drupal_not_found();
    exit();
  }
  return array(
    'form_name' => array(
      '#type' => 'hidden',
      '#value' => $form_name,
    ),
    'description' => array(
      '#type' => 'markup',
      '#value' => "<div>Are you sure you want to delete the form <b>\"$form_name\"</b>. This action is irreversible.</div>",
    ),
    'delete' => array(
      '#type' => 'submit',
      '#value' => t('Delete'),
    ),
    'cancel' => array(
      '#type' => 'submit',
      '#value' => t('Cancel'),
    ),
  );
}

/**
 * List form submitted. Redirect based on what button was clicked.
 * 
 * @param array $form
 * @param array $form_state 
 */
function xml_form_builder_delete_submit(array $form, array &$form_state) {
  if ($form_state['clicked_button']['#value'] == 'Delete') {
    module_load_include('inc', 'xml_form_builder', 'FormBuilder');
    $form_name = $form_state['values']['form_name'];
    if(FormBuilder::DeleteForm($form_name)) {
      drupal_set_message("Successfully deleted form '$form_name'.");
    }
    else {
      drupal_set_message("Failed to deleted form '$form_name'.", 'error');
    }
  }
  $form_state['redirect'] = FormBuilder::BASE_PATH;
}