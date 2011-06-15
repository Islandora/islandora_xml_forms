<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:restriction> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'All');
module_load_include('inc', 'schema_api', 'Choice');
module_load_include('inc', 'schema_api', 'Sequence');
module_load_include('inc', 'schema_api', 'Group');

/**
 * Models a <xs:restriction> element.
 */
class XMLSchemaRestriction extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'restriction';

  /**
   * 
   * @param DOMElement $node 
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->add(
        array(
          'all' => $this->createChild(XMLSchemaAll::LocalName),
          'choice' => $this->createChild(XMLSchemaChoice::LocalName),
          'sequence' => $this->createChild(XMLSchemaSequence::LocalName),
          'group' => $this->createChild(XMLSchemaGroup::LocalName),
        )
    );
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