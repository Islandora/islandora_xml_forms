<?php

// $Id$

/**
 * @file
 *
 */
class Ahah {

  /**
   * Is this a valid request.
   * 
   * Ensure that a form_build_id is specified.
   * 
   * @return boolean
   */
  public static function validRequest() {
    return isset($_REQUEST['form_build_id']);
  }

  /**
   * Cancel processing an AHAH callback.
   */
  public static function cancelResponse() {
    header("HTTP/1.0 200 OK", false, 200);
    exit();
  }

  /**
   * Ge the form_id, form_build_id, form, and form_state.
   * 
   * @param Post $post
   * @return array
   */
  public static function getFormInfo() {
    if (!self::validRequest()) {
      self::cancelResponse();
    }
    $form_build_id = $_REQUEST['form_build_id'];
    $form_state = array('storage' => NULL, 'submitted' => FALSE, 'post' => $_POST);
    if (!$form = form_get_cache($form_build_id, $form_state)) {
      self::cancelResponse();
    }
    $form_id = array_shift($form['#parameters']);
    return array($form_id, $form_build_id, &$form, &$form_state);
  }

  public static function &rebuildForm($form_id, $form_build_id, array &$form, array &$form_state) {
    $args = $form['#parameters'];
    $form['#post'] = $_POST;
    $form['#redirect'] = FALSE;
    $form['#programmed'] = FALSE;
    $form_state['post'] = $_POST;
    //$form = self::_drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
    //$form['#post'] = $form_state['#post'];
    //$form_state['values'] = array();
    //$form = drupal_process_form($form_id, $form, $form_state);
    $form = self::_drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
    //$form = form_builder($form_id, $form, $form_state);
    //drupal_process_form($form_id, $form, $form_state);
    return $form;
  }

  private static function _drupal_rebuild_form($form_id, &$form_state, $args, $form_build_id = NULL) {
    // Remove the first argument. This is $form_id.when called from
    // drupal_get_form and the original $form_state when called from some AHAH
    // callback. Neither is needed. After that, put in the current state.
    $args[0] = &$form_state;
    // And the form_id.
    array_unshift($args, $form_id);
    $form = call_user_func_array('drupal_retrieve_form', $args);

    if (!isset($form_build_id)) {
      // We need a new build_id for the new version of the form.
      $form_build_id = 'form-' . md5(uniqid(mt_rand(), TRUE));
    }
    $form['#build_id'] = $form_build_id;
    drupal_prepare_form($form_id, $form, $form_state);

    // Now, we cache the form structure so it can be retrieved later for
    // validation. If $form_state['storage'] is populated, we'll also cache
    // it so that it can be used to resume complex multi-step processes.
    form_set_cache($form_build_id, $form, $form_state);

    // Clear out all post data, as we don't want the previous step's
    // data to pollute this one and trigger validate/submit handling,
    // then process the form for rendering.
    $_POST = array();
    $form['#post'] = array();
    drupal_process_form($form_id, $form, $form_state);
    return $form;
  }

  /**
   * Send a response to the AHAH request.
   * 
   * @param array $form 
   */
  public static function respond(array &$form = NULL) {
    //$data = theme('status_messages');
    if (isset($form)) {
      unset($form['#prefix'], $form['#suffix']);
      $data .= drupal_render($form);
    }
    $javascript = drupal_add_js(NULL, NULL, 'header');
    $settings = call_user_func_array('array_merge_recursive', $javascript['setting']);
    unset($settings['ahah']['']);
    drupal_json(array(
      'status' => TRUE,
      'data' => $data,
      'settings' => $settings,
    ));
  }

}