<?php

/**
 * @file
 *
 * Callbacks and functions for the Main Page.
 */

/**
 * Show the Main page.
 *
 * Here the user can select which action they would like to do.
 *
 * @return array
 *   The tble to display.
 */
function xml_form_builder_main() {

  module_load_include('inc', 'xml_form_builder', 'XMLFormDatabase');
  $names = XMLFormDatabase::GetNames();

  // No forms exist can only create.
  if (count($names) == 0) { 
    return '<div>No forms are defined. Please create a new form.</div><br/>';
  }

  $table = array(
    'header' => array(
      array(
        'data' => t('Title'),
      ),
      array(
        'data' => t('Operations'),
        'colspan' => 5,
      ),
    ),
    'rows' => array(),
  );

  foreach($names as $name) {
    $copy = l('Copy', xml_form_builder_get_copy_form_path($name));
    $edit = l('Edit', xml_form_builder_get_edit_form_path($name));
    $view = l('View', xml_form_builder_get_view_form_path($name));
    $export = l('Export', xml_form_builder_get_export_form_path($name));
    $delete = l('Delete', xml_form_builder_get_delete_form_path($name));
    $table['rows'][] = array($name, $copy, $edit, $view, $export, $delete);
  }

  return theme('table', $table);
}