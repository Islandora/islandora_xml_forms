<?php



/**
 * @file
 *
 * A collection of helper functions.
 */

/**
 *
 * @param type $form_build_id
 */
function get_form_info($form_build_id) {
  if (!isset($_REQUEST['form_build_id'])) {
    header("HTTP/1.0 200 OK", FALSE, 200);
    exit();
  }
  $form_build_id = $_REQUEST['form_build_id'];
  $form_state = array('storage' => NULL, 'submitted' => FALSE, 'post' => $_POST);
  if (!$form = form_get_cache($form_build_id, $form_state)) {
    header("HTTP/1.0 200 OK", FALSE, 200);
    exit();
  }
  $form_id = $form['#parameters'][0];
  return array($form_id, $form_build_id, &$form, &$form_state);
}

/**
 *
 * @param type $form_build_id
 * @return array
 */
function get_form_and_form_state($form_build_id) {
  $form_state = array('storage' => NULL, 'submitted' => FALSE, 'post' => $_POST);
  if (!$form = form_get_cache($form_build_id, $form_state)) {
    header("HTTP/1.0 200 OK", FALSE, 200);
    exit();
  }
  return array(&$form, &$form_state);
}

/**
 *
 * @param type $form_build_id
 * @param array $form
 * @param array $form_state
 */
function cache_form_and_form_state($form_build_id, array &$form, array &$form_state) {
  $form['#post'] = $_POST;
  $form['#programmed'] = FALSE;
  form_set_cache($form_build_id, $form, $form_state);
}

/**
 *
 * @param array $element
 */
function ahah_return_html_via_json(array &$element) {
  unset($element['#prefix'], $element['#suffix']);
  $javascript = drupal_add_js(NULL, NULL, 'header');
  $settings = call_user_func_array('array_merge_recursive', $javascript['setting']);
  unset($settings['ahah']['']);
  drupal_json(array(
    'status' => TRUE,
    'data' => theme('status_messages') . drupal_render($element),
    'settings' => $settings,
  ));
}