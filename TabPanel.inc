<?php

// $Id$

/* @file
 *
 *  Allows forms to dynamically add new tabs to a tabpanel in a form.
 */
module_load_include('inc', 'xml_form_elements', 'Utils');
module_load_include('inc', 'xml_form_api', 'Form');

class TabPanel {

  public static function Process(array $element, $edit, array &$form_state, array $complete_form = NULL) {
    $tabpanel = self::findTabPanel($element['#hash'], $form_state);
    $add = self::createAddButton($tabpanel);
    $element['add-tabpanel'] = $add->toDrupalForm();
    return $element;
  }

  private static function findTabPanel($hash, array &$form_state) {
    $form = new Form($form_state);
    $tabpanel = $form->findElement($hash);
    return $tabpanel;
  }

  private static function createAddButton(FormElement $tabpanel) {
    $tabs = $tabpanel->parent;
    $add = new FormElement();
    $add->type = 'submit';
    $add->weight = 4;
    $add->size = 30;
    $add->value = 'Add';
    $add->prefix = '<div class="ui-tabpanel-add-button">';
    $add->suffix = '</div>';
    $add->ahah = array(
      'path' => 'xml_form_elements/ahah/tabpanel/duplicate/' . $tabpanel->hash,
      'wrapper' => $tabs->hash, // The parents wrapper is set to the parents hash.
      'method' => 'replace',
      'effect' => 'fade'
    );
    $tabpanel['add-tabpanel'] = $add; // Add to the definition.
    return $add;
  }

  public static function Theme($element) {
    return "<div id='{$element['#hash']}'>{$element['#children']}</div>";
  }

  public static function Duplicate($hash) {
    if (!isset($_REQUEST['form_build_id']))
      return;
    $form_build_id = $_REQUEST['form_build_id'];
    list($drupal_form, $form_state) = get_form_and_form_state($form_build_id);
    $form_id = $drupal_form['#parameters'][0];
    // Apply changes.
    $form = new Form($form_state);
    $form->duplicateElement($hash);
    $drupal_form = $form->generate();
    // Store Changes
    cache_form_and_form_state($form_build_id, $drupal_form, $form_state);
    // Render the changed bit.
    $drupal_form = form_builder($form_id, $drupal_form, $form_state);
    $tabs = self::getTabs($form, $drupal_form, $hash);
    ahah_return_html_via_json($tabs);
  }
  
