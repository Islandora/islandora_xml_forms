<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormElementArrayObject');
module_load_include('inc', 'xml_form_api', 'XMLProperties');

/**
 * 
 */
class FormElement extends FormElementArrayObject {

  /**
   *
   * @var XMLProperties 
   */
  public $properties;
  /**
   * Id for this element.
   * @var type 
   */
  protected $_hash;

  /**
   *
   * @param string $xml_definition
   * @return type 
   */
  public static function createFromXMLDefiniton($xml_definition) {
    return self::createFromSimpleXML(simplexml_load_string($xml_definition));
  }

  /**
   *
   * @param SimpleXMLElement $simple 
   */
  public static function createFromSimpleXML(SimpleXMLElement $element) {
    $form_element = new FormElement();
    $form_element->name = isset($element['key']) ? (string) $element['key'] : NULL;
    $form_element->properties = isset($element->xml_properties) ? new XMLProperties($form_element, $element->xml_properties) : NULL;
    $form_element->controls = FormControls::createFromSimpleXML($element);
    $children = $element->xpath('child::children/element');
    foreach ($children as $child) {
      $child = self::createFromSimpleXML($child);
      $form_element[$child->name] = $child;
    }
    return $form_element;
  }

  /**
   * Determines if the object is of type FormElement
   * 
   * @param mixed $obj
   *   The type to check if it is a FormElement
   * @return boolean 
   *   TRUE if $obj is a FormElement FALSE otherwise.
   */
  public static function isFormElement($obj) {
    return isset($obj) && is_object($obj) && get_class($obj) == 'FormElement';
  }

  /**
   * Creates a FormElement.
   * 
   * By default it won't have any form controls or children.
   */
  public function __construct() {
    parent::__construct();
    $this->_hash = spl_object_hash($this);
  }

  /**
   * 
   */
  public function __clone() {
    unset($this->name);
    $this->_hash = spl_object_hash($this);
  }

  /**
   * Returns the Drupal Form repersentation of this object.
   * 
   * @return array
   *   Drupal Form array.
   */
  public function toDrupalForm() {
    $output = $this->controls->toDrupalForm();
    foreach ($this as $key => $child) {
      $output[$key] = $child->toDrupalForm();
    }
    $output['#hash'] = $this->hash;
    return $output;
  }

  /**
   * Returns the XML repersentation of this object. The XML repersentation is defined by FormDefinition.xsd.
   * 
   * @return SimpleXMLElement
   */
  public function toXML() {
    
  }

  public function __get($name) {
    switch ($name) {
      case 'hash':
        return $this->_hash;
      default:
        return parent::__get($name);
    }
  }

  public function __set($name, $value) {
    parent::__set($name, $value);
  }

}