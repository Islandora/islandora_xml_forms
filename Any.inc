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
class XMLSchemaAny extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'element';

  /**
   * 
   * @param DOMElement $node 
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->add(
        array(
          'complexType' => $this->createType(XMLSchemaComplexType::LocalName),
          'simpleType' => $this->createType(XMLSchemaSimpleType::LocalName),
        //'key' => @todo Implement...
        //'keyRef' => @todo Implement...
        //'unique' => @todo Implement...
        )
    );
  }

  /**
   *
   * @param string $local_name
   * @return XMLSchemaNode
   */
  private function createType($local_name) {
    $node = $this->isTypeInline() ?
        $this->findChild($local_name) :
        $this->schema->findGlobalType($local_name, $this->getTypeName());
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
  public function asInsertOrderMapNode(XMLSchemaInsertOrderMapNode $parent = NULL) {
    list($min, $max) = $this->getMinMaxOccurs();
    return new XMLSchemaInsertOrderMapNode($min, $max, 'element', $this->getName(), $parent);
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
   * 
   * @return string
   */
  public function getTypeName() {
    return $this->node->getAttribute('type');
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

  /**
   * Has type
   * 
   * @return boolean
   */
  public function isTypeInline() {
    return $this->getTypeName() == '';
  }

}