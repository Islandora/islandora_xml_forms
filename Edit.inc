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
  $output = drupal_get_form('create_element');
  return theme('edit_form_page', $output);
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
    $path = drupal_get_path('module', 'xml_form_builder');
    $doc = new DOMDocument();
    $doc->load("$path/xml/Element.xml");
    $form_definition = FormDefinition::createFromXMLDefiniton($doc->saveXML());
    $document = new Document($form_definition->properties->document);
    $form->initialize($form_definition, $document);
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
  $types = FormBuilder::GetElementTypes();
  foreach ($types as $type) {
    unset($types[key($types)]);
    $types[$type] = $type;
  }
  $form[Form::ROOT]['element']['type']['#options'] = $types;
}

/**
 * Preprocess tabs theme hook.
 * 
 * @param array $vars 
 */
function xml_form_elements_preprocess_edit_form_page(&$vars) {
  $path = drupal_get_path('module', 'xml_form_builder');
  drupal_add_js("$path/js/jsTree/_lib/jquery.js");
  drupal_add_js("$path/js/jsTree/jquery.jstree.js");
  drupal_add_js("$path/js/EditForm.js");
  $vars['scripts'] = drupal_get_js();
  $vars['element_form'] = $vars[0];
}
