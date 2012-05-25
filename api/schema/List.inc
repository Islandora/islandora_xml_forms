<?php



/**
 * @file
 *
 * Models a <xs:list> element.
 */
module_load_include('inc', 'xml_schema_api', 'Node');

/**
 * Models a <xs:list> DOMElement.
 */
class XMLSchemaList extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'list';

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