  private static function getTabs(Form $form, array &$drupal_form, $hash) {
    $tabpanel = $form->findElement($hash);
    $tabs_hash = $tabpanel->parent->hash;
    $iterator = new RecursiveIteratorIterator(new RecursiveArrayIterator($drupal_form), RecursiveIteratorIterator::SELF_FIRST);
    foreach ($iterator as $element) {
      if ($element['#hash'] == $tabs_hash) {
        return $element;
      }
    }
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

  /*
    private function themeTabHeader(&$form, $name, $title, $collapsible, $collapsed) {
    $output = '<ul>';
    $output .= '<li>';
    $output .= "<a href='#$name-0'>";
    $output .= "<span style='float:left; vertical-align: middle; text-align: center;'>$title</span>";
    if ($collapsible) {
    $icon = ($collapsed) ? 'ui-icon-circle-triangle-e' : 'ui-icon-circle-triangle-s';
    $output .= "<span class='expand-tabpanel-icon ui-icon $icon' style='float: right; vertical-align: middle; margin-left:0.2em;'></span>";
    }
    $output .= '</a>';
    $output .= '</li>';
    foreach ($form as $key => $panel) {
    if (is_numeric($key) && $key > 0) {
    $output .= '<li>';
    $output .= "<a href='#$name-$key'>$key</a>";
    $output .= "<span id='remove-$name-$key' class='ui-icon ui-icon-close' style='float: left; margin: 0.4em 0.2em 0 0; cursor: pointer;'>";
    $output .= $panel['remove-tab']['#id'];
    $output .= '</span>';
    $output .= '</li>';
    }
    }
    $output .= '</ul>';
    return $output;
    }

    /*
    public function theme(&$element) {
    global $base_url;
    $path = drupal_get_path('module', 'islandora_form_builder_elements');
    drupal_add_js($path . '/js/jquery-ui-1.8.7.custom.min.js');
    drupal_add_js($path . '/js/tabpanel_ahah.js');
    drupal_add_css($path . '/css/custom-theme/jquery-ui-1.8.11.custom.css');
    $classes = 'action-load-tabpanel';
    if ($element['#collapsible']) {
    $classes .= ' property-collapsible-tabpanel';
    }
    if ($element['#collapsed']) {
    $classes .= ' property-collapsed-tabpanel';
    }
    $id = t(implode('-', $element['#array_parents']));
    return "<div id='$id' class='$classes'>{$element['#children']}</div>";
    }

    public function themeEachTab(&$form) {
    $name = $form['#name'];
    $title = $form['#title'];
    $collapsible = $form['#collapsible'];
    $collapsed = $form['#collapsed'];
    $output = $this->themeTabHeader($form, $name, $title, $collapsible, $collapsed);
    $output .= $this->themeTabContent($form, $name, $title);
    return $output;
    }

    private function themeTabHeader(&$form, $name, $title, $collapsible, $collapsed) {
    $output = '<ul>';
    $output .= '<li>';
    $output .= "<a href='#$name-0'>";
    $output .= "<span style='float:left; vertical-align: middle; text-align: center;'>$title</span>";
    if ($collapsible) {
    $icon = ($collapsed) ? 'ui-icon-circle-triangle-e' : 'ui-icon-circle-triangle-s';
    $output .= "<span class='expand-tabpanel-icon ui-icon $icon' style='float: right; vertical-align: middle; margin-left:0.2em;'></span>";
    }
    $output .= '</a>';
    $output .= '</li>';
    foreach ($form as $key => $panel) {
    if (is_numeric($key) && $key > 0) {
    $output .= '<li>';
    $output .= "<a href='#$name-$key'>$key</a>";
    $output .= "<span id='remove-$name-$key' class='ui-icon ui-icon-close' style='float: left; margin: 0.4em 0.2em 0 0; cursor: pointer;'>";
    $output .= $panel['remove-tab']['#id'];
    $output .= '</span>';
    $output .= '</li>';
    }
    }
    $output .= '</ul>';
    return $output;
    }

    private function themeTabContent(&$form, $name, $title) {
    $output = '';
    foreach ($form as $key => $panel) {
    if (is_numeric($key)) {
    $output .= "<div id='$name-$key'>";
    $output .= drupal_render($form[$key]);
    $output .= '</div>';
    }
    }
    return $output;
    }

    public function process(&$form, &$form_state) {
    $form['#tree'] = TRUE;
    $wrapper = $form['#id'] . '-wrapper';
    $this->wrap($form, $wrapper);
    $this->addThemeInfoToContent($form['content'], $form);
    $path = $form['#parents'];
    array_walk($path, array($this, 'stringifyPath'));
    $path = implode('', $path);
    $this->createAddTabButton($form, $path, $wrapper);
    $this->createRemoveTabButtons($form['content'], $path, $wrapper);
    return $form;
    }

    public static function Process(array &$form, array &$form_state) {
    $tabpanel = self::findTabPanel($form['#hash']);
    $add = self::createAddButton($tabpanel);
    $form['add-tabpanel'] = $add->toDrupalForm();
    return $form;
    }

    private static function findTabPanel($hash) {
    $form = new Form($form_state);
    $tabpanel = $form->findElement($hash);
    return $tabpanel;
    }

    private static function createAddButton(FormElement $tabpanel) {
    $tabs = $tabpanel->parent;
    $add = new FormElement();
    $add->type = 'submit';
    $add->weight = 4;
    $add->size = 30;
    $add->value = 'Add';
    $add->ahah = array(
    'path' => 'xml_form_elements/ahah/tabpanel/duplicate/' . $tabpanel->hash,
    'wrapper' => $tabs->hash, // The parents wrapper is set to the parents hash.
    'method' => 'replace',
    'effect' => 'fade'
    );
    $tabpanel['add-tabpanel'] = $add; // Add to the definition.
    return $add;
    }

    private function stringifyPath(&$value, $key) {
    if (is_numeric($value)) {
    $value = "[$value]";
    }
    else {
    $value = "['$value']";
    }
    }

    private function wrap(&$form, $wrapper) {
    $form['#prefix'] = '<div class="clear-block" id="' . $wrapper . '">';
    $form['#suffix'] = '</div>';
    }

    private function addThemeInfoToContent(&$content, $form) {
    $content['#title'] = $form['#title'];
    $content['#theme'] = 'tabpanel_items';
    $content['#name'] = $form['#id'];
    $content['#collapsible'] = $form['#collapsible'];
    $content['#collapsed'] = $form['#collapsed'];
    }

    private function createAddTabButton(&$form, $path, $wrapper) {
    $content = &$form['content'];
    $content[0]['add-tab'] = array(
    '#type' => 'submit',
    '#weight' => 4,
    '#size' => 30,
    '#attributes' => array('style' => 'display:none'), // Hidden, triggered via javascript.
    '#value' => "add $path",
    '#ahah' => array(
    'path' => 'formbuilder/elements/ahah/tabpanel',
    'wrapper' => $wrapper,
    'method' => 'replace',
    'effect' => 'fade'
    )
    );
    // Actual buttons
    $id = $form['#id'] . '-content-0-add-tab';
    foreach ($content as $key => &$panel) {
    if (is_numeric($key)) {
    $panel[] = array(
    '#type' => 'button',
    '#button_type' => 'button',
    '#attributes' => array('onclick' => "return formbuilder.tabpanel.addTab('$id');", 'class' => 'action-add-tab'),
    '#weight' => 4,
    '#size' => 30,
    '#value' => "Add",
    );
    }
    }
    }

    private function createRemoveTabButtons(&$content, $path, $wrapper) {
    foreach ($content as $key => &$panel) {
    if (is_numeric($key) && $key > 0) {
    $panel['remove-tab'] = array(
    '#type' => 'submit',
    '#weight' => 4,
    '#size' => 30,
    '#attributes' => array('style' => 'display:none'), // Hidden, triggered via javascript.
    '#value' => "remove $key $path", // Unique
    '#ahah' => array(
    'path' => 'formbuilder/elements/ahah/tabpanel',
    'wrapper' => $wrapper,
    'method' => 'replace',
    'effect' => 'fade'
    ),
    );
    }
    }
    }

    public function ahah($form_build_id) {
    list($form, $form_state) = $this->getFormAndState($form_build_id);
    $path = $this->performAction($form, $form_state);
    $this->saveChanges($form_build_id, $form, $form_state);
    $form = $this->processForm($form, $form_state);
    $tabpanel = $this->findTabPanel($form, $path);
    $this->sendJSON($tabpanel);
    }

    private function getFormAndState($form_build_id) {
    $form_state = array('storage' => NULL, 'submitted' => FALSE, 'post' => $_POST);
    if (!$form = form_get_cache($form_build_id, $form_state))
    exit();
    return array(&$form, &$form_state);
    }

    private function performAction(&$form, &$form_state) {
    $params = explode(' ', $form_state['post']['op']);
    $action = array_shift($params);
    switch ($action) {
    case 'add':
    $path = array_shift($params);
    $tabpanel = &$this->findTabPanel($form, $path);
    $this->addPanel($tabpanel);
    break;
    case 'remove':
    list($index, $path) = $params;
    $tabpanel = &$this->findTabPanel($form, $path);
    $post = &$this->findPost($form, $path);
    $this->removePanel($index, $tabpanel, $post);
    break;
    }
    return $path; // $path of the modified tabpanel.
    }

    private function addPanel(&$tabpanel) {
    $panel = $tabpanel['content'][0]; // Copy the first panel.
    $tabpanel['content'][] = &$panel; // Add it to the tabpanel
    $this->pruneExtraPanels($panel); // Remove extra tabpanels.
    $this->removeCopiedHashes($panel);
    $this->removeDefaultValues($panel);
    }

    private function pruneExtraPanels(&$panel) {
    foreach ($panel as $key => &$child) {
    if (is_numeric($key) && isset($child['content'])) {
    $content = &$child['content'];
    $this->removeExtraPanels($content);
    $this->pruneExtraPanels($content[0]);
    }
    }
    }

    private function removeExtraPanels(&$content) {
    foreach ($content as $key => &$panel) {
    if (is_numeric($key) && $key > 0) {
    unset($content[$key]);
    }
    }
    }

    private function removeCopiedHashes(&$panel) {
    $this->removeHash($panel);
    foreach ($panel as $key => &$child) {
    if (is_numeric($key)) {
    $this->removeHash($child);
    if (isset($child['content'])) {
    $this->removeCopiedHashes($child['content'][0]);
    }
    }
    }
    }

    private function removeHash(&$form) {
    if (isset($form['#form_builder'])) {
    if (isset($form['#form_builder']['hash'])) {
    unset($form['#form_builder']['hash']);
    }
    }
    }

    private function removeDefaultValues(&$panel) {
    $this->removeHash($panel);
    foreach ($panel as $key => &$child) {
    if (is_numeric($key)) {
    $this->removeDefaultValue($child);
    if (isset($child['content'])) {
    $this->removeDefaultValues($child['content'][0]);
    }
    }
    }
    }

    private function removeDefaultValue(&$form) {
    if (isset($form['#default_value'])) {
    unset($form['#default_value']);
    }
    }

    private function removePanel($index, &$tabpanel, &$post) {
    array_splice($tabpanel['content'], $index, 1);
    array_splice($post['content'], $index, 1);
    }

    private function saveChanges(&$form_build_id, &$form, &$form_state) {
    $form['#post'] = $_POST;
    $form['#programmed'] = FALSE;
    form_set_cache($form_build_id, $form, $form_state);
    }

    private function processForm(&$form, &$form_state) {
    $form_id = $form['#parameters'][0];
    return form_builder($form_id, $form, $form_state);
    }

    private function &findTabPanel(&$form, &$path) {
    $tabpanel = '';
    $command = "\$tabpanel = &\$form$path;";
    $result = eval($command);
    return $tabpanel;
    }

    private function &findPost(&$form, &$path) {
    $post = '';
    $command = "\$post = &\$_POST$path;";
    $result = eval($command);
    return $post;
    }

    private function sendJSON(&$tabpanel) {
    unset($tabpanel['#prefix'], $tabpanel['#suffix']);
    $javascript = drupal_add_js(NULL, NULL, 'header');
    $settings = call_user_func_array('array_merge_recursive', $javascript['setting']);
    unset($settings['ahah']['']);
    drupal_json(array(
    'status' => TRUE,
    'data' => theme('status_messages') . drupal_render($tabpanel),
    'settings' => $settings,
    ));
    } */
}

/**
 * Theme hook for a tabpanel
 */
function theme_tabpanel($element) {
  return TabPanel::Theme($element);
}

