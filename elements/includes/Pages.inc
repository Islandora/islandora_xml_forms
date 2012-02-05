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
  foreach (element_children($element) as $key) {
    if ($element[$key]['#type'] != 'page') {
      drupal_set_message(t('A form element of type \'%type\' was given where a \'page\' was expected. Only \'page\'s can be children of a \'pages\' form element.', array('%type' => $element[$key]['#type'])), 'error');
    }
  }
  return $element;
}

/**
 *
 * @param array $element
 * @param type $edit
 * @param array $form_state
 * @param array $complete_form
 * @return string 
 */
function xml_form_elements_process_page_initialize(array $element, $edit, array &$form_state, array $complete_form = NULL) {
  $element['#id'] = $element['#hash'];
  $element['#required'] = FALSE; // Can't require a page.
  $element['#type'] = 'markup'; // Prevent name collision with 'page' theme.
  $element['#theme'] = THEME_XML_FORM_ELEMENTS_PAGE;
  return $element;
}

function foo($element, $content) {
  return theme('form_element', $element, $content);
}

/**
 *
 * @param type $element
 * @return type 
 */
function theme_xml_form_elements_page(array $element) {
  $content = '';
  foreach (element_children($element) as $key) {
    $content .= drupal_render($element[$key]);
  }
  /**
   * Note that this is pulled mostly from theme_form_element.
   * The difference being that the description is included before the content.
   */
  $t = get_t();
  $output = '<div class="form-item"';
  if (!empty($element['#id'])) {
    $output .= ' id="' . $element['#id'] . '-wrapper"';
  }
  $output .= ">\n";
  if (!empty($element['#description'])) {
    $output .= ' <div class="description">' . $element['#description'] . "</div>\n";
  }
  $output .= " $content\n";
  $output .= "</div>\n";
  return $output;
}

/**
 * Implements template_preprocess_THEME().
 *  
 * @param array $vars
 *   The properties availible to this theme's template.
 */
function template_preprocess_pages(array &$vars) {
  //error_log(print_r($vars['element'], TRUE));
  $pages = &$vars['element'];
  $vars['content'] = $pages['#children'];
}