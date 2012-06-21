<?php

/**
 * @file
 *
 *
 */
module_load_include('inc', 'php_lib', 'Ahah');
module_load_include('inc', 'xml_form_api', 'Utils');
module_load_include('inc', 'xml_form_api', 'XMLForm');
module_load_include('inc', 'objective_forms', 'Utils');
module_load_include('inc', 'objective_forms', 'FormPopulator');

/**
 * A collection of generic static functions that can be used for AHAH callbacks.
 */
class Element {

  /**
   * Loads any javascript files which are needed for elements which use AHAH.
   */
  public static function addRequiredResources() {
    static $load = TRUE;
    if ($load) {
      drupal_add_js('misc/jquery.form.js'); // Required
      drupal_add_js('misc/ahah.js'); // Required
      drupal_add_js(PATH_XML_FORM_ELEMENTS_JS . 'ahah.js');
      $load = FALSE;
    }
  }

  /**
   * Includes the given jQuery UI widgets.
   * 
   * @param array $files 
   *   An list of files to be included from the jquery theme. Or a single file name.
   */
  public static function addUIWidgets($files = array()) {
    jquery_ui_add($files);
  }

  /**
   * Includes the given jQuery UI theme files.
   * 
   * @param array $files 
   *   An list of files to be included from the jquery theme. Or a single file name.
   */
  public static function addUIThemeStyles($files = array()) {
    self::addFiles('drupal_add_css', PATH_XML_FORM_ELEMENTS_JQUERY_THEME, $files, 'theme');
  }

  /**
   * Includes the given javascript files from this module.
   * 
   * @param type $files 
   */
  public static function addJS($files = array()) {
    self::addFiles('drupal_add_js', PATH_XML_FORM_ELEMENTS_JS, $files, 'module');
  }

  /**
   * Includes the given css files from this module.
   * 
   * @param type $files 
   */
  public static function addCSS($files = array()) {
    self::addFiles('drupal_add_css', PATH_XML_FORM_ELEMENTS_CSS, $files, 'theme');
  }

  /**
   * Adds files using the given $method, $path and $files.
   *
   * @param string $function
   *   The function to call to add the given files.
   * @param string $path
   *   The path where the files are located.
   * @param array $files 
   *   The file(s) to be added.
   */
  private static function addFiles($function, $path, $files = array(), $additional_argument = NULL) {
    $files = is_array($files) ? $files : array($files); // Convert string to array if needed.
    foreach ($files as $file) {
      $function($path . $file, $additional_argument);
    }
  }

  /**
   * AHAH callback to duplicate an existing element.
   *
   * @param hash $duplicate
   * @param hash $render
   */
  public static function duplicate($duplicate, $render = NULL) {
    list($form_id, $form_build_id, $drupal_form, $form_state) = Ahah::getFormInfo();
    $drupal_form = Ahah::processForm($form_id, $drupal_form, $form_state);
    // Duplicate Element
    $form = new XMLForm($form_state);
    $original = $form->findElement($duplicate);
    $new_element = $form->duplicateOriginal($duplicate);
    $new_element->eachDecendant('Element::removeDefault');
    $original->parent->adopt($new_element);
    $drupal_form = Ahah::rebuildForm($form_id, $form_build_id, $drupal_form, $form_state);
    // Find the element to render.
    $render = isset($render) ? $render : $new_element->hash; // Render the new element if no render value is supplied.
    $element = find_element($drupal_form, $render);
    // Respond to the request with JSON.
    Ahah::respond($element);
  }

  /**
    * Used by the duplicate function to clear default values of descendants
    * 
    * @param type $child 
    */
  public static function removeDefault($child) {
     $child->default_value = '';
  }

  /**
   * AHAH callback to remove an existing element.
   *
   * @param hash $remove
   * @param hash $render
   */
  public static function remove($remove, $render) {
    list($form_id, $form_build_id, $form, $form_state) = Ahah::getFormInfo();
    // Remove Element
    $xml_form = new XMLForm($form_state);
    $xml_form->remove($remove);
    // Rebuild the form, validation and submit callbacks will be called.
    $form = Ahah::processForm($form_id, $form, $form_state);
    $form = Ahah::rebuildForm($form_id, $form_build_id, $form, $form_state);
    // Find the element to render.
    $element = find_element($form, $render);
    // Respond to the request with JSON.
    Ahah::respond($element);
  }

  /**
   * AHAH callback to validate an existing element.
   *
   * @param hash $render
   */
  public static function validate($render) {
    list($form_id, $form_build_id, $form, $form_state) = Ahah::getFormInfo();
    // Rebuild the form, validation and submit callbacks will be called.
    $form = Ahah::processForm($form_id, $form, $form_state);
    _form_validate(find_element($form, $render), $form_state);
    $form = Ahah::rebuildForm($form_id, $form_build_id, $form, $form_state);
    // Find the element to render.
    $element = find_element($form, $render);
    // Respond to the request with JSON.
    Ahah::respond($element);
  }

  /**
   * AHAH callback to rebuild an existing element.
   *
   * @param hash $render
   */
  public static function rebuild($render) {
    list($form_id, $form_build_id, $form, $form_state) = Ahah::getFormInfo();
    // Rebuild the form, validation and submit callbacks will be called.
    $form = Ahah::processForm($form_id, $form, $form_state);
    $form = Ahah::rebuildForm($form_id, $form_build_id, $form, $form_state);
    // Find the element to render.
    $element = find_element($drupal_form, $render);
    // Respond to the request with JSON.
    Ahah::respond($element);
  }

}
