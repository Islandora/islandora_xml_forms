<?php
/**
 * @file
 * This file lists and documents all available hook functions to manipulate
 * data.
 */

/**
 * This hook lets modules add default forms to the formbuilder.
 *
 * @return
 *   This function should return an array of the form:
 *   @code
 *   return array(
 *     'Unique Form Name' => array(
 *       'form_file' => 'full/path/to/form/definition/file.xml'
 *     )
 *   );
 *   @endcode
 */
function hook_islandora_xml_form_builder_forms() {}