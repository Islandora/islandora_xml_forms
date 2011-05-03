<?php

// $Id$

/**
 * @file
 *
 */

/**
 * I think this class is more or less complete.
 */
class FormPopulator {

  /**
   *
   * @var Document 
   */
  protected $document;
  /**
   *
   * @var FormElements
   */
  protected $elements;

  /**
   * 
   * @param Document $document 
   */
  public function __construct(Document $document) {
    $this->document = $document;
  }

  /**
   * Set the default values for elements, from the $document, and register the nodes with the document.
   * 
   * @param FormElements $elements 
   *   Elements to set the default value for.
   */
  public function populate(FormElements $elements) {
    $this->elements = $elements;
    foreach ($elements as $element) {
      $this->process($element);
    }
  }

  /**
   *
   * @param FormElement $element 
   */
  public function process(FormElement $element) {
    $values = $this->getDefaultValues($element);
    if ($values) {
      list($node, $value) = array_shift($values);
      $this->setDefaultValue($element, $value);
      $this->document->register($node, $element);
      foreach ($values as $result) {
        list($node, $value) = $result;
        $twin = $this->elements->duplicate($element->hash);
        $this->setDefaultValue($twin, $value);
        $this->document->register($node, $element);
      }
    }
  }

  private function getDefaultValues(FormElement $element) { //Need a better name
    $properties = $element->properties;
    $read = $properties ? $properties->read : null;
    if ($read) {
      return $read->read($this->document);
    }
    return NULL;
  }

  public function setDefaultValue(FormElement $element, $value) {
    if ($value) {
      $element->default_value = (string) $value;
    }
  }

}