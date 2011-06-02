<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_elements', 'Ahah');
module_load_include('inc', 'xml_form_api', 'Utils');
module_load_include('inc', 'xml_form_api', 'XMLForm');
module_load_include('inc', 'objective_forms', 'Utils');
//module_load_include('inc', 'objective_forms', 'FormStorage');
//module_load_include('inc', 'objective_forms', 'FormElementRegistry');

/**
 * A collection of generic static functions that can be used for AHAH callbacks.
 */
class Element {

  /**
   * Find the element identified by $hash.
   * 
   * @param hash $hash
   * @param array $form_state
   * @return FormElement 
   */
  public static function find($hash, array& $form_state) {
    /*if (!FormStorage::isInitialized()) {
      FormStorage::initialize($form_state);
    }
    return FormElementRegistry::getInstance()->get($hash);*/
  }

  /**
   * AHAH callback to duplicate an existing element.
   * 
   * @param hash $duplicate
   * @param hash $render 
   */
  public static function Duplicate($duplicate, $render = NULL) {
    list($form_id, $form_build_id, $drupal_form, $form_state) = Ahah::getFormInfo();
    // Duplicate Element
    $form = new XMLForm($form_state);
    $original = $form->findElement($duplicate);
    $new_element = $form->duplicateOriginal($duplicate);
    $original->parent[] = $new_element;
    // Rebuild the form, validation and submit callbacks will be called.
    $drupal_form = Ahah::rebuildForm($form_id, $form_build_id, $drupal_form, $form_state);
    // Find the element to render.
    $render = isset($render) ? $render : $new_element->hash; // Render the new element if no render value is supplied.
    $element = find_element($drupal_form, $render);
    // Respond to the request with JSON.
    Ahah::respond($element);
  }

  /**
   * AHAH callback to remove an existing element.
   * 
   * @param hash $remove
   * @param hash $render
   */
  public static function Remove($remove, $render = NULL) {
    list($form_id, $form_build_id, $drupal_form, $form_state) = Ahah::getFormInfo();
    // Remove Element
    $form = new Form($form_state);
    $form->remove($remove);
    // Rebuild the form, validation and submit callbacks will be called.
    $drupal_form = Ahah::rebuildForm($form_id, $form_build_id, $drupal_form, $form_state);
    // Find the element to render.
    $element = Utils::getDrupalFormElement($render, $drupal_form);
    // Respond to the request with JSON.
    Ahah::respond($element);
  }

  /**
   * AHAH callback to validate an existing element.
   * 
   * @param hash $render 
   */
  public static function Validate($render) {
    list($form_id, $form_build_id, $form, $form_state) = Ahah::getFormInfo();
    // Rebuild the form, validation and submit callbacks will be called.
    $form = Ahah::rebuildForm($form_id, $form_build_id, $form, $form_state);
    // Find the element to render.
    $element = Utils::getDrupalFormElement($render, $form);
    // Respond to the request with JSON.
    Ahah::respond($element);
  }

  /**
   * AHAH callback to rebuild an existing element.
   * 
   * @param hash $render 
   */
  public static function Rebuild($render) {
    list($form_id, $form_build_id, $form, $form_state) = Ahah::getFormInfo();
    // Rebuild the form, validation and submit callbacks will be called.
    $form = Ahah::rebuildForm($form_id, $form_build_id, $form, $form_state);
    // Find the element to render.
    $element = Utils::getDrupalFormElement($render, $form);
    // Respond to the request with JSON.
    Ahah::respond($element);
  }

}