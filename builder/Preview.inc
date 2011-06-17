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
  if (!FormBuilder::FormExists($form_name) || !FormBuilder::HasFormDefinition($form_name)) {
    drupal_set_message(t("Form '$form_name' does not exist, or it does not have a complete definition."));
    drupal_not_found();
    exit();
  }
  module_load_include('inc', 'xml_form_api', 'XMLForm');
  $xml_form = new XMLForm($form_state);
  if (!$xml_form->isInitialized()) { // Was initialized from storage.
    module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
    module_load_include('inc', 'xml_form_api', 'XMLDocument');
    $xml_definition = FormBuilder::GetFormDefinition($form_name);
    $form = XMLFormDefinition::GetDrupalForm($xml_definition);
    $properties = XMLFormDefinition::GetFormProperties($xml_definition);
    $properties = $properties['document'];
    $document = new XMLDocument($properties['root'], $properties['namespaces'], $properties['schema']);
    $xml_form->initialize($form, $document);
  }
  $output = $xml_form->toArray();
  return $output;
}