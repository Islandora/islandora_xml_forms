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
  $form = xml_form_builder_get_form($form_state, $form_name);
  if ($form === FALSE) {
    return array();
  }
  /**
   * Deals with a bug in drupals AHAH, where the user would be redirected to a different page.
   * This is fixed in Drupal 7.
   */
  $form['#action'] = url("admin/content/xml/form/$form_name/view"); // AHAH bug workaround. 
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
    $message = 'File: ' . $e->getFile() . '</br>';
    $message .= 'Line: ' . $e->getLine() . '</br>';
    $message .= 'Error: ' . $e->getMessage() . '</br>';
    drupal_set_message($message, 'error');
    $form_state['rebuild'] = TRUE;
  }
}