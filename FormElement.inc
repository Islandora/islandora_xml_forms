<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormControl');
module_load_include('inc', 'xml_form_api', 'XMLProperties');

/**
 * 
 */
class FormElements implements IteratorAggregate {

  /**
   * Dummy root element, doesn't get used.
   * 
   * @var FormElement
   */
  public $_root;
  /**
   * The root of this set of elements.
   * 
   * @var FormElement
   */
  public $root;
  /**
   *
   * @var RecursiveIteratorIterator
   */
  protected $iterator;

  /**
   *
   * @param type $root_name
   * @param type $xml_definition
   * @return type 
   */
  public static function createFromXMLDefiniton($root_name, $xml_definition) {
    return self::createFromSimpleXML($root_name, simplexml_load_string($xml_definition));
  }

  /**
   *
   * @param type $root_name
   * @param SimpleXMLElement $elements 
   */
  public static function createFromSimpleXML($root_name, SimpleXMLElement $elements) {
    $form_elements = new FormElements($root_name);
    $children = $elements->children();
    foreach ($children as $child) {
      $child = FormElement::createFromSimpleXML($child);
      $form_elements->root[$child->name] = $child;
    }
    return $form_elements;
  }

  /**
   *
   * @param string $root_name 
   */
  public function __construct($root_name) {
    $this->createRoot($root_name);
  }

  /**
   *
   * @param string $root_name 
   */
  private function createRoot($root_name) {
    $this->root = new FormElement();
    $this->root->name = $root_name;
    $this->root->tree = new FormControl('#tree', true);
    $this->_root = new ArrayObject(array($root_name => $this->root));
    return $this->_root;
  }

  /**
   *
   * @return RecursiveIteratorIterator
   */
  public function getIterator() {
    return new RecursiveIteratorIterator(new RecursiveArrayIterator($this->_root), RecursiveIteratorIterator::SELF_FIRST);
  }

  /**
   * 
   */
  public function toDrupalForm() {
    return array($this->root->name => $this->root->toDrupalForm());
  }

}

/**
 * 
 */
class FormElement extends ArrayObject {

  /**
   * The parent element of this element if it exists.
   * @var FormElement 
   */
  public $parent;
  /**
   * The form controls that define this element.
   * 
   * @var FormControls
   */
  public $controls;
  /**
   *
   * @var XMLProperties 
   */
  public $xmlProperties;
  /**
   * 
   * @var string 
   */
  public $name;

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
    $form_element->xmlProperties = isset($element->xml_properties) ? new XMLProperties($element->xml_properties) : NULL;
    $form_element->controls = FormControls::createFromSimpleXML($element);
    $children = $element->xpath('child::children/element');
    foreach ($children as $child) {
      $child = self::createFromSimpleXML($child);
      $child->parent = $form_element;
      $form_element[$child->name] = $child;
    }
    return $form_element;
  }

  /**
   * Creates a FormElement.
   * 
   * By default it won't have any form controls or children.
   */
  public function __construct() {
    $this->controls = new FormControls();
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
    return $output;
  }

  /**
   * Returns the XML repersentation of this object. The XML repersentation is defined by FormDefinition.xsd.
   * 
   * @return SimpleXMLElement
   */
  public function toXML() {
    
  }

  /**
   * Check to see if a FormControl or child FormElement exists.
   * 
   * @param mixed $offset
   *   Key for either a FormControl or child FormElement, generally a integer or string.  
   * @return boolean
   *   True if the offset refers to a FormControl or a child FormElement.
   */
  public function offsetExists($offset) {
    if (parent::offsetExists($offset)) {
      return true;
    }
    else {
      return isset($this->contols[$offset]);
    }
  }

  /**
   * Retrieves a FormControl or child FormElement if it exists.
   * 
   * @param mixed $offset
   *   Key for either a FormControl or child FormElement, generally a integer or string.  
   * @return mixed
   *   It returns either a FormControl or a child FormElement.
   */
  public function offsetGet($offset) {
    if (parent::offsetExists($offset)) {
      return parent::offsetGet($offset);
    }
    return $this->controls[$offset];
  }

  /**
   * Adds/modifies a FormControl or child FormElement.
   * 
   * @param mixed $offset
   *   Key for either a FormControl or child FormElement, generally a integer or string.  
   * @param mixed $value 
   *  Either a FormControl or a FormElement.
   */
  public function offsetSet($offset, $value) {
    if (isset($value) && is_object($value) && get_class($value) == 'FormElement') {
      parent::offsetSet($offset, $value);
    }
    else {
      $this->controls[$offset] = $value;
    }
  }

  /**
   * Removes a FormControl or child FormElement.
   * 
   * @param mixed $offset 
   *   Key for either a FormControl or child FormElement, generally a integer or string.  
   */
  public function offsetUnset($offset) {
    if (parent::offsetExists($offset)) {
      unset($this[$offset]);
    }
    else {
      unset($this->controls[$offset]);
    }
  }

  /**
   *
   * @param string $name 
   */
  public function __get($name) {
    if ($this->offsetExists($name)) {
      return $this->offsetGet($name);
    }
    return $this->controls->$name;
  }

  /**
   *
   * @param string $name
   * @param mixed $value 
   */
  public function __set($name, $value) {
    if (isset($value) && is_object($value) && get_class($value) == 'FormElement') {
      $this->offsetSet($name, $value);
    }
    else {
      $this->controls->$name = $value;
    }
  }

}