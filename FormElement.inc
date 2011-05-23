<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormElementArrayObject');
module_load_include('inc', 'xml_form_api', 'Actions');
module_load_include('inc', 'xml_form_api', 'Registry');

/**
 * 
 */
class FormElement extends FormElementArrayObject {

  /**
   * Id for this element.
   * @var type 
   */
  protected $_hash;
  /**
   *
   * @var Actions 
   */
  public $actions;

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
    $form_element->actions = Actions::createFromSimpleXML($element->actions);
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
    Registry::registerElement($this);
  }

  /**
   * Create a Copy of this Object. It will be unnamed and the 
   * new object won't be attached to a Parent.
   */
  public function __clone() {
    parent::__clone();
    $original_hash = $this->_hash;
    $this->_hash = spl_object_hash($this);
    Registry::registerElement($this, $original_hash);
    foreach ($this as $child) { // Clone Children.
      $key = $child->key;
      $this[$key] = clone $child;
    }
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
    $output['#hash'] = $this->hash; // This can be used with ajax events to determine what FormElement should be changed.
    return $output;
  }

  /**
   * Get an element's property, child, or FormControl.
   * 
   * Defined properties: hash, key, parent, collection
   * 
   * @param string $name
   *   The name of the property, child or FormControl to get.
   * @return mixed 
   *   The property, child, or FormControl requested. NULL if not found.
   */
  public function __get($name) {
    switch ($name) {
      case 'hash':
        return $this->_hash;
      default:
        return parent::__get($name);
    }
  }

  /**
   * Set this element's property, child, or FormControl.
   * 
   * Defined properties: hash, key, parent, collection
   * 
   * @param string $name
   *   The name of the property, child or FormControl to set.
   * @param mixed $value 
   */
  public function __set($name, $value) {
    parent::__set($name, $value);
  }


  /**
   * @return DOMNode
   */
  public function asNode(DOMDocument $document) {
    // Create Element
    $element = $document->createElement('element');
    if (isset($this->name)) {
      $element->setAttribute('key', $this->name);
    }
    // Controls
    foreach ($this->controls as $control) {
      $control = $control->asNode($document);
      $element->appendChild($control);
    }
    // Actions
    if (isset($this->actions)) {
      $actions = $this->actions->asNode($document);
      $element->appendChild($actions);
    }
    // Children
    if (count($this) > 0) {
      $children = $document->createElement('children');
      foreach ($this as $child) {
        $child = $child->asNode($document);
        $children->appendChild($child);
      }
      $element->appendChild($children);
    }
    return $element;
  }

}