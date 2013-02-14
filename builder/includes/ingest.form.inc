<?php

/**
 * @file
 * The ingest metadata form.
 */

/**
 * The ingest form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param array $association
 *   The form association.
 *
 * @return array
 *   A drupal form definition.
 */
function xml_form_builder_ingest_form(array $form, array &$form_state, array $association) {
  form_load_include($form_state, 'inc', 'xml_form_builder', 'ingest.form');
  $form_name = $association['form_name'];
  $dsid = $association['dsid'];
  $object = islandora_ingest_form_get_object($form_state);
  $xml = isset($object[$dsid]) ? $object[$dsid]->content : NULL;
  $form = xml_form_builder_get_form($form, $form_state, $form_name, $xml);
  if ($form !== FALSE) {
    // Make the Object label field required.
    $title_element = $association['title_field'];
    $title_element[] = '#required';
    drupal_array_set_nested_value($form, $title_element, TRUE);
  }
  return $form;
}

/**
 * Updates the ingestable object's datastream.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_ingest_form_submit(array $form, array &$form_state) {
  $metadata_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_metadata_step');
  $association = $metadata_step_storage['association'];
  $object = islandora_ingest_form_get_object($form_state);
  $xml_form = new XMLForm($form_state);
  $document = $xml_form->submit($form, $form_state);
  $title_field = drupal_array_get_nested_value($form, $association['title_field']);
  $label = $title_field['#value'];
  $metadata_step_storage['created'] = xml_form_builder_update_object($object, $association, $document, $label);
}

/**
 * Undoes the submit, purging any datastreams created by this step.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_ingest_form_undo_submit(array $form, array &$form_state) {
  $metadata_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_metadata_step');
  $object = islandora_ingest_form_get_object($form_state);
  foreach ($metadata_step_storage['created'] as $dsid => $created) {
    if ($created) {
      $object->purgeDatastream($dsid);
    }
  }
  unset($metadata_step_storage['created']);
}
