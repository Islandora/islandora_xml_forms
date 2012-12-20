<?php

/**
 * @file
 *
 *
 */
module_load_include('inc', 'xml_form_api', 'Utils');
module_load_include('inc', 'xml_form_api', 'XMLForm');
module_load_include('inc', 'objective_forms', 'Utils');
module_load_include('inc', 'objective_forms', 'FormPopulator');

/**
 * A collection of generic static functions that help in constructing elements.
 */
class Element {

  /**
   * Loads any javascript files which are needed for elements which use AHAH.
   */
  public static function addRequiredResources() {
    static $load = TRUE;
    if ($load) {
      drupal_add_js('misc/jquery.form.js'); // Required
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
    // TODO $files needs to be a string or multiple calls to drupal_add_library need to be made.
    drupal_add_library('system', $files);
  }

  /**
   * Includes the given jQuery UI theme files.
   *
   * @param array $files
   *   An list of files to be included from the jquery theme. Or a single file name.
   */
  public static function addUIThemeStyles($files = array()) {
    self::addFiles('drupal_add_css', PATH_XML_FORM_ELEMENTS_JQUERY_THEME, $files, array(
      'group' => CSS_THEME,
    ));
  }

  /**
   * Includes the given javascript files from this module.
   *
   * @param type $files
   */
  public static function addJS($files = array()) {
    self::addFiles('drupal_add_js', PATH_XML_FORM_ELEMENTS_JS, $files);
  }

  /**
   * Includes the given css files from this module.
   *
   * @param type $files
   */
  public static function addCSS($files = array()) {
    self::addFiles('drupal_add_css', PATH_XML_FORM_ELEMENTS_CSS, $files, array(
      'group' => CSS_THEME,
    ));
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
    // Convert string to array.
    $files = (array)$files;
    foreach ($files as $file) {
      $function($path . $file, $additional_argument);
    }
  }
}
