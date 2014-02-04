<?php

/**
 * @file
 * Handles the uploading of a text file as part of the ingest process.
 */

/**
 * Defines a file upload form for uploading the text file.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_test_cm_upload_form(array $form, array &$form_state) {
  $upload_size = min((int) ini_get('post_max_size'), (int) ini_get('upload_max_filesize'));
  $extensions = array('txt');
  $form = array();
  $form['file'] = array(
    '#title' => t('Text File'),
    '#type' => 'managed_file',
    '#required' => TRUE,
    '#description' => t('Select file to upload.<br/>Files must be less than <b>@size MB.</b><br/>Allowed file types: <b>@ext.</b>', array('@size' => $upload_size, '@ext' => $extensions[0])),
    '#default_value' => isset($form_state['values']['files']) ? $form_state['values']['files'] : NULL,
    '#upload_location' => 'temporary://',
    '#upload_validators' => array(
      'file_validate_extensions' => $extensions,
      // Assume it's specified in MB.
      'file_validate_size' => array($upload_size * 1024 * 1024),
    ),
  );

  return $form;
}

/**
 * Adds the uploaded file into the 'OBJ' datastream, and adds a thumbnail.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_test_cm_upload_form_submit(array $form, array &$form_state) {
  $object = $form_state['islandora']['objects'][0];
  if (empty($object['OBJ'])) {
    $ds = $object->constructDatastream('OBJ', 'M');
    $object->ingestDatastream($ds);
  }
  else {
    $ds = $object['OBJ'];
  }
  $txt_file = file_load($form_state['values']['file']);
  $txt_path = drupal_realpath($txt_file->uri);
  $ds->setContentFromFile($txt_path, FALSE);
  $ds->label = $txt_file->filename;
  $ds->mimetype = $txt_file->filemime;

  if (empty($object['TN'])) {
    $ds = $object->constructDatastream('TN', 'M');
    $object->ingestDatastream($ds);
  }
  else {
    $ds = $object['TN'];
  }
  $tn_path = drupal_get_path('module', 'islandora_test_cm') . '/images/file.jpg';
  $ds->setContentFromFile($tn_path, FALSE);
  $ds->label = "Thumbnail image";
  $ds->mimetype = "image/jpeg";
}