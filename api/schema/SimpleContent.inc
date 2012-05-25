<?php



/**
 * @file
 *
 * Models a <xs:simpleContent> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Extension');
module_load_include('inc', 'xml_schema_api', 'Restriction');

/**
 * Models a <xs:simpleContent> element.
 */
class XMLSchemaSimpleContent extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'simpleContent';

  /**
   *
   * @param DOMElement $node
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers(
        array(
      'extension' => $this->createChild(XMLSchemaExtension::LocalName),
      'restriction' => $this->createChild(XMLSchemaRestriction::LocalName),
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
