<?php

// $Id$

/**
 * @file
 */
module_load_include('inc', 'php_lib', 'ReadOnlyProtectedMembers');
module_load_include('inc', 'php_lib', 'DOMHelpers');

/**
 * Place for the common bits between element and attribute definitions.
 */
class NodeDefinition {

  /**
   * The xs:element, or xs:attribute this defintion is built from.
   * 
   * @var DOMElement 
   */
  protected $node;
  /**
   * Only defined if this element is a reference. In such cases some attribute defined in the reference take
   * precedent over attributes defined for the element.
   *
   * @var DOMElement 
   */
  protected $reference;
  /**
   * Cached result of the function ElementDefinition::isReference().
   * @var boolean
   */
  protected $isReference;
  /**
   * We will need to refer back to the parser for information.
   * 
   * @var SchemaParser 
   */
  protected $schemaParser;

  /**
   *
   * @param DOMElement $node
   *   Either an xs:element, or xs:attribute from which we will built a definition.
   */
  public function __construct($node) {
    $this->setDefinitionSource($node);
  }

  /**
   * Sets the source element and possibly its reference from which this definition will be built.
   *
   * @param DOMElement $element
   */
  private function setDefinitionSource($element) {
    if ($this->isReference($element)) {
      $this->reference = $element;
      $this->node = $this->schemaParser->findElementDefinition($this->reference);
    }
    else {
      $this->node = $element;
    }
  }

  /**
   * Check if this element is defined inline or a reference to another element.
   *
   * If this is the first time this function is called a parameter must be specified. After the first call to this
   * function the return value is cached for subsquent calls.
   *
   * @return boolean
   *   TRUE if this element is a reference false otherwise.
   */
  public function isReference($element = NULL) {
    if (empty($this->isReference)) {
      $this->isReference = $element->hasAttribute('ref');
    }
    return $this->isReference;
  }

}

/**
 *
 */
class ElementDefinition extends NodeDefinition {

  /**
   * The xs:element this defintion is built from. Is a referenc of $this->node.
   * 
   * @var DOMElement 
   */
  protected $element;

  /**
   *
   * @param DOMElement $element
   *   The xs:element this ElementDefintion will be
   *   Generated from.
   */
  public function __construct($element, $parser) {
    parent::__construct($element, $parser);
    $this->element = $this->node;
  }

  public function getName() {
    
  }

}

/**
 *
 */
class AttributeDefinition extends NodeDefinition {

  /**
   *
   * @var DOMElement
   */
  protected $attribute;

  /**
   *
   * @param DOMElement $attribute
   *   The xs:attribute element from which this class is built from.
   */
  public function __construct($attribute) {
    parent::__construct($attribute, $parser);
    $this->attribute = $this->node;
  }

}

/**
 * Ignoring imported documents for the most part as they are not used much in FGDC this will be an issue we will need
 * to address in the future.
 */
class Schema {

  protected $protected;
  protected $schema;
  protected $xpath;
  protected $includes; // Array of include Schemas
  protected $imports; // Array of imported Schemas
  protected static $schemas; // An array of all created schemas identified by file name.

  const prefix = 'xs';
  const uri = 'http://www.w3.org/2001/XMLSchema';

  public function __construct($filename, $namespace = NULL) {
    self::$schemas[$filename] = $this;
    $this->protected = new ReadOnlyProtectedMembers(array(
          'filename' => $filename,
          'location' => $this->getSchemaLocation($filename),
          'namespace' => $namespace));
    $this->loadSchema($filename);
  }

  public function __sleep() {
    return array('protected', 'includes', 'imports');
  }

  public function __wakeup() {
    $this->loadSchema($this->filename);
  }

  public function __get($name) {
    return $this->protected->$name;
  }

  public function __set($name, $value) {
    $this->protected->$name = $value;
  }

  private function getSchemaLocation($filename) {
    $index = strrpos($filename, '/');
    return substr($filename, 0, $index);
  }

  private function getNamespace($namespace = NULL) {
    if (empty($this->namespace)) {
      $this->namespace = $namespace; // For the moment this isn't really implemented.
    }
    return $this->namespace;
  }

  private function loadSchema($filename) {
    $this->schema = new DOMDocument();
    $this->schema->load($filename);
    $this->configureXPath();
    $this->loadIncludedFiles();
    $this->loadImportedFiles();
  }

  private function configureXPath() {
    $this->xpath = new DOMXPath($this->schema);
    $this->xpath->registerNamespace(self::prefix, self::uri);
    $this->registerNamespaces();
  }

  private function registerNamespaces() {
    $that = $this;
    $simple = simplexml_import_dom($this->schema->documentElement);
    $namespaces = $simple->getNamespaces(true);
    foreach ($namespaces as $prefix => $uri) {
      $this->xpath->registerNamespace($prefix, $uri);
    }
  }

