<?php

// $Id$

/**
 * @file
 *
 */

/**
 *
 * @param array $form_state
 * @param string $form_name 
 * @return array
 */
function xml_form_builder_preview(array &$form_state, $form_name) {
  module_load_include('inc', 'xml_form_builder', 'FormBuilder');
  if (!FormBuilder::FormExists($form_name)) {
    drupal_set_message(t("Form '$form_name' does not exist."));
    drupal_not_found();
    exit();
  }
  module_load_include('inc', 'xml_form_api', 'Form');
  $form = new Form($form_state);
  if (!$form->isInitialized()) {
    $xml_definition = FormBuilder::GetFormDefinition($form_name);
    $form_definition = FormDefinition::createFromXMLDefiniton($xml_definition);
    $document = new Document($form_definition->properties->document);
    $form->initialize($form_definition, $document);
  }
  return $form->toDrupalForm();  
}