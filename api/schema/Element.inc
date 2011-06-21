<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:element> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'ComplexType');
module_load_include('inc', 'xml_schema_api', 'SimpleType');

/**
 * Models a <xs:element> DOMElement.
 */
class XMLSchemaElement extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'element';

  /**
   *
   * @var DOMElement
   */
  protected $definition; // If $node is a reference then this is the DOMElement $node references, otherwise it is equivalent to $node.
  /**
   *
   * @var DOMElement
   */
  protected $reference; // The DOMElement $node reference's if it exists.

  /**
   * 
   * @param DOMElement $node 
   */

  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $xml = $node->ownerDocument->saveXML($node);
    $type = $this->createType();
    $this->protected->add('type', $type);
    if ($type instanceof XMLSchemaComplexType) {
      $this->protected->add('complexType', $type);
    }
    else if ($type instanceof XMLSchemaSimpleType) {
      $this->protected->add('simpleType', $type);
    }
    /*
    $this->protected->add(
        array(
        //'key' => @todo Implement...
        //'keyRef' => @todo Implement...
        //'unique' => @todo Implement...
        )
    );*/
  }

  /**
   *
   * @return XMLSchemaNode
   */
  private function createType() {
    $node = $this->getType();
    return isset($node) ? $this->createNodeTypedObject($node) : NULL;
  }

  /**
   * Returns the local name of this object.
   * 
   * @return string
   */
  protected function getLocalName() {
    return self::LocalName;
  }

  /**
   *
   * @param XMLSchemaInsertOrderMapNode $parent_node 
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    list($min, $max) = $this->getMinMaxOccurs();
    return new InsertOrderElement($this->getName(), $min, $max, $parent);
  }

  /**
   * 
   * @return DOMElement
   */
  public function getDefinition() {
    if (empty($this->definition)) {
      $this->definition = $this->isReference() ?
          $this->getReference() :
          $this->node;
    }
    return $this->definition;
  }

  /**
   * Is a reference?
   * 
   * @return boolean
   */
  public function isReference() {
    return $this->node->hasAttribute('ref');
  }

  /**
   * Gets the element this element refer's to.
   * 
   * @return DOMElement
   */
  public function getReference() {
    if (empty($this->reference) && $this->isReference()) {
      $name = $this->node->getAttribute('ref');
      $this->reference = $this->schema->findGlobalElement($name);
    }
    return $this->reference;
  }

  /**
   *
   * @return boolean
   */
  public function isTyped() {
    return $this->getDefinition()->hasAttribute('type');
  }

  /**
   * 
   * 
   * @return boolean
   */
  public function isAnonymous() {
    return $this->isTyped() == FALSE;
  }

  /**
   * @return DOMElement
   */
  public function getType() {
    if ($this->isAnonymous()) {
      $child = dom_node_find_child($this->node, 'DOMElement', 'localName', 'complexType');
      if (empty($child)) {
        $child = dom_node_find_child($this->node, 'DOMElement', 'localName', 'simpleType');
      }
      return $child; // if key, keyRef, unique, return NULL...
    }
    else {
      $type_name = $this->getDefinition()->getAttribute('type');
      return $this->schema->findGlobalType($type_name);
    }
  }

  /**
   * 
   * @return string
   */
  public function getName() {
    if ($this->isReference()) {
      return $this->node->getAttribute('ref');
    }
    else {
      return $this->node->getAttribute('name');
    }
  }

}