<?php



/**
 * @file
 *
 * Models a <xs:extension> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema  _api', 'Any');
module_load_include('inc', 'xml_schema_api', 'Choice');
module_load_include('inc', 'xml_schema_api', 'Sequence');
module_load_include('inc', 'xml_schema_api', 'Group');

/**
 * Models a <xs:extension> element.
 */
class XMLSchemaExtension extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'extension';

  /**
   *
   * @param DOMElement $node
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $base = $node->getAttribute('base');
    $this->protected->addMembers(
        array(
      'base' => $this->findExtension($base),
      'choice' => $this->createChild(XMLSchemaChoice::LocalName),
      'sequence' => $this->createChild(XMLSchemaSequence::LocalName),
      'group' => $this->createChild(XMLSchemaGroup::LocalName),
      'children' => $this->createChildren(), // All children in the correct order.
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
   * @param string $base
   * @return $complexType.
   */
  private function findExtension($base) {
    if ($base == 'xs:anyType') {
      return new XMLSchemaAny($this->schema);
    }
    else {
      $node = $this->schema->findGlobalType($base);
      return new XMLSchemaComplexType($this->schema, $node);
    }
  }

  public function extendsComplexContent() {
    return $this->node->parentNode->localName == 'complexContent';
  }

  public function extendsSimpleContent() {
    return $this->node->parentNode->localName == 'simpleContent';
  }

  /**
   *
   * @param XMLSchemaInsertOrderMapNode $parent
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    if ($this->extendsComplexContent()) {
      list($min, $max) = $this->getMinMaxOccurs();
      $map_node = new InsertOrderSequence($min, $max, $parent);
      $map_node->addChild($this->base->asInsertOrderNode()); // Base class.
      foreach ($this->children as $child) {
        $map_node->addChild($child->asInsertOrderNode($map_node));
      }
      return $map_node;
    }
    return NULL;
  }

}