  private function loadIncludedFiles() {
    $included_files = $this->getIncludedFiles();
    foreach ($included_files as $include_file) {
      $filename = "$this->location/$include_file";
      if (empty(self::$schemas[$filename])) {
        $this->includes[$filename] = new Schema($filename, $this->namespace); // Included files share the same namespace.
      }
      else {
        $this->includes[$filename] = self::$schemas[$filename];
      }
    }
  }

  private function getIncludedFiles() {
    $included_files = array();
    $result = $this->xpath->query('//xs:include');
    if ($result && $result->length > 0) {
      for ($i = 0; $i < $result->length; $i++) {
        $included_files[] = $result->item($i)->getAttribute('schemaLocation');
      }
    }
    return $included_files;
  }

  private function loadImportedFiles() {
    $imported_files = $this->getImportedFiles();
    foreach ($imported_files as $import_file) {
      list($location, $namespace) = $import_file;
      $filename = "$this->location/$location";
      if (empty(self::$schemas[$filename])) {
        $this->imports[$filename] = new Schema($filename, $namespace);
      }
      else {
        $this->imports[$filename] = self::$schemas[$filename];
      }
    }
  }

  private function getImportedFiles() {
    $imported_files = array();
    $result = $this->xpath->query('//xs:import');
    if ($result && $result->length > 0) {
      for ($i = 0; $i < $result->length; $i++) {
        $location = $result->item($i)->getAttribute('schemaLocation');
        $namespace = $result->item($i)->getAttribute('namespace');
        $imported_files[] = array($location, $namespace);
      }
    }
    return $imported_files;
  }

  public function query($path) {
    $results = dom_node_list_to_array($this->xpath->query($path));
    foreach ($this->includes as $schema) {
      $results = array_merge($results, $schema->query($path));
    }
    return $results;
  }

//  /**
//   * Gets a list of elements in order that this element at $path should appear before.
//   * 
//   * @param type $path 
//   */
//  private function getInsertBeforeOrder($path) {
//    $insert_before = array();
//    $element = $this->getUniqueNode($path);
//    list($compositor, $type) = $this->getCompositor($element);
//    if ($compositor_type == 'any') {
//      return array();
//    }
//    else if ($compositor_type == 'sequence') { // Get the elements in this sequence.
//      $insert_before = $this->getSiblingElements($element);
//    }
//
//    // Process my compositor.
//    // Process siblings
//    //  elements just go onto the array.
//    //  if sequence or choice get all possible outcomes, underneath them.
//    // Process the parent compositor 
//    // process siblings
//
//    if ($compositor_type == 'any') {
//      return array();
//    }
//    else if ($compositor_type == 'sequence') { // Get the elements in this sequence.
//      $insert_before = $this->getSiblingElements($element);
//    }
//    $insert_before = array_merge($insert_before, $this->_getInsertBeforeOrder($compositor));
//    return $insert_before;
//  }
//
//  /**
//   *
//   * @param DOMElement $element
//   * @return array
//   */
//  private function getSiblingElements(DOMElement $element) {
//    $output = array();
//    $next_sibling = $element->nextSibling;
//    while ($next_sibling) {
//      $output[] = $this->getElementName($next_sibling);
//      $next_sibling = $next_sibling->nextSibling;
//    }
//    return $output;
//  }
//
//  private function _getInsertBeforeOrder($compositor) {
//    $insert_before = array();
//    list($compositor, $type) = $this->getCompositor($compositor);
//    if ($compositor_type == 'sequence') {
//      $insert_before = $this->getSequenceElements($compositor);
//    }
//
//    return $insert_before;
//  }
//
//  private function getSequenceElements(DOMElement $sequence) {
//    $output = array();
//    $elements = dom_node_list_to_array($sequence->getElementsByTagNameNS(self::uri, 'element'));
//    foreach ($elements as $element) {
//      $output[] = $this->getElementName($element);
//    }
//    return $output;
//  }

  /**
   *
   * @param string $path
   * @param DOMElement $parent
   * @param DOMNode $child
   * @return DOMNode 
   */
  public function insertNode($path, DOMElement $parent, DOMNode $child) {
    $parser = new InsertOrderSchemaParser($this);
    $insert_order = $parser->getInsertBeforeOrder($path);
    foreach ($insert_order as $tag_name) {
      $children = dom_node_list_to_array($parent->getElementsByTagName($tag_name));
      if (count($children) >= 1) {
        $node = array_shift($children);
        return $parent->insertBefore($child, $node);
      }
    }
    return $parent->appendChild($child); // Insert at the end.
  }

  public function getAllElements() {
    $results = $this->query('//xs:element[@name]'); // 
  }

}

/**
 * @file
 *
 * Parses a given schema for the insert order of an element given at path.
 */
