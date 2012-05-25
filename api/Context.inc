<?php



/**
 * @file
 *
 * Defines the three types of XPath context that can be used when defining forms (document, parent, self). Also
 * it defines a Class for determining which DOMNode a given context refers to.
 */
module_load_include('inc', 'php_lib', 'Enum');

/**
 * Enumerated types for Context.
 */
class ContextType extends Enum {
  const __default = 'document';
  const Document = 'document';
  const Parent = 'parent';
  const Self = 'self';
}

/**
 * Stores the type of context a Path uses, and retrieve the DOMNode a context refers to.
 */
class Context {

  /**
   * The type of context this class repersents.
   *
   * @var ContextType
   */
  protected $type;

  /**
   * Create XPath Context Object.
   *
   * @param strings $context
   */
  public function __construct(ContextType $type) {
    $this->type = $type;
  }

  /**
   * Checks to see if the DOMNode this context repersents exists for the given FormElement.
   *
   * @param XMLDocument $document
   * @param FormElement $element
   *
   * @return boolean
   */
  public function exists(XMLDocument $document, FormElement $element) {
    try {
      $node = $this->getNode($document->registry, $element);
      return TRUE;
    }
    catch (XMLFormsContextException $e) {
      return FALSE;
    }
  }

  /**
   * Gets the node defined by this context in relation to the given form element.
   *
   * @throws XMLFormsContextException
   *
   * @param XMLDocument $document
   * @param FormElement $element
   *
   * @return DOMNode
   *   If the context node is found it is returned, NULL otherwise.
   */
  public function getNode(XMLDocument $document, FormElement $element) {
    switch ($this->type->val) {
      case ContextType::Document:
        return NULL;
      case ContextType::Parent:
        return $this->getParent($document, $element);
      case ContextType::Self:
        return $this->getSelf($document, $element);
    }
  }

  /**
   * Gets the parent context node of the provided FormElement.
   *
   * @param XMLDocument $document
   * @param FormElement $element
   * @throws Exception
   *   If no parent form element defines a create or read action an exception is thrown.
   *
   * @return DOMNode
   *   If found the parent node is returned, otherwise NULL.
   */
  private function getParent(XMLDocument $document, FormElement $element) {
    $parent = $element->parent;
    while ($parent) {
      $selected_node = isset($parent->actions->read);
      $created_node = isset($parent->actions->create);
      if ($selected_node || $created_node) { // We've found a parent that should have a node registered
        if ($document->registry->isRegistered($parent->hash)) {
          return $document->registry->get($parent->hash);
        }
        throw new XMLFormsContextNotFoundException($this->type, $element);
      }
      $parent = $parent->parent; // Check next Parent.
    }
    throw new XMLFormsContextDefinitionException($this->type, $element);
  }

  /**
   * Gets the self context node of the provided Form Element.
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   *
   * @return DOMNode
   */
  private function getSelf(XMLDocument $document, FormElement $element) {
    if ($document->registry->isRegistered($element->hash)) {
      return $document->registry->get($element->hash);
    }
    throw new XMLFormsContextNotFoundException($this->type, $element);
  }

  /**
   * Returns a string describing this context.
   *
   * @return string
   */
  public function __toString() {
    return (string) $this->type;
  }

}

/**
 * Class repersenting Exceptions that can occur when looking for the context DOMNode.
 */
class XMLFormsContextException extends Exception {

  public function __construct(ContextType $type, FormElement $element, $message) {
    $variable_description = "The Form Element<br/>";
    $variable_description .= "&nbsp;Location: '{$element->getLocation()}'<br/>";
    $variable_description .= "&nbsp;Title: '{$element['#title']}'</br>";
    $variable_description .= "&nbsp;Type: '{$element['#type']}'</br>";
    $variable_description .= "&nbsp;Context: '$type->val'</br>";
    $message = $variable_description . 'Error: ' . $message;
    parent::__construct($message, 0);
  }

  public function __toString() {
    return __CLASS__ . ": [{$this->code}]: {$this->message}\n";
  }

}

/**
 * Class repersenting Exceptions that can be attributed directly to a misconfigured element context definition.
 */
class XMLFormsContextDefinitionException extends XMLFormsContextException {

  public function __construct(ContextType $type, FormElement $element) {
    $message = "Specifies a xpath context of {$type} but none is defined. Check the form defintion";
    parent::__construct($type, $element, $message);
  }

}

/**
 * The given context DOMNode could not be found. In some cases this is acceptible in others it is not.
 */
class XMLFormsContextNotFoundException extends XMLFormsContextException {

  public function __construct(ContextType $type, FormElement $element) {
    $message = "The DOMNode associated with the context {$type->val} was not found.";
    parent::__construct($type, $element, $message);
  }

}
