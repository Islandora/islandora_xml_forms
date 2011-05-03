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

  protected $definition;
  protected $document;
  protected $elements;

  
  public function __construct(Document $document) {
    $this->document = $document;
  }

  public function setDefaultValues(FormDefinition $definition) {
    $this->definition = $definition;
    $this->elements = $this->definition->elements;
    foreach ($this->elements as $element) {
      $this->setDefaultValue($element);
    }
  }

  public function setDefaultValue(FormElement $element) {
    $values = $this->getDefaultValues($element);
    if ($values) {
      /* Store the first node found */
      list($node, $value) = array_shift($values);
      $this->setDefaultValueAndRegisterElement($element, $value, $node);
      /* Create new elements to store the extra values. */
      foreach ($values as $result) {
        list($node, $value) = $result;
        $form_element = $this->elements->duplicate($element->hash);
        $this->setDefaultValueAndRegisterElement($form_element, $value, $node);
      }
    }
  }

  private function setDefaultValueAndRegisterElement($form_element, $value, $node) {
    if ($value) {
      $form_element->default_value = (string) $value;
    }
    $this->document->register($node, $form_element);
  }

  private function getDefaultValues(FormElement $element) {
    $properties = $element->properties;
    $read = $properties ? $properties->read : null;
    if ($read) {
      return $read->read($this->document);
    }
    return NULL;
  }

}