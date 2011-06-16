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
    '#attributes' => array(
      'enctype' => "multipart/form-data"
    ),
    'form_name' => array(
      '#type' => 'textfield',
      '#title' => 'Form Name',
      '#required' => TRUE,
    ),
    'file' => array(
      '#type' => 'file',
      '#title' => t('Form Definition'),
      '#size' => 64,
      '#description' => t('An XML Form Definition, to associate with this file.'),
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
  $file_uploaded = $_FILES['files']['error']['file'] == 0;
  if ($file_uploaded) {
    if ($_FILES['files']['type']['file'] == 'text/xml') {
      module_load_include('inc', 'xml_form_api', 'FormDefinition');
      $definition = new DOMDocument();
      $definition->load($_FILES['files']['tmp_name']['file']);
      if (!FormDefinition::isValid($definition)) {
        form_set_error('files', "The given form definition is not valid.");
      }
    }
    else {
      form_set_error('files', "Form definitions must have a type of text/xml.");
    }
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
      $file_uploaded = $_FILES['files']['error']['file'] == 0;
      if ($file_uploaded) {
        $definition = new DOMDocument();
        $definition->load($_FILES['files']['tmp_name']['file']);
        FormBuilder::SetFormDefinition($form_name, $definition);
      }
      else { // Use the default template.
        $path = drupal_get_path('module', 'xml_form_builder');
        $definition = new DOMDocument();
        $definition->load("$path/FormDefinition.xml");
        FormBuilder::SetFormDefinition($form_name, $definition);
      }
      $form_state['redirect'] = FormBuilder::EditPath($form_name);
      return;
    }
    else {
      drupal_set_message("Failed to create form: $form_name.", 'error');
    }
  }
  $form_state['redirect'] = FormBuilder::BASE_PATH;
}