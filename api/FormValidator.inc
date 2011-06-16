<?php

// $Id$

/**
 * @file
 *
 */

module_load_include('inc', 'xml_form_api', 'FormValues');
module_load_include('inc', 'xml_form_api', 'FormElements');

class FormValidator {

  /**
   *
   * @var array 
   */
  protected $values;

  public function __construct(FormValues $values) {
    
  }

  public function validate(FormElements $elements) {
    
  }

}