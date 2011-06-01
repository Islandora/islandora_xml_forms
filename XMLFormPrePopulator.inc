<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'NodeRegistry');
module_load_include('inc', 'xml_form_api', 'FormElements');
module_load_include('inc', 'xml_form_api', 'FormValues');
module_load_include('inc', 'xml_form_api', 'Utils');

/**
 * 
 */
class XMLFormPopulator {

  /**
   * Node Registry.
   * 
   * @var NodeRegistry
   */
  protected $registry;
  /**
   *
   * @var FormValues
   */
  protected $values;

  /**
   * Create FormPopulater.
   * 
   * @param NodeRegistry $registry 
   */
  public function __construct(NodeRegistry $registry, FormValues $values) {
    $this->registry = $registry;
    $this->values = $values;
  }

  /**
   * Populate a Drupal form.
   * 
   * @param array $form
   */
  public function populate(FormElements $elements) {
    foreach ($elements as $element) {
      $value = $this->getDefaultValue($element);
      $this->setDefaultValue($element, $value);
    }
  }

  /**
   * Get the default value for the given element.
   * 
   * @param FormElement $element
   */
  private function getDefaultValue(FormElement $element) {
    $value = $this->getSubmittedValue($element);
    if (empty($value)) {
      $value = $this->getRegisteredValue($element);
    }
    return $value;
  }

  /**
   *
   * @param FormElement $element
   * @return mixed 
   */
  private function getSubmittedValue(FormElement $element) {
    return $this->values->getValue($element);
  }

  /**
   *
   * @param FormElement $element
   * @return mixed 
   */
  private function getRegisteredValue(FormElement $element) {
    $node = $this->registry->getNode($element->hash);
    if ($node) {
      return $this->getNodeValue($node);
    }
    return NULL;
  }

  /**
   * Set the FormElements default value if appropriate.
   * 
   * @param FormElement $element 
   */
  private function setDefaultValue(FormElement $element, $value) {
    if (isset($value)) {
      $element->default_value = $value;
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
    switch (get_class($node)) {
      case 'DOMAttr':
        return $node->nodeValue;
      case 'DOMElement':
        if (!Utils::hasChildDOMElements($node)) {
          return $node->nodeValue; // May not be good...
        }
      default:
        return NULL;
    }
  }

}