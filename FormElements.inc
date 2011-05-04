<?php

// $Id$

/**
 * @file
 *
 */

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
      $this->elements[$element->hash] = $element;
    }
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
    $this->elements[$element->hash] = $element;
  }

  /**
   * 
   */
  public function findElement($hash) {
    return isset($this->elements[$hash]) ? $this->elements[$hash] : null;
  }

}
