<?php

/**
 * @file
 *
 * STUFF
 */

/**
 * The select association form.
 *
 * This is used as part of the ingest process.
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
function xml_form_builder_select_association_form(array $form, array &$form_state, array $associations) {
  form_load_include($form_state, 'inc', 'xml_form_builder', 'includes/select_association.form');
  $form_state['associations'] = $associations;
  $models = array_map(function($o) { return $o['content_model']; }, $associations);
  $models = array_unique($models);
  $model_labels = array_map(function($o) { return islandora_object_load($o)->label; }, $models);
  $selected_model = isset($form_state['values']['models']) ? $form_state['values']['models'] : current($models);
  $selected_model_associations = array_filter($associations, function($o) use($selected_model) { return $o['content_model'] == $selected_model; });
  $forms = array_map(function($o) { return $o['form_name']; }, $selected_model_associations);
  $selected_form = isset($form_state['values']['forms']) ? $form_state['values']['forms'] : current($forms);
  $multiple_forms = count($forms) > 1;
  $multiple_models = count($models) > 1;
  return array(
    'models' =>  array(
      '#options' => $multiple_models ? array_combine($models, $model_labels) : NULL,
      '#title' => t('Select a Content Model'),
      '#type' => $multiple_models ? 'select' : 'hidden',
      '#default_value' => $selected_model,
      '#ajax' => array(
        'callback' => 'xml_form_builder_select_association_form_ajax_callback',
        'method' => 'replace',
        'effect' => 'fade',
        'event' => 'change',
        'wrapper' => 'form-select-wrapper'
      )
    ),
    'forms' => array(
      '#type' => $multiple_forms ? 'select' : 'hidden',
      '#title' => t('Select a Form'),
      '#options' => $multiple_forms ? array_combine($forms, $forms) : NULL,
      '#default_value' => $selected_form,
      '#prefix' => '<div id="form-select-wrapper">',
      '#suffix' => '</div>'
    )
  );
}

/**
 * The AJAX callback for the Content Model select field in the select
 * association form.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   A drupal form definition.
 */
function xml_form_builder_select_association_form_ajax_callback(array $form, array $form_state) {
  return $form['forms'];
}

/**
 * Submit handler modifies the metadata step. Reverting the changes if the
 * association has changed.
 *
 * @param array $form
 *   The Drupal form.
 * @param array $form_state
 *   The Drupal form state.
 */
function xml_form_builder_select_association_form_submit(array $form, array &$form_state) {
  $values = $form_state['values'];
  $association_matches = function($o) use($values) { return $o['content_model'] == $values['models'] && $o['form_name'] == $values['forms']; };
  $association = array_pop(array_filter($form_state['associations'], $association_matches));
  $steps = &islandora_ingest_form_get_steps($form_state);
  $metadata_step_index = NULL;
  array_walk($steps, function($v, $k) use(&$metadata_step_index) {
      if (isset($v['form_id']) && $v['form_id'] == 'xml_form_builder_ingest_form') {
        $metadata_step_index = $k;
      }
    }
  );
  $metadata_step = &$steps[$metadata_step_index];
  $original_association = $metadata_step['args'][0];
  if ($association != $original_association) {
    $object = $form_state['islandora']['objects'][0];
    $dsid = $original_association['dsid'];
    $object->purgeDatastream($dsid);
    unset($metadata_step['values']);
    $metadata_step['args'] = array($association, $object);
  }
}
