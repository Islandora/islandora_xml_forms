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
    drupal_goto(MENU_XML_FORM_BUILDER_MAIN);
  }
  return array(
    '#attributes' => array(
      'enctype' => "multipart/form-data"
    ),
    'form_name' => array(
      '#type' => 'textfield',
      '#title' => 'Form Name',
      '#required' => TRUE,
      '#maxlength' => 128,
      '#element_validate' => array('xml_form_builder_create_validate_name')
    ),
    'file' => array(
      '#type' => 'file',
      '#title' => t('Form Definition'),
      '#size' => 64,
      '#description' => t('An optional XML form definition template.'),
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

function xml_form_builder_create_validate_name(array $element, array &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormDatabase');
  $form_name = &$form_state['values']['form_name'];
  if (XMLFormDatabase::Exists($form_name)) {
    form_set_error('form_name', "The given form name '$form_name' is already in use. Form names must be unique.");
  }
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
  $file_uploaded = isset($_FILES['files']['error']['file']) && ($_FILES['files']['error']['file'] == 0);
  if ($file_uploaded) {
    if ($_FILES['files']['type']['file'] == 'text/xml') {
      module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
      $filename = $_FILES['files']['tmp_name']['file'];
      $definition = new DOMDocument();
      $definition->load($filename);
      try {
        $version = XMLFormDefinition::getVersion($definition);
        if (!XMLFormDefinition::isValid($definition, $version)) {
          form_set_error('files', "The given form definition is not valid.");
        }
      } catch (Exception $e) {
        form_set_error('files', $e->getMessage());
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
  module_load_include('inc', 'xml_form_builder', 'XMLFormDatabase');
  $form_name = $form_state['values']['form_name'];
  $clicked = $form_state['clicked_button']['#value'];
  if ($clicked == t('Create')) {
    $definition = xml_form_builder_create_get_uploaded_file();
    $definition = $definition ? $definition : xml_form_api_get_empty_form_definition();
    if (XMLFormDatabase::Create($form_name, $definition)) {
      drupal_set_message("Successfully created form: $form_name.");
      $form_state['redirect'] = xml_form_builder_get_edit_form_path($form_name);
      return;
    }
    else {
      drupal_set_message("Failed to create form: $form_name.", 'error');
    }
  }
  $form_state['redirect'] = MENU_XML_FORM_BUILDER_MAIN;
}

/**
 * Gets the uploaded file from the create form.
 *  
 * @return DOMDocument 
 *   The uploaded file if it exists, FALSE otherwise.
 */
function xml_form_builder_create_get_uploaded_file() {
  $file_uploaded = isset($_FILES['files']['error']['file']) && ($_FILES['files']['error']['file'] == 0);
  if ($file_uploaded) {
    $definition = new DOMDocument();
    $definition->load($_FILES['files']['tmp_name']['file']);
    return $definition;
  }
  return FALSE;
}