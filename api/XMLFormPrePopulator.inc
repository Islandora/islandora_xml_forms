<?php



/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'XMLDocument');
module_load_include('inc', 'php_lib', 'DOMHelpers');

/**
 *
 */
class XMLFormPrePopulator {

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
  public function __construct(XMLDocument $document) {
    $this->registry = $document->registry;
  }

  /**
   * Populate a Drupal form.
   *
   * @param array $form
   */
  public function prePopulate(FormElement $root) {
    $this->prePopulateElement($root);
    $root->eachDecendant(array($this, 'prePopulateElement'));
  }

  /**
   *
   * @param FormElement $element
   */
  public function prePopulateElement(FormElement $element) {
    $value = $this->getDefaultValue($element);
    $this->setDefaultValue($element, $value);
  }

  /**
   * Get the default value for the given element.
   *
   * @param FormElement $element
   */
  private function getDefaultValue(FormElement $element) {
    $node = $this->registry->get($element->hash);
    return $this->getNodeValue($node);
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
  private function getNodeValue(DOMNode $node = NULL) {
    if (empty($node)) {
      return NULL;
    }
    switch (get_class($node)) {
      case 'DOMAttr':
        return $node->nodeValue;
      case 'DOMElement':
        if (!dom_element_has_child_elements($node)) {
          return $node->nodeValue; // May not be good...
        }
      default:
        return NULL;
    }
  }

}
