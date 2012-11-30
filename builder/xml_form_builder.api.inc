<?php
/**
 * @file
 * This file lists and documents all available hook functions to manipulate
 * data.
 */

/**
 * This hook allows modules to add default forms to form builder.
 *
 * @return array
 *   An associative array mapping unique names to associative arrays containing a
 *   single key:
 *   - form_file: A string containing the path to the form definition, relative
 *     to the webserver's document root (such that I might be opened
 */
function hook_islandora_xml_form_builder_forms() {
  return array(
    'Unique Form Name' => array(
      'form_file' => 'full/path/to/form/definition/file.xml',
    ),
  );
}

/**
 * This hook allows modules to register XSLT transformations.
 *
 * @return array
 *   An associative array mapping a shortened name to the full path of the
 *   transformation.
 */
function hook_islandora_xml_form_builder_get_transforms() {
  return array(
    'awesome.xslt' => 'sites/all/modules/my_cool_module/transforms/awesome.xslt',
  );
}

/**
 * This hook allows modules to register default form associations.
 *
 * @return array
 *   An associative array mapping a unique name to an associative array which
 *   contains the following keys:
 *   - content_model: A string containing the PID for a content model.
 *   - form_name: A string identifying a form visible to XML Forms.
 *   - dsid: A string identifying what datastream should be created using the
 *     XML generated on submission.
 *   - title_field: A string containing an "array path", identifying what value
 *     in the submitted form should be used as a title.
 *   - transform: A string identifying which transform can be used on the XML
 *     to produce DC.  Can be "No Transform" if the form generates DC.
 *   - template: A string whose contents should be used to prepopulate the
 *     form.  Can be empty or FALSE.
 */
function hook_islandora_xml_form_builder_form_associations() {
  return array(
    // By convention, the unique name should start with your modules name.
    'unique_assoication_name' => array(
      'content_model' => 'islandora:sp_basic_image',
      'form_name' => 'Image DC Form',
      'dsid' => 'DC',
      'title_field' => "['dc:title']",
      'transform' => 'No Transform',
      'template' => FALSE,
    ),
  );
}
