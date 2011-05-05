<?php

// $Id$

/**
 * @file
 *
 */
class Element {

  /**
   *
   * @param hash $hash 
   */
  public static function Duplicate($hash) {
    if (!isset($_REQUEST['form_build_id']))
      return;
    $form_build_id = $_REQUEST['form_build_id'];
    list($form, $form_state) = get_form_and_form_state($form_build_id);
    // Apply changes.
    $form = new Form($form_state);
    $form->definition->elements->duplicateElement($hash);
    $drupal_form = $form->generate();
    cache_form_and_form_state($form_build_id, $drupal_form, $form_state);
    $element = $form->definition->elements->findElement($hash);
    // Render the changed bit.
    $drupal_form = $element->toDrupalForm();
    $drupal_form = form_builder($drupal_form);
    ahah_return_html_via_json($drupal_form);
  }

  /**
   *
   * @param hash $hash 
   */
  public static function Remove($hash) {
    if (!isset($_REQUEST['form_build_id']))
      return;
    $form_build_id = $_REQUEST['form_build_id'];
    list($form, $form_state) = get_form_and_form_state($form_build_id);
    // Apply changes.
    $form = new Form($form_state);
    $form->definition->elements->removeElement($hash);
    $drupal_form = $form->generate();
    cache_form_and_form_state($form_build_id, $drupal_form, $form_state);
    $element = $form->definition->elements->findElement($hash);
    // Render the changed bit.
    $drupal_form = $element->toDrupalForm();
    $drupal_form = form_builder($drupal_form);
    ahah_return_html_via_json($drupal_form);
  }

}