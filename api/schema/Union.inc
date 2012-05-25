<?php



/**
 * @file
 *
 * Models a <xs:union> element.
 */
module_load_include('inc', 'xml_schema_api', 'Node');

/**
 * Models a <xs:union> DOMElement.
 */
class XMLSchemaUnion extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'union';

  /**
   *
   * @param DOMElement $node
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
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
    return NULL;
  }

}
