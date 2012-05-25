<?php



/**
 * @file
 *
 * Models a <xs:complexType> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'ComplexContent');
module_load_include('inc', 'xml_schema_api', 'SimpleContent');
module_load_include('inc', 'xml_schema_api', 'All');
module_load_include('inc', 'xml_schema_api', 'Choice');
module_load_include('inc', 'xml_schema_api', 'Sequence');
module_load_include('inc', 'xml_schema_api', 'Group');

/**
 * Models a <xs:complexType> DOMElement.
 */
class XMLSchemaComplexType extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'complexType';

  /**
   *
   * @param DOMElement $node
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers(
        array(
      'complexContent' => $this->createChild(XMLSchemaComplexContent::LocalName),
      'simpleContent' => $this->createChild(XMLSchemaSimpleContent::LocalName),
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
   *
   * @param XMLSchemaInsertOrderMapNode $parent_node
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    $content = $this->getContent();
    if ($content) {
      return $content->asInsertOrderNode($parent);
    }
  }

  /**
   *
   * @return mixed
   */
  protected function getContent() {
    if (isset($this->complexContent)) {
      return $this->complexContent;
    }
    if (isset($this->simpleContent)) {
      return $this->simpleContent;
    }
    if (isset($this->all)) {
      return $this->all;
    }
    if (isset($this->choice)) {
      return $this->choice;
    }
    if (isset($this->sequence)) {
      return $this->sequence;
    }
    if (isset($this->group)) {
      return $this->group;
    }
    return NULL;
  }

}
