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
function xml_form_elements_process_pages_add_resources(array $element, $edit, array &$form_state, array $complete_form = NULL) {
  static $load = TRUE;
  if ($load) {
    module_load_include('inc', 'xml_form_elements', 'includes/Element');
    Element::addRequiredResources();
    Element::addUIWidgets('ui.tabs');
    Element::addUIThemeStyles(array('ui.core.css', 'ui.tabs.css', 'ui.theme.css'));
    Element::addJS('pages.js');
    Element::addCSS('pages.css');
    $load = FALSE;
  }
  return $element;
}

/**
 *
 * @param array $element
 * @param type $edit
 * @param array $form_state
 * @param array $complete_form
 * @return array 
 */
function xml_form_elements_process_pages_add_pagers(array $element, $edit, array &$form_state, array $complete_form = NULL) {
  $element['#id'] = $element['#hash'];
  $children = element_children($element);
  $count = count($children);
  $i = 0;
  foreach ($children as $key) {
    $back = $i != 0;
    $next = $i != ($count - 1);
    xml_form_elements_process_pages_add_pager($element, $element[$key], $back, $next);
    $i++;
  }
  return $element;
}

/**
 *
 * @param array $element
 * @param array $page
 * @param type $back
 * @param type $next 
 */
function xml_form_elements_process_pages_add_pager(array $element, array &$page, $back, $next) {
  if ($back) {
    $last = array_pop(element_children($page));
    if ($page[$last]['#type'] == 'submit') {
      $submit = $page[$last];
      unset($page[$last]);
    }
    array_push($page, array(
      '#type' => 'button',
      '#attributes' => array('class' => 'xml-form-elements-page-back'),
      '#value' => t('Back'),
      '#executes_submit_callback' => FALSE,
    ));
    if (isset($submit)) {
      $page[$last] = $submit;
    }
  }
  if ($next) {
    array_push($page, array(
      '#type' => 'button',
      '#value' => t('Next'),
      '#name' => $page['#hash'],
      '#executes_submit_callback' => FALSE,
      '#ahah' => array(
        'path' => MENU_XML_FORM_ELEMENTS_AHAH . 'validate/' . $page['#hash'],
        'wrapper' => $page['#hash'] . '-wrapper',
        'method' => 'replace',
        'effect' => 'fade',
      ),
    ));
  }
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
  $element['#value'] = NULL;
  $element['#theme'] = THEME_XML_FORM_ELEMENTS_PAGE;
  //$element['#prefix'] = "<div class='clear-block' id='{$element['#id']}-wrapper'>";
  //$element['#suffix'] = "</div>";
  return $element;
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
  $pages = &$vars['element'];
  $vars['content'] = $pages['#children'];
}