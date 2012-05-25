<?php



/**
 * @file
 *
 * Models a <xs:group> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_schema_api', 'Node');
module_load_include('inc', 'xml_schema_api', 'ComplexType');
module_load_include('inc', 'xml_schema_api', 'SimpleType');

/**
 * Models a <xs:group> DOMElement.
 */
class XMLSchemaGroup extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'group';

  /**
   *
   * @param DOMElement $node
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    parent::__construct($schema, $node);
    $this->protected->addMembers(
        array(
      'all' => $this->createType(XMLSchemaAll::LocalName),
      'choice' => $this->createType(XMLSchemaChoice::LocalName),
      'sequence' => $this->createType(XMLSchemaSequence::LocalName),
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
   * @param string $local_name
   * @return XMLSchemaNode
   */
  private function createType($local_name) {
    $group = $this->schema->findGroup($this->getName);
    $children = dom_node_filter_children($group, 'DOMElement', 'localName', $local_name);
    if (count($children) == 1) {
      $child = array_shift($children);
      return $this->createNodeTypedObject($child);
    }
  }

  /**
   *
   * @param XMLSchemaInsertOrderMapNode $parent
   */
  public function asInsertOrderNode(InsertOrderNode $parent = NULL) {
    $content = $this->getContent();
    if ($content) {
      return $content->asInsertOrderNode($parent);
    }
  }

  /**
   *
   * @return
   */
  private function getContent() {
    if (isset($this->all)) {
      return $this->all;
    }
    if (isset($this->choice)) {
      return $this->choice;
    }
    if (isset($this->sequence)) {
      return $this->sequence;
    }
    return NULL;
  }

}
