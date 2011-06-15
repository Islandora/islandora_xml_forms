<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:group> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'Node');
module_load_include('inc', 'schema_api', 'ComplexType');
module_load_include('inc', 'schema_api', 'SimpleType');

/**
 * Models a <xs:group> DOMElement.
 */
class XMLSchemaGroup extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'group';

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

}