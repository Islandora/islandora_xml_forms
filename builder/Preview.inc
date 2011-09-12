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
      $definition = new DOMDocument();
      $definition->loadXML(FormBuilder::GetFormDefinition($form_name));
      $definition = new XMLFormDefinition($definition);
      $form = $definition->getForm();
      $document = $definition->createXMLDocument();
      $xml_form->initialize($form, $document);
    }
    $output = $xml_form->toArray($form_state);
    $output['#action'] = url("admin/content/xml/form/$form_name/view"); // Deals with a bug in drupals AHAH, where the user would be redirected to a different page.
  } catch (Exception $e) {
    $msg = "File: {$e->getFile()}<br/>Line: {$e->getLine()}<br/>Error: {$e->getMessage()}";
    drupal_set_message($msg, 'error');
    return array();
  }
  return $output;
}

/**
 *
 * @param array $form
 * @param array $form_state 
 */
function xml_form_builder_preview_validate(array $form, array &$form_state) {
  
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
    $document = $xml_form->submit($form, $form_state);
    dom_document_pretty_print($document->document);
    exit();
  } catch (Exception $e) {
    $msg = "File: {$e->getFile()}<br/>Line: {$e->getLine()}<br/>Error: {$e->getMessage()}";
    drupal_set_message($msg, 'error');
    $form_state['rebuild'] = true;
  }
}