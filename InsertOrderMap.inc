<?php

// $Id$

/**
 * @file
 *
 * Builds a map that defines where elements can be inserted
 */

class InsertOrder {

  /**
   *
   * @var Schema
   */
  protected $schema;
  /**
   *
   * @var DOMElement
   */
  protected $element;
  /**
   *
   * @var DOMElement
   */
  protected $reference;
  /**
   * 
   */
  protected $type;

  /**
   *
   * @param Schema $schema
   * @param string $path 
   */
  public function __construct(Schema $schema, $path, DOMElement $parent) {
    $this->schema = $schema;
    $this->path = $path;
    $this->parent = $parent;
    $this->element = $this->getElementDeclaration($path);
    if (SchemaTag::isReference($this->element)) {
      $this->reference = $this->schema->getUniqueNode("/xs:schema/xs:element[@name='{$parent->localName}']");
    }

    // Get type definition.
    if (isset($this->reference) && SchemaTag::hasType($node)) {
      // Get type..
    }
    else {
      // Type is inline...
    }
    // Now that we have the type, see if it's an extension.
    // Build rules...
  }

  /**
   * Get the element declaration.
   * 
   * @param string $path
   * @return DOMElement
   */
  private function getElementDeclaration($path) {
    $results = $this->schema->query($path);
    if (count($results) != 1) {
      throw new Exception('The provided schema path did not select a unique node. Path: ' . $path);
    }
    $element = array_shift($results);
    if (!SchemaTag::isElement($element)) {
      throw new Exception('The given path does not select an element type. Path: ' . $path);
    }
    return $element;
  }

}

class Blah {

  public $name;
  public $type; // Element Any 
  public $min;
  public $max;

  public function __construct(DOMElement $node) {
    
  }

}

/**
 * @file
 *
 * Parses a given schema for the insert order of an element given at path.
 */
class InsertOrderSchemaParser {
  const ComplexType = 'complexType';
  const Element = 'element';
  const ElementGroup = 'group';
  const Attribute = 'attribute';
  const AttributeGroup = 'attributeGroup';
  const Any = 'any';
  const All = 'all';
  const Sequence = 'sequence';
  const Choice = 'sequence';

  /**
   * The root schema file, it may include other schema files.
   * 
   * @var DOMDocument
   */
  protected $schema;

  /**
   * 
   */
  public function __construct(Schema $schema) {
    $this->schema = $schema;
  }

  /**
   *
   * @param type $path
   * @return type 
   */
  private function getElementDeclaration($path) {
    $element = $this->schema->query($path);
    if (count($element) != 1) {
      throw new Exception('The provided schema path did not select a unique node. Path: ' . $path);
    }
    return array_shift($element);
  }

  /**
   *
   * @param DOMElement $parent
   * @return array
   */
  private function getChildDOMElements(DOMElement $parent) {
    $xpath = new DOMXPath($parent->ownerDocument);
    return dom_node_list_to_array($xpath->query('child::*', $parent));
  }

  /**
   *
   * @param string $path
   * @param DOMElement $parent 
   */
  public function getNodeToInsertBefore($path, DOMElement $parent, DOMElement $child) {
    $insert_order = new InsertOrder($path);
    $children = $this->getChildDOMElements($parent);
    foreach ($children as $child) {
      if ($this->shouldInsertBefore()) {
        return $child;
      }
    }
    return NULL;
  }

  private function shouldInsertBefore($path, DOMElement $sibling) {
    
  }

  /**
   *
   * @param string $path
   * @return array
   */
  public function getInsertBeforeOrder($path) {
    /**
     * With "Any" order doesn't matter. "Any" can't be embedded in a 
     * sequence or choice, so no insert before order.
     */
    $element = $this->getElementDeclaration($path);
    if ($this->isType($this->getCompositor($element), self::All)) {
      return array();
    }
    return $this->getSiblings($element);
  }

  /**
   * Takes an <xs:element> declaration or reference and attempts to get its compositor.
   * 
   * @param DOMElement $element 
   * @return DOMElement
   */
  private function getCompositor(DOMElement $element) {
    if ($this->isCompositor($element->parentNode)) {
      return $element->parentNode;
    }
    return NULL;
  }

  /**
   *
   * @param DOMElement $node
   * @return array
   */
  private function getSiblings(DOMElement $node) {
    $output = array();
    // For each sibling
    $sibling = $node->nextSibling;
    while ($sibling) {
      if (is_type($sibling, 'DOMElement')) {
        $output = array_merge($output, $this->getSibling($sibling));
      }
      $sibling = $sibling->nextSibling;
    }
    // Go up the compositor chain, until we reach a parent that is not a compositor.
    if ($this->isCompositor($node->parentNode)) {
      $output = array_merge($output, $this->getSiblings($node->parentNode));
    }
    return $output;
  }

  /**
   *
   * @param DOMElement $sibling
   * @return array
   */
  private function getSibling(DOMElement $sibling) {
    $output = array();
    $type = $this->getType($sibling);
    switch ($type) {
      case self::Element:
        $output[] = $this->getNodeName($sibling);
        break;
      case self::ElementGroup:
        // Process element group
        break;
      case self::Choice:
      case self::Sequence:
        // Add first level elements and compositors. process compositors untill they don't have
        break;
    }
    return $output;
  }

}