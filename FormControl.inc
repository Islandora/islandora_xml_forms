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
   * @param SimpleXMLElement $form_control 
   */
  public function __construct(SimpleXMLElement &$form_control) {
    $this->name = $form_control->getName();
    $this->setValue($form_control);
  }

  /**
   *
   * @param SimpleXMLElement $form_control 
   */
  private function setValue(SimpleXMLElement &$form_control) {    
    $function = FormControlGeneratorFunctions::getFunctionFromName($this->name);
    $hack = dom_import_simplexml($form_control);
    $this->value = FormControlGeneratorFunctions::$function($hack);
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
