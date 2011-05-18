<?php

// $Id$

/**
 * @file
 *
 */

/**
 *
 * @param array $form_state
 * @param string $form_name 
 * 
 * @return array
 */
function xml_form_builder_edit($form_name) {
  module_load_include('inc', 'xml_form_builder', 'FormBuilder');
  $path = drupal_get_path('module', 'xml_form_builder');
  drupal_add_css("$path/css/Clear.css");
  drupal_add_css("$path/css/EditForm.css");
  drupal_add_css("$path/js/ext-4.0.0/resources/css/ext-all.css");
  // Ext JS 4.0
  drupal_add_js("$path/js/ext-4.0.0/ext-all-debug-w-comments.js");
  // Main Application
  drupal_add_js("$path/js/Formbuilder.js");
  // Custom Types
  drupal_add_js("$path/js/FormGrid.js");
  drupal_add_js("$path/js/Tooltips.js");
  // Custom Models
  drupal_add_js("$path/js/ElementModel.js");
  // Actual Panels that make up the editor.
  drupal_add_js("$path/js/MainPanel.js");
  drupal_add_js("$path/js/DisplayPanel.js");
  drupal_add_js("$path/js/TreePanel.js");
  drupal_add_js("$path/js/ElementForm.js");
  drupal_add_js("$path/js/PropertiesForm.js");
  drupal_add_js("$path/js/PreviewPanel.js");
  xml_form_builder_create_element_type_store();
  xml_form_builder_create_element_store($form_name);
  return '<div id="xml-form-builder-editor"></div>'; //theme('edit_form_page', $output);
}

/**
 * Create store to be used in Element Type Combo Box.
 */
function xml_form_builder_create_element_type_store() {
  $types = Formbuilder::GetElementTypes();
  $data = array();
  foreach ($types as $type) {
    $data[] = array('display' => $type, 'value' => $type);
  }
  $data = json_encode($data);
  $js = <<<EOT
  Ext.formbuilder.elementTypeStore = Ext.data.Store({
    storeId: 'ElementTypes',
    fields: ['display', 'value'],
    proxy: {
      type: 'memory',
      reader: {
        type: 'json'
      }
    },
    data: $data
  });
EOT;
  drupal_add_js($js, 'inline');
}

/**
 * Create a element store.
 * 
 * @param string $form_name 
 */
function xml_form_builder_create_element_store($form_name) {
  $data = Formbuilder::TransformElementsToJSON($form_name);
  $js = <<<EOT
  Ext.formbuilder.elementStore = Ext.create('Ext.data.TreeStore', {
    storeId: 'ElementStore',
    model: 'Element',
    proxy: {
      type: 'memory',
      data: $data,
      reader: {
        type: 'json'
      }
    },
    root: {
      text: 'Elements',
      id: 'elements',
      expanded: true
    }
 });
EOT;
  drupal_add_js($js, 'inline');
}

/**
 *
 * @param array $form
 * @param array $form_state 
 */
function create_element(array &$form_state) {
  module_load_include('inc', 'xml_form_api', 'Form');
  $form = new Form($form_state);
  if (!$form->isInitialized()) {
    module_load_include('inc', 'xml_form_builder', 'FormBuilder');
    $path = drupal_get_path('module', 'xml_form_builder');
    $doc = new DOMDocument();
    $doc->load("$path/xml/Element.xml");
    $form_definition = FormDefinition::createFromXMLDefiniton($doc->saveXML());
    $document = new Document($form_definition->properties->document);
    $form->initialize($form_definition, $document);
    // Custom changes...
    $types = FormBuilder::GetElementTypes();
    $type = $form->elements->root['element']['type'];
    $advanced = $form->elements->root['element']['advanced'];
    $ahah = $type['#ahah'];
    $ahah['path'] = "xml/form/elements/ahah/rebuild/{$advanced->hash}";
    $type['#ahah'] = $ahah;
    $type['#options'] = $types;
    $type['#default_value'] = array_shift($types);
  }
  return $form->toDrupalForm();
}

/**
 *
 * @param array $form
 * @param array $form_state 
 */
function xml_form_builder_form_create_element_alter(array &$form, array &$form_state) {
  module_load_include('inc', 'xml_form_builder', 'FormBuilder');
  //$type = $form[Form::ROOT]['element']['type']['#default_value'];
  // TODO call some hook function for modifying the advanced form controls.
}

/**
 * Preprocess tabs theme hook.
 * 
 * @param array $vars 
 */
function xml_form_elements_preprocess_edit_form_page(array &$vars) {
  $vars['element_form'] = $vars[0];
}

/**
 * 
 */
function xml_form_builder_ahah_controls() {
  module_load_include('inc', 'xml_form_elements', 'Ahah');
  list($form_id, $form_build_id, $form, $form_state) = Ahah::getFormInfo();
  $form = Ahah::rebuildForm($form_id, $form_build_id, $form, $form_state);
  Ahah::respond($form[FORM::ROOT]['element']['advanced']);
}