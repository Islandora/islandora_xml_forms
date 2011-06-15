<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:complexContent> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'Extension');
module_load_include('inc', 'schema_api', 'Restriction');

/**
 * Models a <xs:complexContent> element.
 */
class XMLSchemaComplexContent extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'complexContent';

  /**
   * 
   * @param DOMElement $node 
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->add(
        array(
          'extension' => $this->createChild(XMLSchemaExtension::LocalName),
          'restriction' => $this->createChild(XMLSchemaRestriction::LocalName)
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