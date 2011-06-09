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
    drupal_process_form($form_id, $form, $form_state);
    $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
    return $form;
  }

  /**
   * Send a response to the AHAH request.
   * 
   * @param array $form 
   */
  public static function respond(array &$form = NULL) {
    $data = theme('status_messages');
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