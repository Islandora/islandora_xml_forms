<?php

// $Id$

/**
 * @file
 *
 */

/**
 * Class that contains all registered relations between FormElements and other objects.
 */
class Registry {

  /**
   * This instance.
   * 
   * @var Registry
   */
  private static $instance = NULL;
  /**
   *
   * @var NodeRegistry
   */
  public $nodes;
  /**
   *
   * @var array
   */
  public $original;
  /**
   * Every element created, how to do ???
   * 
   * @var array
   */
  public $elements;

  /**
   * Creates a singleton reference to the registry.
   * 
   * @staticvar Registry $instance
   * @return Registry 
   */
  public static function getInstance() {
    if (empty(self::$instance)) {
      self::$instance = new Registry();
    }
    return self::$instance;
  }

  /**
   * Registers an element.
   * 
   * @param FormElement $element 
   * @param hash $original_hash
   */
  public static function registerElement(FormElement $element, $original_hash = NULL) {
    $instance = self::getInstance();
    $instance->elements[$element->hash] = $element;
    $instance->original[$element->hash] = empty($original_hash) ? $element : $instance->original[$original_hash];
  }

  /**
   * Creates Registry.
   */
  private function __construct() {
    $this->elements = array();
    $this->original = array();
  }

  /**
   * Set the instance on wakeup.
   */
  public function __wakeup() {
    self::$instance = $this;
  }

  /**
   * Finds an element.
   * 
   * If the element was created in this request or was stored in the session it should be able to find it.
   * 
   * @param hash $hash
   * @return FormElement 
   */
  public function findElement($hash) {
    if (isset($this->elements[$hash])) {
      return $this->elements[$hash];
    }
    return NULL;
  }

  /**
   *
   * @param mixed $element
   * @return FormElement
   *   The original FormElement, or NULL. 
   */
  public function findOriginal($hash) {
    if (isset($this->original[$hash])) {
      return $this->original[$hash];
    }
    return NULL;
  }

  /**
   *
   * @param mixed $element
   * @return FormElement 

    public function duplicateOriginal($element) {
    $hash = FormElement::isFormElement($element) ? $element->hash : $element;
    $element = FormElement::isFormElement($element) ? $element : $this->findElement($hash);
    $original = $this->findOriginal($hash);
    if (isset($original)) {
    $duplicate = clone $original;
    if ($element->parent) {
    $element->parent[] = $duplicate;
    }
    $this->mapToOrignal($duplicate, $original);
    return $duplicate;
    }
    return NULL;
    }

    /**
   *
   * @param FormElement $element
   * @param FormElement $original 

    private function mapToOrignal(FormElement $element, FormElement $original) {
    $itor1 = new RecursiveArrayIterator(array($element));
    $itor2 = new RecursiveArrayIterator(array($original));
    $flags = RecursiveIteratorIterator::SELF_FIRST;
    $itor1 = new RecursiveIteratorIterator($itor1, $flags);
    $itor2 = new RecursiveIteratorIterator($itor2, $flags);
    $iterator = new MultipleIterator();
    $iterator->attachIterator($itor1);
    $iterator->attachIterator($itor2);
    foreach ($iterator as $item) {
    list($duplicate, $orig) = $item;
    $this->original[$duplicate->hash] = $orig;
    }
    }

    /**
   *
   * @param mixed $element
   * @return DOMNode 
   */
  public function findNode($element) {
    return $this->nodes->find($element);
  }

}