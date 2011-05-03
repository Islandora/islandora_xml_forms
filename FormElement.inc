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
   * A flat array that stores references to FormElements via the #hash value.
   * 
   * @var array 
   */
  protected $elements;

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
    $form_elements = array();
    $children = $elements->children();
    foreach ($children as $child) {
      $child = FormElement::createFromSimpleXML($child);
      $form_elements[] = $child;
    }
    $form_elements = new FormElements($root_name, $form_elements);
    return $form_elements;
  }

  /**
   *
   * @param string $root_name 
   */
  public function __construct($root_name, array $elements) {
    $this->createRoot($root_name);
    $this->appendElementsToRoot($elements);
    $this->createHashTable();
  }

  /**
   *
   * @param array $elements 
   */
  private function appendElementsToRoot(array $elements) {
    foreach ($elements as $element) {
      $this->root[$element->name] = $element;
    }
  }

  /**
   * 
   */
  private function createHashTable() {
    foreach ($this as $element) {
      $hash = $this->getHash($element);
      $element->hash = $hash;
      $this->elements[$hash] = $element;
    }
  }

  /**
   * 
   */
  private function getHash(FormElement $element) {
    static $hash = 0; // Unique...
    $hash++;
    return $hash;
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

  /**
   *
   * @param type $hash 
   */
  public function duplicateElement($hash) {
    $element = $this->findElement($hash);
    if ($element) {
      $new_element = clone $element;
      $element->parent[] = $new_element;
      $this->registerElement($new_element);
    }
    else {
      throw new Exception('Failed to duplicate element.');
    }
  }

  /**
   *
   * @param type $hash 
   */
  public function removeElement($hash) {
    $element = $this->findElement($hash);
    if ($element) {
      unset($this->elements[$hash]);
      unset($element->parent[$element->key]);
    }
    else {
      throw new Exception('Failed to remove element.');
    }
  }

  /**
   *
   * @param FormElement $element 
   */
  public function registerElement(FormElement $element) {
    $hash = $this->getHash($element);
    $element->hash = $hash;
    $this->elements[$hash] = $element;
  }

  /**
   * 
   */
  public function findElement($hash) {
    return isset($this->elements[$hash]) ? $this->elements[$hash] : null;
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
  public $properties;
  /**
   * 
   * @var string 
   */
  public $name;
  /**
   * Id for this element.
   * @var type 
   */
  public $hash;

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
    $form_element ->properties = isset($element->xml_properties) ? new XMLProperties($form_element, $element->xml_properties) : NULL;
    $form_element->controls = FormControls::createFromSimpleXML($element);
    $children = $element->xpath('child::children/element');
    foreach ($children as $child) {
      $child = self::createFromSimpleXML($child);
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
      return isset($this->controls[$offset]);
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
      $value->parent = $this;
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

  public function getKey() {
    if ($this->name) {
      return $this->name;
    }
    else {
      foreach ($this->parent as $key => $sibling) {
        if ($sibling === $this) {
          return $key;
        }
      }
    }
    return NULL;
  }

  /**
   *
   * @param string $name 
   */
  public function __get($name) {
    if ($name == 'key') {
      return $this->getKey();
    }
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