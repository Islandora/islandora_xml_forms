<?php

/**
 * @file
 *
 * Functions for processing/theming the 'pages' form element. 
 */

/**
 * The default #process callback used to initalize the pages element. If this function 
 * is removed from the #process callback list this element will not function correctly.
 * 
 * * @param array $element
 *   The element to be processed.
 * @param mixed $edit
 *   The submitted value for this element, or NULL.
 * @param array $form_state
 *   The drupal form state.
 * @param array $complete_form
 *   The drupal form.
 */
function xml_form_elements_process_pages_initialize(array $element, $edit, array &$form_state, array $complete_form = NULL) {
  static $add_files = TRUE;
  if ($add_files) {
    jquery_ui_add('ui.tabs');
    drupal_add_js(PATH_XML_FORM_ELEMENTS_JS . 'XMLFormElements.js');
    drupal_add_js(PATH_XML_FORM_ELEMENTS_JS . 'pages.js');
    drupal_add_css(PATH_XML_FORM_ELEMENTS_JQUERY_THEME . 'ui.core.css');
    drupal_add_css(PATH_XML_FORM_ELEMENTS_JQUERY_THEME . 'ui.tabs.css');
    drupal_add_css(PATH_XML_FORM_ELEMENTS_JQUERY_THEME . 'ui.theme.css');
    drupal_add_css(PATH_XML_FORM_ELEMENTS_CSS . 'pages.css', 'theme');
    $add_files = FALSE;
  }
  return $element;
}

/**
 * Implements template_preprocess_THEME().
 *  
 * @param array $vars
 *   The properties availible to this theme's template.
 */
function template_preprocess_pages(array &$vars) {
  
}