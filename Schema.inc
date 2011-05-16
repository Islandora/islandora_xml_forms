<?php

// $Id$

/**
 * @file
 */

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

  protected $filename;
  protected $location;
  protected $schema;
  protected $xpath;
  protected $includes; // Array of include Schemas
  protected $imports; // Array of imported Schemas
  protected static $schemas; // An array of all created schemas identified by file name.

  const prefix = 'xs';
  const uri = 'http://www.w3.org/2001/XMLSchema';

  public function __construct($filename, $namespace = NULL) {
    $this->filename = $filename;
    $this->location = $this->getSchemaLocation();
    $this->namespace = $this->getNamespace($namespace);
    self::$schemas[$this->filename] = $this;
    $this->loadSchema();
    //var_dump($filename);
  }

  private function getSchemaLocation() {
    if (empty($this->location)) {
      $index = strrpos($this->filename, '/');
      $this->location = substr($this->filename, 0, $index);
    }
    return $this->location;
  }

  private function getNamespace($namespace = NULL) {
    if (empty($this->namespace)) {
      $this->namespace = $namespace; // For the moment this isn't really implemented.
    }
    return $this->namespace;
  }

  private function loadSchema() {
    $this->schema = new DOMDocument();
    $this->schema->load($this->filename);
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
    $thing = $this;
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

  private function query($expression) {
    $results = $this->domNodeListToArray($this->xpath->query($expression));
    foreach ($this->includes as $schema) {
      $results = array_merge($results, $schema->query($expression));
    }
    return $results;
  }

  public function getAllElements() {
    $results = $this->query('/xs:schema/xs:element[@name]');
    array_walk($results, array($this, 'foo'));
    //var_dump($results);
  }


  private function domNodeListToArray($node_list) {
    $result = array();
    if ($node_list) {
      $length = $node_list->length;
      for ($i = 0; $i < $length; $i++) {
        $result[] = $node_list->item($i);
      }
    }
    return $result;
  }

}

/**
 * @file
 *
 */
class SchemaParser {

  /**
   * The root schema file, it may include other schema files.
   * 
   * @var DOMDocument
   */
  protected $schema;
  /**
   * An array of all the included schema's.
   * @var array
   */
  protected $included;
  protected $xpaths;
  protected $pathToRootSchema;
  protected $prefix = 'xs';
  protected $uri = 'http://www.w3.org/2001/XMLSchema';

  /**
   *
   */
  public function __construct($filename) {
    // Hardcoded Hack...
    $path = drupal_get_path('module', 'islandora_form_builder');
    $this->pathToRootSchema = "$path/schemas/fgdc/fgdc-std-012-2002";
    $filename = "$path/schemas/fgdc/fgdc-std-012-2002/fgdc-std-012-2002.xsd";
    $this->schema = new Schema($filename);
    $this->schema->getAllElements();
  }

}