<?php



/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_elements', 'TabPanel');

class Tabs {

  /**
   *
   * @param array $element
   * @param type $edit
   * @param array $form_state
   * @param array $complete_form 
   */
  public static function Process(array $element, $edit, array &$form_state, array $complete_form = NULL) {
    static $add_files = TRUE;
    if ($add_files) {
      $path = drupal_get_path('module', 'xml_form_elements');
      jquery_ui_add('ui.tabs');
      drupal_add_js("$path/js/XMLFormElements.js");
      drupal_add_js("$path/js/Tabs.js");
      drupal_add_css("$path/theme/ui.core.css");
      drupal_add_css("$path/theme/ui.tabs.css");
      drupal_add_css("$path/theme/ui.theme.css");
      drupal_add_css("$path/stylesheets/tabs.css");
      $add_files = FALSE;
    }
    $element['#prefix'] = "<div class='clear-block' id='{$element['#hash']}'>";
    $element['#suffix'] = '</div>';
    return $element;
  }

  /**
   *
   * @param array $children 
   */
  public static function FilterChildren(array $child) {
    $ret = ($child['#type'] == 'tabpanel') ? TRUE : FALSE;
    return $ret;
  }

}

/**
 * Preprocess tabs theme hook.
 * 
 * @param array $vars 
 */
function xml_form_elements_preprocess_tabs_content(&$vars) {
  $tabs = $vars['tabs'];
  // Header Info
  $vars['collapsible'] = $tabs['#collapsible'] == TRUE;
  $vars['collapsed'] = $tabs['#collapsed'] == TRUE;
  // Get Tabpanels
  $keys = element_children($tabs);
  $children = array();
  foreach ($keys as $key) {
    $children[$key] = &$tabs[$key];
  }
  $children = array_filter($children, array('Tabs', 'FilterChildren'));
  // First element has different icons, and title.
  $first = array_shift($children);
  $vars['title'] = isset($first['#title']) ? $first['#title'] : $tabs['#title'];
  $vars['link'] = $first['#hash'];
  // Remaining tabpanels.
  $count = 2;
  foreach ($children as $child) {
    $title = isset($child['#title']) ? $child['#title'] : $count;
    $delete = isset($child[TabPanel::DeleteButton]) ? $child[TabPanel::DeleteButton]['#id'] : FALSE;
    $vars['tabpanels'][] = array($title, $child['#hash'], $delete);
    $count++;
  }
  // Content Info
  if(isset($tabs['#children'])) {
    $vars['content'] = $tabs['#children'];
  }
  else {
    $parents = $tabs['#array_parents'];
    $name = array_pop($parents);
    $vars['content'] = '';
    drupal_set_message("Attempted to create a tabs: '$name', without defining any child tabpanels.", 'error');
  }
}

/**
 * Theme Hook for tags elements.
 */
function xml_form_elements_theme_tabs($element) {
  $content = theme('tabs_content', $element);
  return theme('form_element', $element, $content);
}