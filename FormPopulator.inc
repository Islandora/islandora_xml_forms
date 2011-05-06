<?php

// $Id$

/**
 * @file
 *
 */

/**
 * 
 */
class FormPopulator {

  /**
   * Node Registry.
   * 
   * @var NodeRegistry
   */
  protected $registry;

  /**
   * Create FormPopulater.
   * 
   * @param NodeRegistry $registry 
   */
  public function __construct(NodeRegistry $registry) {
    $this->registry = $registry;
  }

  /**
   * Set the default values for elements, from the $document, and register the nodes with the document.
   * 
   * @param FormElements $elements 
   *   Elements to set the default value for.
   */
  public function populate(FormElements $elements) {
    foreach ($elements as $element) {
      $this->setDefaultValue($element);
    }
    return $elements;
  }

  /**
   * Set the FormElements default value if appropriate.
   * 
   * @param FormElement $element 
   */
  private function setDefaultValue(FormElement $element) {
    $node = $this->registry->getNode($element);
    if ($node) {
      $value = $this->getNodeValue($node);
      if (isset($value)) {
        $element->default_value = (string) $value;
      }
    }
  }

  /**
   * Get the value of the node.
   * 
   * Only return values for DOMAttr and DOMElements that only contain character data.
   * 
   * @param DOMNode $node
   * @return string
   *   The DOMNode's value if appropriate, otherwise return NULL.
   */
  private function getNodeValue(DOMNode $node) {
    return $node->nodeValue; // May not be good...
  }

}