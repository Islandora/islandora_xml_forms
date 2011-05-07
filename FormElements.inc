<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormElement');

/**
 * 
 */
class FormElements implements IteratorAggregate {

  /**
   * Dummy root element, doesn't get used.
   * 
   * @var FormElement
   */
  protected $_root;
  /**
   * The root of this set of elements.
   * 
   * @var FormElement
   */
  public $root;
  /**
   * A flat array that stores references to FormElements via the #hash value. 
   * This array will include references to elements that are not part of the collection, 
   * but were at one time. This gets populated when this object is created, and every time 
   * FormElements::toDrupalArray() is called. FormElement's are never removed from this array.
   * 
   * Users of this class should be aware in some cases they may need to manually register elements.
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

  public function __clone() {
    $form_name = $this->root->key;
    $root = clone $this->root; // Will recursively clone all children.
    $this->createRoot($form_name);
    $this->root = $root;
    $this->_root[$form_name] = $this->root;
    $this->elements = NULL;
    $this->registerElements();
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
    $this->_root = new FormElement();
    $this->_root[$root_name] = $this->root;
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
    $this->registerElements();
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
      $this->registerElements($new_element);
      return $new_element;
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
      unset($element->parent[$element->key]);
    }
    else {
      throw new Exception('Failed to remove element.');
    }
  }

  public function findElement($hash) {
    foreach ($this as $element) {
      if ($element->hash == $hash) {
        return $element;
      }
    }
    return NULL;
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
   * @param FormElements $element 
   */
  public function registerElements(FormElement $element = NULL) {
    if ($element) {
      $this->registerElement($element);
      foreach ($element as $child) {
        $this->registerElement($child);
      }
    }
    else {
      foreach ($this as $element) {
        $this->registerElement($element);
      }
    }
  }

  /**
   *
   * @param hash $hash
   * @return FormElement 
   */
  public function getRegisteredElement($hash) {
    return isset($this->elements[$hash]) ? $this->elements[$hash] : null;
  }

  /**
   *
   * @return array 
   */
  public function getRegisteredElements() {
    return $this->elements;
  }

}
