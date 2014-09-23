<?php

/**
 * @file
 * Allows for adding / editing XML datastreams.
 */

/**
 * Datastream form.
 *
 * Displays a select association form if more than one association is defined
 * for the given datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param AbstractObject $object
 *   The object to add the datastream to.
 * @param string $dsid
 *   The datastream to add / edit.
 *
 * @return array
 *   The form for selecting the association to add the datastream, or the XML
 *   form for adding the datastream.
 */
function xml_form_builder_datastream_form(array $form, array &$form_state, AbstractObject $object, $dsid) {
  form_load_include($form_state, 'inc', 'xml_form_builder', 'includes/datastream.form');
  // Leave this here for legacy reasons.
  $form_state['datastream'] = isset($object[$dsid]) ? $object[$dsid] : FALSE;
  $associations = xml_form_builder_datastream_form_get_associations($form_state, $object->models, $dsid);
  $association = xml_form_builder_datastream_form_get_association($form_state, $associations);
  return isset($association) ?
    xml_form_builder_datastream_form_metadata_form($form, $form_state, $object, $association) :
    xml_form_builder_datastream_form_select_association_form($form, $form_state, $associations);
}

/**
 * Gets the form associations for the given content models and datastream.
 *
 * @param array $form_state
 *   The drupal form state.
 * @param string[] $models
 *   The content models to match against.
 * @param string $dsid
 *   The Datastream ID to match against.
 *
 * @return array
 *   An array of found associations.
 */
function xml_form_builder_datastream_form_get_associations(array &$form_state, $models, $dsid) {
  module_load_include('inc', 'xml_form_builder', 'includes/associations');
  if (isset($form_state['associations'])) {
    return $form_state['associations'];
  }
  return xml_form_builder_get_associations(array(), $models, array($dsid));
}

/**
 * Gets the selected form association.
 *
 * If only one association exists, it is used.
 *
 * @param array $form_state
 *   The drupal form state.
 * @param array $associations
 *   A passed in array of associations.
 *
 * @return array|NULL
 *   The selected association, or NULL if no association is selected.
 */
function xml_form_builder_datastream_form_get_association(array &$form_state, array $associations) {
  if (isset($form_state['association'])) {
    return $form_state['association'];
  }
  return count($associations) == 1 ? current($associations) : NULL;
}

/**
 * Gets the select associations form.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param array $associations
 *   The associations to choose from.
 *
 * @return array
 *   The form for selecting the association to edit the datastream.
 */
function xml_form_builder_datastream_form_select_association_form(array $form, array &$form_state, array $associations) {
  module_load_include('inc', 'xml_form_builder', 'includes/select_association.form');
  $form_state['associations'] = $associations;
  $form = xml_form_builder_select_association_form($form, $form_state, $associations);
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
    '#name' => 'next',
    '#submit' => array('xml_form_builder_datastream_form_select_association_form_submit'),
  );
  return $form;
}

/**
 * Selects an association.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function xml_form_builder_datastream_form_select_association_form_submit(array $form, array &$form_state) {
  $model = $form_state['values']['models'];
  $form_name = $form_state['values']['available_forms'];
  $association_matches = function($o) use($model, $form_name) {
    return $o['content_model'] == $model && $o['form_name'] == $form_name;
  };
  $matching_associations = array_filter($form_state['associations'], $association_matches);
  $form_state['association'] = array_pop($matching_associations);
  $form_state['rebuild'] = TRUE;
}

/**
 * The edit datastream form.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param AbstractObject $object
 *   The object to which the datastream belongs.
 * @param array $association
 *   The association to use for creating the edit datastream form.
 *
 * @return array
 *   The Drupal form.
 */
function xml_form_builder_datastream_form_metadata_form(array $form, array &$form_state, AbstractObject $object, array $association) {
  $form_state['pid'] = $object->id;
  $form_state['association'] = $association;
  $form_name = $association['form_name'];
  $dsid = $association['dsid'];
  $xml = NULL;
  if (isset($object[$dsid])) {
    $xml = $object[$dsid]->content;
  }
  $form = xml_form_builder_get_form($form, $form_state, $form_name, $xml);
  // Make the Object label field required.
  if ($association['title_field']) {
    $title_field_parents = $association['title_field'];
    $title_field_parents[] = '#required';
    drupal_array_set_nested_value($form, $title_field_parents, TRUE);
  }
  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#name' => 'update',
    '#submit' => array('xml_form_builder_datastream_form_metadata_form_submit'),
  );
  return $form;
}

/**
 * Updates the datastream's content with generated xml from the metadata form.
 *
 * Also updates the objects label, and applies a transform to update the DC.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function xml_form_builder_datastream_form_metadata_form_submit(array $form, array &$form_state) {
  $object = islandora_object_load($form_state['pid']);
  $association = $form_state['association'];
  $label = NULL;
  if ($association['title_field']) {
    $title_field = drupal_array_get_nested_value($form, $association['title_field']);
    $label = $title_field['#value'];
  }
  $xml_form = new XMLForm($form_state);
  $document = $xml_form->submit($form, $form_state);
  xml_form_builder_update_object($object, $association, $document, $label);
  $form_state['redirect'] = "islandora/object/{$object->id}";
}
