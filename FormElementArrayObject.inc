<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormControl');

/**
 * This class defines array style access for FormElement's. This includes setting the properties of child/parent elements
 * so that the correct heiarchy structure of FormElements remains intact while adding/removing FormElement's.
 */
abstract class FormElementArrayObject extends ArrayObject {

  /**
   * The name of the element, this is used as the key for the element if defined.
   * @var string
   */
  public $name;
  /**
   * The parent element of this element if it exists.
   * @var FormElement 
   */
  protected $_parent;
  /**
   * The form controls that define this element.
   * 
   * @var FormControls
   */
  public $controls;

  /**
   *
   */
  public function __construct() {
    parent::__construct();
    $this->controls = new FormControls();
  }

  /**
   * 
   */
  public function __clone() {
    $this->name = NULL;
    $this->orphan();
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
    if (FormElement::isFormElement($value)) {
      $value->orphan();
      $value->_parent = $this;
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
      $child = $this->offsetGet($offset);
      $child->_parent = NULL;
      parent::offsetUnset($offset);
    }
    else {
      unset($this->controls[$offset]);
    }
  }

  /**
   *
   * @param string $name
   * @return mixed
   */
  public function __get($name) {
    switch ($name) {
      case 'key':
        return $this->getKey();
      case 'parent':
        return $this->_parent;
      default:
        return $this->getDefault($name);
    }
  }

  /**
   *
   * @param string $name
   * @param mixed $value 
   */
  public function __set($name, $value) {
    switch ($name) {
      case 'parent':
        $this->setParent($value);
      default:
        $this->setDefault($name, $value);
    }
  }

  /**
   * Get the index of this element in its parent array.
   * 
   * @return mixed
   *   The string or integer key of this element in its parent array. 
   */
  private function getKey() {
    if ($this->_parent) {
      foreach ($this->_parent as $key => $sibling) {
        if ($sibling === $this) {
          return $key;
        }
      }
    }
    return NULL;
  }

  /**
   *
   * @param type $name
   * @return type 
   */
  private function getDefault($name) {
    if ($this->offsetExists($name))
      return $this->offsetGet($name);
    else {
      return $this->controls->{$name};
    }
  }

  /**
   *
   * @param type $name
   * @param type $value 
   */
  private function setDefault($name, $value) {
    if (FormElement::isFormElement($value)) {
      $this->offsetSet($name, $value);
    }
    else {
      $this->controls->{$name} = $value;
    }
  }

  /**
   * 
   */
  private function orphan() {
    if (FormElement::isFormElement($this->_parent)) {
      if ($this->key) {
        unset($this->_parent[$this->key]);
      }
      $this->_parent = NULL;
    }
  }

  /**
   *
   * @param FormElement $parent 
   */
  private function setParent($parent) {
    if (FormElement::isFormElement($parent)) {
      $parent[$this->name] = $this;
    }
    else if ($parent === NULL) {
      $this->orphan();
    }
    else {
      throw new Exception('The parent of a form element must be null or a FormElement.');
    }
  }

}