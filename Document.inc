<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'Utils');

class DocumentProperties {

  /**
   *
   * @var string 
   */
  public $root_name;
  public $root_prefix;
  public $root_uri;
  /**
   * A map of the namespaces used in the generated XML document. In the form of:
   *  array('prefix' => 'uri');
   * 
   * @var array 
   */
  public $namespaces;
  /**
   * URI to the schema.
   * 
   * @var string
   */
  public $schema;

  /**
   *
   * @param SimpleXMLElement $properties 
   */
  public function __construct(SimpleXMLElement $properties) {
    $this->root_name = (string) $properties->root['local_name'];
    $this->root_prefix = (string) $properties->root['prefix'];
    $this->root_uri = (string) $properties->root['uri'];
    if (isset($properties->schema)) {
      $this->schema = (string) $properties->schema;
    }
    if (isset($properties->namespaces)) {
      $namespaces = $properties->namespaces->children();
      foreach ($namespaces as $namespace) {
        $prefix = (string) $namespace['prefix'];
        $uri = (string) $namespace;
        $prefix = $prefix == '' ? 'default' : $prefix;
        $this->namespaces[$prefix] = $uri;
      }
    }
  }

  public function getNamespaces() {
    
  }

  public function getNamespace($prefix) {
    
  }

}

/**
 * 
 */
class Document {

  /**
   * 
   * @var DocumentProperties 
   */
  protected $properties;
  /**
   *
   * @var string 
   */
  protected $xml;
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
   *
   * @param XMLDocumentProperties $document_properties
   * @param string $xml 
   */
  public function __construct(DocumentProperties $document_properties, &$xml = NULL) {
    $this->properties = $document_properties;
    $this->xml = $xml;
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
    return array('properties', 'xml');
  }

  /**
   * Reinitialize this object after it has be unserialized.
   */
  public function __wakeup() {
    $this->document = $this->createDocument();
    $this->xpath = new DOMXPath($this->document);
    $this->registerNamespaces();
  }

  /**
   *
   * @return DOMDocument 
   */
  private function createDocument() {
    if ($this->exists()) {
      return $this->createFromExistingDocument();
    }
    else {
      $doc = new DOMDocument();
      $doc->appendChild($this->createRootElement());
      return $doc;
    }
  }

  /**
   * 
   */
  private function createFromExistingDocument() {
    $doc = new DOMDocument();
    $doc->loadXML($this->xml);
    $schema = $this->properties->schema;
    if ($schema) {
      if (!$doc->schemaValidate($schema)) {
        throw new Exception('Can not create a document from an existing invalid document.');
      }
    }
    return $doc;
  }

  /**
   *
   * @return DOMElement 
   */
  private function createRootElement() {
    $properties = $this->properties;
    $name = $properties->root_prefix ? "{$properties->root_prefix}:{$properties->root_name}" : $properties->root_name;
    return new DOMElement($name, NULL, $properties->root_uri);
  }

  /**
   * 
   */
  private function registerNamespaces() {
    $namespaces = $this->properties->namespaces;
    foreach ($namespaces as $prefix => $uri) {
      $this->xpath->registerNamespace($prefix, $uri);
    }
  }

  /**
   *
   * @return boolean
   */
  public function exists() {
    return isset($this->xml);
  }

  /**
   *
   * @param string $prefix 
   * @return string
   */
  public function getNamespace($prefix) {
    $this->properties->namespaces[$prefix];
  }

  /**
   *
   * @return string 
   */
  public function asXML() {
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
    $results = $context ?
        $this->xpath->query($path, $context) :
        $this->xpath->query($path);
    return $results;
  }

}