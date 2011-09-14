<?php

/**
 * @file
 * 
 * Callbacks and functions used in the Preview Page.
 */

/**
 * Render the preview form.
 * 
 * This show the user what their form will look like. When it is submitted it will show the user what the generated XML
 * Looks like.
 * 
 * @param array $form_state
 *   The Drupal Form State.
 * @param string $form_name
 *   The name of the form to preview.
 * 
 * @return array
 *   A Drupal Form that repersents the given form, identified by $form_name.
 */
function xml_form_builder_preview(array &$form_state, $form_name) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormDatabase');
  module_load_include('inc', 'xml_form_api', 'XMLForm');
  module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
  if (!XMLFormDatabase::Exists($form_name)) {
    drupal_set_message(t('The form "%name" does not exist.', array('%name' => $form_name)));
    drupal_not_found();
    exit();
  }
  try {
    $xml_form = new XMLForm($form_state);
    if (!$xml_form->isInitialized()) { // Was not initialized from storage.
      $definition = new XMLFormDefinition(XMLFormDatabase::Get($form_name));
      $xml_form->initialize($definition->getForm(), $definition->createXMLDocument());
    }
    $form = $xml_form->toArray($form_state);
    /**
     * Deals with a bug in drupals AHAH, where the user would be redirected to a different page.
     * This is fixed in Drupal 7.
     */
    $form['#action'] = url("admin/content/xml/form/$form_name/view"); // AHAH bug workaround.
  } catch (Exception $e) {
    /**
     * Catch any errors that occured and display them to the user so that they may correct their form definition.
     */
    $msg = "File: {$e->getFile()}<br/>Line: {$e->getLine()}<br/>Error: {$e->getMessage()}";
    drupal_set_message(filter_xss($msg), 'error');
    return array(); // Empty Form.
  }
  return $form;
}

/**
 * Submit the preview form.
 * 
 * Render the generated metadata document.
 *
 * @param array $form
 *   The Drupal Form.
 * @param array $form_state 
 *   The Drupal Form State.
 */
function xml_form_builder_preview_submit(array $form, array &$form_state) {
  module_load_include('inc', 'xml_form_api', 'XMLForm');
  try {
    $xml_form = new XMLForm($form_state);
    $document = $xml_form->submit($form, $form_state);
    dom_document_pretty_print($document->document);
    exit();
  } catch (Exception $e) {
    drupal_set_message(check_plain("File: {$e->getFile()}"), 'error');
    drupal_set_message(check_plain("Line: {$e->getLine()}"), 'error');
    drupal_set_message(check_plain("Error: {$e->getMessage()}"), 'error');
    $form_state['rebuild'] = TRUE;
  }
}