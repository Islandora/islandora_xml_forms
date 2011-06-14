<?php

// $Id$

/**
 * @file
 */
module_load_include('inc', 'php_lib', 'ReadOnlyProtectedMembers');
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'xml_form_api', 'SchemaTag');

class XSComplexContent {
  const TagName = 'complexContent';
}



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
  /**
   * Const's
   */
  const prefix = 'xs';
  const uri = 'http://www.w3.org/2001/XMLSchema';

  /**
   *
   * @var ReadOnlyProtected
   */
  protected $protected;
  /**
   *
   * @var DOMDocument
   */
  protected $schema;
  /**
   *
   * @var DOMXPath
   */
  protected $xpath;
  /**
   *
   * @var Schema[]
   */
  protected $includes; // Array of include Schemas
  /**
   *
   * @var Schema[]
   */
  protected $imports; // Array of imported Schemas
  /**
   * An array of all created schemas identified by file name.
   * 
   * @var Schema[]
   */
  protected static $schemas;

  /**
   *
   * @param string $filename
   * @param string $namespace 
   */
  public function __construct($filename, $namespace = NULL) {
    self::$schemas[$filename] = $this;
    $this->protected = new ReadOnlyProtectedMembers(array(
          'filename' => $filename,
          'location' => $this->getSchemaLocation($filename),
          'namespace' => $namespace));
    $this->loadSchema($filename);
  }

  /**
   *
   * @return array
   */
  public function __sleep() {
    return array('protected', 'includes', 'imports');
  }

  /**
   * 
   */
  public function __wakeup() {
    $this->loadSchema($this->filename);
  }

  /**
   *
   * @param string $name
   * @return mixed
   */
  public function __get($name) {
    return $this->protected->$name;
  }

  /**
   *
   * @param string $name
   * @param mixed $value 
   */
  public function __set($name, $value) {
    $this->protected->$name = $value;
  }

  /**
   *
   * @param string $filename
   * @return string
   */
  private function getSchemaLocation($filename) {
    $index = strrpos($filename, '/');
    return substr($filename, 0, $index);
  }

  /**
   *
   * @param type $namespace
   * @return type 
   */
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
    $this->includes = array();
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
    $this->imports = array();
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

  /**
   * 
   */
  public function getUniqueNode($path) {
    $results = $this->query($path);
    if (count($results) != 1) { // Not unique
      throw new Exception('Node at path was not unique. Path: ' . $path);
    }
    return array_shift($results);
  }

  /**
   *
   * @param string $path
   * @param DOMElement $parent
   * @param DOMNode $child
   * @return DOMNode 
   */
  public function insertNode($path, DOMElement $parent, DOMNode $child) {
    $parser = new InsertOrderSchemaParser($this);
    $insert_before = $parser->getNodeToInsertBefore($path, $parent);
    if ($insert_before) {
      return $parent->insertBefore($child, $insert_before);
    }
    else {
      return $parent->appendChild($child); // Insert at the end. Allows for out of order building, its assumed that following elements will be placed after this to create the correct ordering.
    }
  }

  public function getAllElements() {
    $results = $this->query('//xs:element[@name]'); // 
  }

}

/**
 * 
 */
class InsertOrder {

  /**
   *
   * @var Schema
   */
  protected $schema;
  /**
   *
   * @var DOMElement
   */
  protected $element;
  /**
   *
   * @var DOMElement
   */
  protected $reference;
  /**
   * 
   */
  protected $type;

  /**
   *
   * @param Schema $schema
   * @param string $path 
   */
  public function __construct(Schema $schema, $path, DOMElement $parent) {
    $this->schema = $schema;
    $this->path = $path;
    $this->parent = $parent;
    $this->element = $this->getElementDeclaration($path);
    if (SchemaTag::isReference($this->element)) {
      $this->reference = $this->schema->getUniqueNode("/xs:schema/xs:element[@name='{$parent->localName}']");
    }

    // Get type definition.
    if (isset($this->reference) && SchemaTag::hasType($node)) {
      // Get type..
    }
    else {
      // Type is inline...
    }
    // Now that we have the type, see if it's an extension.
    // Build rules...
  }

  /**
   * Get the element declaration.
   * 
   * @param string $path
   * @return DOMElement
   */
  private function getElementDeclaration($path) {
    $results = $this->schema->query($path);
    if (count($results) != 1) {
      throw new Exception('The provided schema path did not select a unique node. Path: ' . $path);
    }
    $element = array_shift($results);
    if (!SchemaTag::isElement($element)) {
      throw new Exception('The given path does not select an element type. Path: ' . $path);
    }
    return $element;
  }

}

class Blah {

  public $name;
  public $type; // Element Any 
  public $min;
  public $max;

  public function __construct(DOMElement $node) {
    
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
  const Any = 'any';
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
   * @param DOMElement $parent
   * @return array
   */
  private function getChildDOMElements(DOMElement $parent) {
    $xpath = new DOMXPath($parent->ownerDocument);
    return dom_node_list_to_array($xpath->query('child::*', $parent));
  }

  /**
   *
   * @param string $path
   * @param DOMElement $parent 
   */
  public function getNodeToInsertBefore($path, DOMElement $parent, DOMElement $child) {
    $insert_order = new InsertOrder($path);
    $children = $this->getChildDOMElements($parent);
    foreach ($children as $child) {
      if ($this->shouldInsertBefore()) {
        return $child;
      }
    }
    return NULL;
  }

  private function shouldInsertBefore($path, DOMElement $sibling) {
    
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

  /**
   *
   * @param DOMElement $sibling
   * @return array
   */
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