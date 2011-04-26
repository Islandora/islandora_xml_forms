<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormControlGeneratorFunctions');

class FormControl {

  /**
   *
   * @var string 
   */
  protected $name;
  /**
   *
   * @var mixed 
   */
  protected $value;

  /**
   *
   * @param DOMElement $form_control 
   */
  public function __construct(DOMElement &$form_control) {
    $this->name = $form_control->tagName;
    $this->setValue($form_control);
  }

  /**
   *
   * @param DOMElement $form_control 
   */
  private function setValue(DOMElement &$form_control) {    
    $function = FormControlGeneratorFunctions::getFunctionFromName($this->name);
    $this->value = FormControlGeneratorFunctions::$function($form_control);
  }

  /**
   * Get this form controls name.
   * 
   * @return string
   *   This form control's name. 
   */
  public function getName() {
    return $this->name;
  }
  
  /**
   * 
   * @return mixed
   *   This form controls value.
   */
  public function getValue() {
    
  }

  /**
   * Converts this form control into a Drupal API form control.
   * 
   * @return array
   */
  public function toArray() {
    return array('#' . $this->name => $this->value);
  }

}
