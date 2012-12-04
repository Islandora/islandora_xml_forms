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
  $selected_form = (isset($form_state['values']['forms']) && in_array($form_state['values']['forms'], $forms)) ? $form_state['values']['forms'] : current($forms);
  $multiple_forms = count($forms) > 1;
  $multiple_models = count($models) > 1;

  $form['models'] =  array(
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
  );
  if ($multiple_forms) {
    $form['forms'] = array(
      '#type' => 'select',
      '#title' => t('Select a Form'),
      '#options' => array_combine($forms, $forms),
      '#default_value' => $selected_form,
    );
  }
  else {
    // XXX: This should be using #default_value, but this leads to the old
    //   previous values being kept in the #value, and therefore markup
    //   containing the old value being supplied to the client after AJAX
    //   requests...  #value means it can not be changed by users, but that
    //   shouldn't be an issue...  There is an issue with not regenerating the
    //   cache, but that is handled below.
    $form['forms'] = array(
      '#type' => 'hidden',
      '#value' => $selected_form,
    );
  }

  $form['forms'] += array(
    '#prefix' => '<div id="form-select-wrapper">',
    '#suffix' => '</div>'
  );

  return $form;
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
  // XXX: This form_set_cache() should not be necessary, but there seems to be
  //   an issue with how the form API handles hidden fields (and therefore
  //   affecting when there is a single form)...
  form_set_cache($form_state['values']['form_build_id'], $form, $form_state);
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
  $association_matches = function($o) use($values) {
    return $o['content_model'] == $values['models'] && $o['form_name'] == $values['forms'];
  };
  $filtered_associations = array_filter($form_state['associations'], $association_matches);
  $association = array_pop($filtered_associations);

  $metadata_step = &xml_form_builder_get_metadata_step($form_state);
  $original_association = $metadata_step['args'][0];
  if ($association != $original_association) {
    $object = $form_state['islandora']['objects'][0];
    $dsid = $original_association['dsid'];
    $object->purgeDatastream($dsid);
    unset($metadata_step['values']);
    $metadata_step['args'] = array($association, $object);
  }
}

/**
 * Grab the step entry for the metadata ingest form.
 *
 * @param array $form_state
 * @return array
 *   Reference to the array defining the ingest step.
 */
function &xml_form_builder_get_metadata_step(array &$form_state) {
  $steps = &islandora_ingest_form_get_steps($form_state);
  $metadata_step_index = NULL;
  array_walk($steps, function($v, $k) use(&$metadata_step_index) {
      if (isset($v['form_id']) && $v['form_id'] == 'xml_form_builder_ingest_form') {
        $metadata_step_index = $k;
      }
    }
  );
  return $steps[$metadata_step_index];
}