class InsertOrderSchemaParser {
  const ComplexType = 'complexType';
  const Element = 'element';
  const ElementGroup = 'group';
  const Attribute = 'attribute';
  const AttributeGroup = 'attributeGroup';
  const All = 'all';
  const Sequence = 'sequence';
  const Choice = 'sequence';

  /**
   * The root schema file, it may include other schema files.
   * 
   * @var DOMDocument
   */
  protected $schema;

  /**
   * 
   */
  public function __construct(Schema $schema) {
    $this->schema = $schema;
  }

  /**
   *
   * @param type $path
   * @return type 
   */
  private function getElementDeclaration($path) {
    $element = $this->schema->query($path);
    if (count($element) != 1) {
      throw new Exception('The provided schema path did not select a unique node. Path: ' . $path);
    }
    return array_shift($element);
  }

  /**
   *
   * @param string $path
   * @return array
   */
  public function getInsertBeforeOrder($path) {
    /**
     * With "Any" order doesn't matter. "Any" can't be embedded in a 
     * sequence or choice, so no insert before order.
     */
    $element = $this->getElementDeclaration($path);
    if ($this->isType($this->getCompositor($element), self::All)) {
      return array();
    }
    return $this->getSiblings($element);
  }

  /**
   *
   * @param DOMElement $node 
   */
  private function getType(DOMElement $node) {
    return $node->localName;
  }

  /**
   *
   * @param DOMElement $node
   * @param string $type
   * @return boolean
   */
  private function isType(DOMElement $node, $type) {
    return $this->getType($node) == $type;
  }

  /**
   *
   * @param DOMElement $node
   * @return boolean
   */
  private function isComplexType(DOMElement $node) {
    return $this->isType($node, self::ComplexType);
  }

  /**
   *
   * @param DOMElement $node 
   * @return boolean
   */
  private function isElement(DOMElement $node) {
    return $this->isType($node, self::Element);
  }

  /**
   *
   * @param DOMElement $node
   * @return boolean
   */
  private function isElementGroup(DOMElement $node) {
    return $this->isType($node, self::ElementGroup);
  }

  /**
   *
   * @param DOMElement $node 
   * @return boolean
   */
  private function isAttribute(DOMElement $node) {
    return $this->isType($node, self::Attribute);
  }

  /**
   *
   * @param DOMElement $node
   * @return boolean
   */
  private function isAttributeGroup(DOMElement $node) {
    return $this->isType($node, self::AttributeGroup);
  }

  /**
   *
   * @param DOMElement $node
   * @return boolean
   */
  private function isCompositor(DOMElement $node) {
    return $this->isType($node, self::All) ||
    $this->isType($node, self::Sequence) ||
    $this->isType($node, self::Choice);
  }

  /**
   * Takes an <xs:element> declaration or reference and attempts to get its compositor.
   * 
   * @param DOMElement $element 
   * @return DOMElement
   */
  private function getCompositor(DOMElement $element) {
    if ($this->isCompositor($element->parentNode)) {
      return $element->parentNode;
    }
    return NULL;
  }

  /**
   * Gets the name of a node, from either its name or reference attribute.
   * 
   * @param DOMElement $node
   * @return string
   */
  private function getNodeName(DOMElement $node) {
    if ($node->hasAttribute('name')) {
      return $node->getAttribute('name');
    }
    else if ($node->hasAttribute('ref')) {
      return $node->getAttribute('ref');
    }
    else {
      throw new Exception('Was unable to get the node name for the given schema element.');
    }
  }

  /**
   *
   * @param DOMElement $node 
   * @return array
   */
  private function getMinMaxOccurs(DOMElement $node) {
    $min = $node->hasAttribute('minOccurs') ? $node->getAttribute('minOccurs') : 1;
    $max = $node->hasAttribute('maxOccurs') ? $node->getAttribute('maxOccurs') : 1;
    return array($min, $max);
  }

  /**
   *
   * @param DOMElement $node
   * @return array
   */
  private function getSiblings(DOMElement $node) {
    $output = array();
    // For each sibling
    $sibling = $node->nextSibling;
    while ($sibling) {
      if (is_type($sibling, 'DOMElement')) {
        $output = array_merge($output, $this->getSibling($sibling));
      }
      $sibling = $sibling->nextSibling;
    }
    // Go up the compositor chain, until we reach a parent that is not a compositor.
    if ($this->isCompositor($node->parentNode)) {
      $output = array_merge($output, $this->getSiblings($node->parentNode));
    }
    return $output;
  }

  private function getSibling(DOMElement $sibling) {
    $output = array();
    $type = $this->getType($sibling);
    switch ($type) {
      case self::Element:
        $output[] = $this->getNodeName($sibling);
        break;
      case self::ElementGroup:
        // Process element group
        break;
      case self::Choice:
      case self::Sequence:
        // Add first level elements and compositors. process compositors untill they don't have
        break;
    }
    return $output;
  }

}