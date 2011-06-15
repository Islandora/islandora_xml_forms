<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:all> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'Node');
module_load_include('inc', 'schema_api', 'Element');

/**
 * Models a <xs:all> element.
 */
class XMLSchemaAll extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'all';

  /**
   * 
   * @param DOMElement $node 
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->add('element', $this->createChildren(XMLSchemaElement::LocalName));
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
   * @param XMLSchemaInsertOrderMapNode $parent 
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    
  }
}