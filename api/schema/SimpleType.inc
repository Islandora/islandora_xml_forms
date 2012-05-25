<?php



/**
 * @file
 *
 * Models a <xs:simpleType> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'List');
module_load_include('inc', 'xml_schema_api', 'Union');
module_load_include('inc', 'xml_schema_api', 'Restriction');

/**
 * Models a <xs:simpleType> DOMElement.
 */
class XMLSchemaSimpleType extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'simpleType';

  /**
   *
   * @param DOMElement $node
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers(
        array(
      'restriction' => $this->createChild(XMLSchemaRestriction::LocalName),
      'list' => $this->createChild(XMLSchemaList::LocalName),
      'union' => $this->createChild(XMLSchemaUnion::LocalName),
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
    return NULL;
  }

}
