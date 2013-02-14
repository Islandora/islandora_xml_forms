<?php

/**
 * @file
 *
 * The ingest metadata form.
 */

/**
 * The ingest form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 * @param array $associations
 *   The form associations to select from.
 *
 * @return array
 *   A drupal form definition.
 */
function xml_form_builder_ingest_form(array $form, array &$form_state) {
  form_load_include($form_state, 'inc', 'xml_form_builder', 'ingest.form');

  $association_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_association_step');
  $association = $association_step_storage['association'];

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
 * Updates the ingestable object's datastream as defined by the given
 * association.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_ingest_form_submit(array $form, array &$form_state) {
  $association_step_storage = &islandora_ingest_form_get_step_storage($form_state, 'xml_form_builder_association_step');
  $association = $association_step_storage['association'];

  $object = islandora_ingest_form_get_object($form_state);
  $xml_form = new XMLForm($form_state);
  $document = $xml_form->submit($form, $form_state);
  $title_field = drupal_array_get_nested_value($form, $association['title_field']);
  $label = $title_field['#value'];
  xml_form_builder_update_object($object, $association, $document, $label);

  // XXX: Restricts to the usage of a single content model.
  $configuration = &islandora_ingest_form_get_configuration($form_state);
  $configuration['models'] = array($association['content_model']);

  $association_step_storage['association_used'] = TRUE;
}
