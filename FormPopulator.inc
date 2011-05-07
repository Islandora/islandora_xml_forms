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
class FormPopulator {

  /**
   * Node Registry.
   * 
   * @var NodeRegistry
   */
  protected $registry;
  protected $elements;
  protected $values;

  /**
   * Create FormPopulater.
   * 
   * @param NodeRegistry $registry 
   */
  public function __construct(NodeRegistry $registry, FormElements $elements, FormValues $values) {
    $this->registry = $registry;
    $this->elements = $elements;
    $this->values = $values;
  }

  /**
   * Populate a Drupal form.
   * 
   * @param array $form
   */
  public function populate(array &$form) {
    $children = element_children($form);
    foreach ($children as $child) {
      $element = &$form[$child];
      $value = $this->getDefaultValue($element['#hash']);
      $this->setDefaultValue($element, $value);
      $this->populate($element);
    }
  }

  /**
   * Get the default value from the Document.
   * 
   * @param hash $hash 
   */
  private function getDefaultValue($hash) {
    $value = $this->getSubmitedValue($hash);
    if (empty($value)) {
      $value = $this->getRegisteredValue($hash);
    }
    return $value;
  }

  /**
   *
   * @param hash $hash
   * @return mixed 
   */
  private function getSubmitedValue($hash) {
    $element = $this->elements->findElement($hash);
    if ($element) {
      return $this->values->getValue($element);
      ;
    }
    return NULL;
  }

  /**
   *
   * @param hash $hash
   * @return mixed 
   */
  private function getRegisteredValue($hash) {
    $node = $this->registry->getNode($hash);
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
  private function setDefaultValue(array &$form, $value) {
    if (isset($value)) {
      $form['#value'] = $value;
    }
    else if (isset($form['#default_value'])) {
      $form['#value'] = $form['#default_value'];
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