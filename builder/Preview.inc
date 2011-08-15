<?php

// $Id$

/**
 * @file
 * 
 * Provides hooks for render and processing the Preview form.
 */

/**
 * Render the preview form.
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
  try {
    module_load_include('inc', 'xml_form_api', 'XMLForm');
    $xml_form = new XMLForm($form_state);
    if (!$xml_form->isInitialized()) { // Was initialized from storage.
      module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
      module_load_include('inc', 'xml_form_api', 'XMLDocument');
      $xml_definition = FormBuilder::GetFormDefinition($form_name);
      $form = XMLFormDefinition::GetDrupalForm($xml_definition);
      $properties = XMLFormDefinition::GetFormProperties($xml_definition);
      $properties = $properties['document'];
      try {
        $root = $properties['root'];
        $root_name = $root['local_name']; // @todo change to name.
        $default_uri = $root['uri'];
        $schema_uri = $properties['schema'];
        $namespaces = $properties['namespaces'];
        $document = new XMLDocument($root_name, $default_uri, $schema_uri, $namespaces);
      } catch (Exception $e) {
        drupal_set_message($e->getMessage(), 'error');
        return array(); // Empty Form.
      }
      $xml_form->initialize($form, $document);
    }
    $output = $xml_form->toArray();
  } catch (Exception $e) {
    $msg = "File: {$e->getFile()}<br/>Line: {$e->getLine()}<br/>Error: {$e->getMessage()}";
    drupal_set_message($msg, 'error');
    return array();
  }
  return $output;
}

/**
 * Process the preview form and pretty print the resulting document.
 *
 * @param array $form
 * @param array $form_state 
 */
function xml_form_builder_preview_submit(array $form, array &$form_state) {
  module_load_include('inc', 'xml_form_api', 'XMLForm');
  try {
    $xml_form = new XMLForm($form_state);
    $doc = $xml_form->submit($form, $form_state);
    //drupal_set_message('the fuck');
    $document = new DOMDocument();
    $document->loadXML('<root>testing</root>');
    //dom_document_pretty_print($document);
    //drupal_set_message('is this');
    exit();
  } catch (Exception $e) {
    $msg = "File: {$e->getFile()}<br/>Line: {$e->getLine()}<br/>Error: {$e->getMessage()}";
    drupal_set_message($msg, 'error');
    $form_state['rebuild'] = true;
  }
}