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
  drupal_add_css("$path/js/ext-4.0.1/resources/css/ext-all.css");
  // Ext JS 4.0
  drupal_add_js("$path/js/ext-4.0.1/ext-all-debug-w-comments.js");
  // Main Application
  drupal_add_js("$path/js/Formbuilder.js");
  // Custom Types
  drupal_add_js("$path/js/FormGrid.js");
  drupal_add_js("$path/js/Tooltips.js");
  // Custom Models
  drupal_add_js("$path/js/ElementModel.js");
  drupal_add_js("$path/js/PropertiesModel.js");
  // Actual Panels that make up the editor.
  drupal_add_js("$path/js/MainPanel.js");
  drupal_add_js("$path/js/DisplayPanel.js");
  drupal_add_js("$path/js/TreePanel.js");
  drupal_add_js("$path/js/ElementForm.js");
  drupal_add_js("$path/js/PropertiesForm.js");
  drupal_add_js("$path/js/PreviewPanel.js");
  xml_form_builder_create_element_type_store();
  xml_form_builder_create_element_store($form_name);
  xml_form_builder_create_properties_store($form_name);
  return '<div id="xml-form-builder-editor"></div>';
}

/**
 * Save edits!
 */
function xml_form_builder_edit_save($form_name) {
  module_load_include('inc', 'xml_form_builder', 'FormBuilder');
  module_load_include('inc', 'xml_form_builder', 'JSONForm');
  $json_form = new JSONForm($form_name);
  $def = $json_form->getFormDefinition();
  //echo $def->saveXML();
  //FormBuilder::SetFormDefinition($form_name, $json_form->getFormDefinition());
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
 * Create a properties store.
 * 
 * @param string $form_name 
 */
function xml_form_builder_create_properties_store($form_name) {
  $data = Formbuilder::TransformPropertiesToJSON($form_name);
  $js = <<<EOT
  Ext.formbuilder.propertiesStore = Ext.create('Ext.data.Store', {
    storeId: 'PropertiesStore',
    model: 'Properties',
    autoLoad: true,
    autoSync: true,
    proxy: {
      type: 'memory',
      data: $data,
      reader: {
        type: 'json'
      }
    }
 });
 Ext.formbuilder.propertiesStore.sync();
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
  //Ext.formbuilder.foobar = $data;
  Ext.formbuilder.elementStore = Ext.create('Ext.data.TreeStore', {
    model: 'Element',
    root: $data
 });
EOT;
  drupal_add_js($js, 'inline');
}