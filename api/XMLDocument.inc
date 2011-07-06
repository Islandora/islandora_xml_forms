<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'NodeRegistry');
module_load_include('inc', 'xml_schema_api', 'Schema');
module_load_include('inc', 'php_lib', 'String');

class XMLDocument {
  /**
   * Constants
   */
  const xmlns = 'http://www.w3.org/2000/xmlns/';

  /**
   * The xml string that this document was created from. It can be NULL.
   * 
   * @var string
   */
  protected $xml;
  /**
   * A array of properties related to the root element of the document.
   * 
   * $root['local_name'] = The local name of the root element;
   * $root['prefix'] = The namespace prefix of the root element;
   * $root['uri'] = The local name of the root element;
   * 
   * @var array
   */
  protected $root;
  /**
   * A map of the namespaces used in the generated XML document.
   * 
   * array('prefix' => 'uri');
   * 
   * @var array 
   */
  protected $namespaces;
  /**
   * The uri to the schema.
   * 
   * @var string
   */
  public $schema;
  /**
   * Stores associations between form elements and DOMNodes.
   * 
   * @var NodeRegisty
   */
  public $registry;
  /**
   * 
   * @var DOMDocument
   */
  public $document;
  /**
   * 
   * @var DOMXPath
   */
  protected $xpath;

  /**
   * Create an XMLDocument.
   * 
   * @param array $root
   * @param array $namespaces
   * @param string $schema
   * @param string $xml 
   */
  public function __construct(array &$root, array &$namespaces, $schema, &$xml = NULL) {
    $this->root = $root;
    $this->namespaces = $namespaces;
    try {
      if (is_non_empty_string($schema)) {
        $this->schema = new XMLSchema($schema);
      }
    } catch (Exception $e) {
      $this->schema = NULL;
      drupal_set_message($e->getMessage(), 'error');
    }
    $this->xml = $xml;
    $this->registry = new NodeRegistry($this);
    $this->document = $this->createDocument();
    $this->xpath = new DOMXPath($this->document);
    $this->registerNamespaces();
  }

  /**
   * Determine what member variables to serialize. 
   * 
   * DOMDocument's and DOMNodes can not be serialized.
   * 
   * @return array
   */
  public function __sleep() {
    return array('root', 'namespaces', 'schema', 'xml', 'registry');
  }

  /**
   * Reinitialize this object after it has be unserialized.
   */
  public function __wakeup() {
    $this->document = $this->createDocument();
    $this->xpath = new DOMXPath($this->document);
    $this->registerNamespaces();
    $this->registry->wakeup();
  }

  /**
   *
   * @return DOMDocument 
   */
  private function createDocument() {
    if ($this->exists()) {
      return $this->createFromExistingDocument();
    }
    $document = new DOMDocument();
    $root = $document->appendChild($this->createRootElement());
    foreach ($this->namespaces as $prefix => $uri) {
      $prefix = preg_match('/xmlns/', $prefix) ? $prefix : "xmlns:$prefix";
      $root->setAttributeNS(self::xmlns, $prefix, $uri);
    }
    return $document;
  }

  /**
   * 
   */
  private function createFromExistingDocument() {
    $doc = new DOMDocument();
    $doc->loadXML($this->xml);
    if ($this->schema) {
      $doc->schemaValidate($this->schema->filename);
    }
    return $doc;
  }

  /**
   * Creates the root element of the document.
   * 
   * @return DOMElement 
   */
  private function createRootElement() {
    $prefix = $this->root['prefix'];
    $name = $this->root['local_name'];
    $uri = $this->root['uri'];
    $has_prefix = isset($prefix) && $prefix != '';
    $name = $has_prefix ? "{$prefix}:{$name}" : $name;
    $uri = $uri != '' ? $uri : NULL;
    if (empty($name) && $name == '') {
      throw new Exception('Missing local name for the root element in the form properties.');
    }
    return new DOMElement($name, NULL, $uri);
  }

  /**
   * Register the namespaces for XPath to work correctly.
   */
  private function registerNamespaces() {
    foreach ($this->namespaces as $prefix => $uri) {
      $this->xpath->registerNamespace($prefix, $uri);
    }
  }

  /**
   * Checks to see if this document existed before this clas
   * 
   * @return boolean
   */
  public function exists() {
    return is_non_empty_string($this->xml);
  }

  /**
   * Get the namespace URI for the given prefix.
   * 
   * @param string $prefix 
   * @return string
   */
  public function getNamespace($prefix) {
    return isset($this->namespaces[$prefix]) ? $this->namespaces[$prefix] : NULL;
  }

  /**
   * Converts the document to an XML string.
   * 
   * @return string 
   */
  public function saveXML() {
    return $this->document->saveXML();
  }

  /**
   * Query the document.
   * 
   * @param string $path
   *   XPath string.
   * @param DOMNode $context
   *   Context node, if NULL the context of the path will be the entire document.
   * 
   * @return DOMNodeList 
   */
  public function query($path, DOMNode $context = NULL) {
    $results = isset($context) ?
        $this->xpath->query($path, $context) :
        $this->xpath->query($path);
    return $results;
  }

}