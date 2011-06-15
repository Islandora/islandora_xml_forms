<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:sequence> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'Node');
module_load_include('inc', 'schema_api', 'Element');

/**
 * Models a <xs:sequence> element.
 */
class XMLSchemaSequence extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'sequence';

  /**
   * 
   * @param DOMElement $node 
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->add(array(
      'element' => $this->createChildren(XMLSchemaElement::LocalName),
      'choice' => $this->createChildren(XMLSchemaChoice::LocalName),
      'sequence' => $this->createChildren(XMLSchemaSequence::LocalName),
      'group' => $this->createChildren(XMLSchemaGroup::LocalName),
      'children' => $this->createChildren() // All children in the correct order.
    ));
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
  public function asInsertOrderMapNode(XMLSchemaInsertOrderMapNode $parent = NULL) {
    list($min, $max) = $this->getMinMaxOccurs();
    $map_node = new XMLSchemaInsertOrderMapNode($min, $max, 'compositor', self::LocalName, $parent);
    foreach ($this->children as $child) {
      $map_node->children[] = $child->asInsertOrderMapNode($map_node);
    }
    return $map_node;
  }

}