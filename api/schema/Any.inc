<?php



/**
 * @file
 *
 * Models a <xs:element> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'ComplexType');
module_load_include('inc', 'xml_schema_api', 'SimpleType');

/**
 * Models a <xs:element> DOMElement.
 */
class XMLSchemaAny extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'any';

  /**
   *
   * @param DOMElement $node
   */
  public function __construct(XMLSchema $schema, DOMElement $node = NULL) {
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
   * @return  array
   */
  public function getValidNames() {
    $names = array();
    // @todo implement!
    return $names;
  }

  /**
   *
   * @param XMLSchemaInsertOrderMapNode $parent_node
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    $names = $this->getValidNames();
    list($min, $max) = $this->getMinMaxOccurs();
    return new InsertOrderAny($names, $min, $max, $parent);
  }

}
