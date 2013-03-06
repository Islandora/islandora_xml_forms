<?php

/**
 * @file
 *
 * Allows for the editing of existing xml datastreams using forms associated
 * with the given datastream.
 */

/**
 * Edit datastream form. Display's a select association form if more than one
 * association is defined for the given datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param FedoraDatastream $datastream
 *   The datastream to edit.
 *
 * @return array
 *   The form for selecting the association to edit the datastream, or the xml
 *   form for editing the datastream.
 */
function xml_form_builder_edit_datastream_form(array $form, array &$form_state, FedoraDatastream $datastream) {
  form_load_include($form_state, 'inc', 'xml_form_builder', 'includes/edit_datastream');
  $form_state['datastream'] = $datastream;
  $associations = xml_form_builder_edit_datastream_form_get_associations($form_state, $datastream);
  $association = xml_form_builder_edit_datastream_form_get_association($form_state, $associations);
  return isset($association) ?
    xml_form_builder_edit_datastream_form_metadata_form($form, $form_state, $association) :
    xml_form_builder_edit_datastream_form_select_association_form($form, $form_state, $associations);
}

/**
 * Gets the form associations for the given datastream.
 *
 * @param array $form_state
 *   The drupal form state.
 * @param FedoraDatastream $datastream
 *   The datastream to edit.
 */
function xml_form_builder_edit_datastream_form_get_associations(array &$form_state, FedoraDatastream $datastream) {
  module_load_include('inc', 'xml_form_builder', 'includes/associations');
  if (isset($form_state['associations'])) {
    return $form_state['associations'];
  }
 return xml_form_builder_get_associations(array(), $datastream->parent->models, array($datastream->id));
}

/**
 * Gets the selected form association. If only one association exists it is
 * used.
 *
 * @param array $form_state
 *   The drupal form state.
 * @param FedoraDatastream $datastream
 *   The datastream to edit.
 *
 * @return array
 *   The selected association, or NULL if no association is selected.
 */
function xml_form_builder_edit_datastream_form_get_association(array &$form_state, array $associations) {
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
function xml_form_builder_edit_datastream_form_select_association_form(array $form, array &$form_state, array $associations) {
  module_load_include('inc', 'xml_form_builder', 'includes/select_association.form');
  $form_state['associations'] = $associations;
  $form = xml_form_builder_select_association_form($form, $form_state, $associations);
  $form['next'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
    '#name' => 'next',
    '#submit' => array('xml_form_builder_edit_datastream_form_select_association_form_submit'),
  );
  return $form;
}

/**
 * Selects a association.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function xml_form_builder_edit_datastream_form_select_association_form_submit(array $form, array &$form_state) {
  $model = $form_state['values']['models'];
  $form_name = $form_state['values']['available_forms'];
  $association_matches = function($o) use($model, $form_name) {
    return $o['content_model'] == $model && $o['form_name'] == $form_name;
  };
  $form_state['association'] = array_pop(array_filter($form_state['associations'], $association_matches));
  $form_state['rebuild'] = TRUE;
}

/**
 * The edit datastream form.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 * @param array $association
 *   The association to use for creating the edit datastream form.
 */
function xml_form_builder_edit_datastream_form_metadata_form(array $form, array &$form_state, array $association) {
  $form_state['association'] = $association;
  $form_name = $association['form_name'];
  $datastream = $form_state['datastream'];
  $xml = $datastream->content;
  $form = xml_form_builder_get_form($form, $form_state, $form_name, $xml);
  // Make the Object label field required.
  $title_field_parents = $association['title_field'];
  $title_field_parents[] = '#required';
  $title_field = drupal_array_set_nested_value($form, $title_field_parents, TRUE);
  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Update'),
    '#name' => 'update',
    '#submit' => array('xml_form_builder_edit_datastream_form_metadata_form_submit'),
  );
  return $form;
}

/**
 * Updates the content of the stored datastream with the generated xml from the
 * metadata form. Also updates the objects label, and applies a transform to
 * update the DC.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function xml_form_builder_edit_datastream_form_metadata_form_submit(array $form, array &$form_state) {
  $datastream = $form_state['datastream'];
  $association = $form_state['association'];
  $object = $datastream->parent;

  $title_field = drupal_array_get_nested_value($form, $association['title_field']);
  $label = $title_field['#value'];

  $xml_form = new XMLForm($form_state);
  $document = $xml_form->submit($form, $form_state);
  xml_form_builder_update_object($object, $association, $document, $label);
  $form_state['redirect'] = "islandora/object/{$object->id}/manage";
}
