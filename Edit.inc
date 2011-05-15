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
  $path = drupal_get_path('module', 'xml_form_builder');
  //drupal_add_js("$path/js/jsTree/_lib/jquery.js");
  drupal_add_js("$path/js/jsTree/jquery.jstree.js");
  drupal_add_js("$path/js/EditForm.js");
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
  $type = $form[Form::ROOT]['element']['type']['#default_value'];
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