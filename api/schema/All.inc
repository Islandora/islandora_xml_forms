<?php



/**
 * @file
 *
 * Models a <xs:all> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'Element');

/**
 * Models a <xs:all> element.
 */
class XMLSchemaAll extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'all';

  /**
   *
   * @param DOMElement $node
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers('element', $this->createChildren(XMLSchemaElement::LocalName));
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
    list($min, $max) = $this->getMinMaxOccurs();
    $map_node = new InsertOrderAll($min, $max, $parent);
    foreach ($this->children as $child) {
      $map_node->addChild($child->asInsertOrderNode($map_node));
    }
    return $map_node;
  }

}
