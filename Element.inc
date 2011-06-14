<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:element> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'Node');
module_load_include('inc', 'schema_api', 'ComplexType');
module_load_include('inc', 'schema_api', 'SimpleType');

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
   * @param DOMElement $node 
   */
  public function __construct(DOMElement $node) {
    parent::__construct($node);
    $this->protected->add(
        array(
          'complexType' => $this->createChild(XMLSchemaComplexType::LocalName),
          'simpleType' => $this->createChild(XMLSchemaSimpleType::LocalName),
        //'key' => @todo Implement...
        //'keyRef' => @todo Implement...
        //'unique' => @todo Implement...
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
   * Is a reference?
   * 
   * @return boolean
   */
  public function isReference() {
    return $this->node->hasAttribute('ref');
  }

  /**
   * Has type
   * 
   * @return boolean
   */
  public function hasType() {
    return $this->node->hasAttribute('type');
  }

}