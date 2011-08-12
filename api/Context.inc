<?php

// $Id$

/**
 * @file
 *
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
 * 
 */
class Context {

  /**
   * The type of context, restricted to this classes constants.
   * 
   * @var string
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
   * Gets the node defined by this context in relation to the given form element.
   * 
   * @param NodeRegistry $registry
   * @param FormElement $element
   * 
   * @throws Exception
   *   If the parent context is not defined.
   * 
   * @return DOMNode
   *   If the context node is found it is returned, NULL otherwise.
   */
  public function getNode(NodeRegistry $registry, FormElement $element) {
    switch ($this->type->val) {
      case ContextType::Document:
        return NULL;
      case ContextType::Parent:
        return $this->getParent($registry, $element);
      case ContextType::Self:
        return $this->getSelf($registry, $element);
    }
  }

  /**
   * Gets the parent context node of the provided FormElement.
   * 
   * @param NodeRegistry $registry
   * @param FormElement $element
   * 
   * @throws Exception
   *   If no parent form element defines a create or read action an exception is thrown.
   * 
   * @return DOMNode
   *   If found the parent node is returned, otherwise NULL.
   */
  private function getParent(NodeRegistry $registry, FormElement $element) {
    $parent = $element->parent;
    while ($parent) {
      $selected_node = isset($parent->actions->read);
      $created_node = isset($parent->actions->create);
      if ($selected_node || $created_node) { // We've found a parent that should have a node registered
        return $registry->getNode($parent->hash);
      }
      $parent = $parent->parent; // Check next Parent.
    }
    throw new Exception("The Form Element <br/>at: '{$element->getLocation()}' <br/>title: '{$element->title}' </br>type: '{$element->type}'.</br> Specifies a xpath context of parent but none is defined. Check the form defintion.");
  }

  /**
   * Gets the self context node of the provided Form Element.
   * 
   * @param NodeRegistry $registry
   * @param FormElement $element
   * 
   * @return DOMNode
   */
  private function getSelf(NodeRegistry $registry, FormElement $element) {
    $node = $registry->getNode($element->hash);
    return $node;
  }

  /**
   * Returns a string describing this context.
   * 
   * @return string
   */
  public function __toString() {
    return $this->type->val;
  }

}
