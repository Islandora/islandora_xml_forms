<?php



/**
 * @file
 *
 * Models a <xs:restriction> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'All');
module_load_include('inc', 'xml_schema_api', 'Choice');
module_load_include('inc', 'xml_schema_api', 'Sequence');
module_load_include('inc', 'xml_schema_api', 'Group');

/**
 * Models a <xs:restriction> element.
 */
class XMLSchemaRestriction extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'restriction';

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
      'all' => $this->createChild(XMLSchemaAll::LocalName),
      'choice' => $this->createChild(XMLSchemaChoice::LocalName),
      'sequence' => $this->createChild(XMLSchemaSequence::LocalName),
      'group' => $this->createChild(XMLSchemaGroup::LocalName),
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
    return NULL;
  }

  public function restrictsComplexContent() {
    return $this->node->parentNode->localName == 'complexContent';
  }

  public function restrictsSimpleContent() {
    return $this->node->parentNode->localName == 'simpleContent';
  }

  public function restrictsSimpleType() {
    return $this->node->parentNode->localName == 'simpleType';
  }

  /**
   *
   * @param XMLSchemaInsertOrderMapNode $parent
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    if ($this->restrictsComplexContent()) {
      return $this->base->asInsertOrderNode($parent);
    }
    return NULL;
  }

}
