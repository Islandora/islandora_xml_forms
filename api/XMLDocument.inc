<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'NodeRegistry');
module_load_include('inc', 'xml_form_api', 'Namespaces');
module_load_include('inc', 'xml_schema_api', 'Schema');
module_load_include('inc', 'php_lib', 'String');

class XMLDocument {

  /**
   * When this object is put to sleep, the current state of the document is stored in this variable.
   * 
   * @var string
   */
  protected $xml;

  /**
   * A string that defines the name of the root node.
   * 
   * @var string
   */
  protected $root_name;

  /**
   * All of the namespaces required for the document.
   * 
   * @var Namespaces 
   */
  public $namespaces;

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
   * The document this object repersents. When this object is serialized the document is stored
   * as a string in the member variable $xml.
   * 
   * @var DOMDocument
   */
  public $document;

  /**
   * The xpath used to query the document.
   * 
   * @var DOMXPath
   */
  protected $xpath;

  /**
   * Create an XMLDocument.
   * 
   * @param string $root_name
   * @param array $namespaces
   * @param string $schema
   * @param string $xml 
   */
  public function __construct($root_name, Namespaces $namespaces, $schema_uri, $xml = NULL) {
    $this->namespaces = $namespaces;
    $this->schema = $this->createSchema($schema_uri);
    $this->document = $this->createDocument($root_name, $namespaces, $xml);
    $this->xpath = new DOMXPath($this->document);
    $this->registry = new NodeRegistry();
  }

  /**
   * Determine what member variables to serialize. 
   * 
   * DOMDocument's and DOMNodes can not be serialized.
   * 
   * @return array
   */
  public function __sleep() {
    $this->xml = $this->document->saveXML();
    return array('namespaces', 'schema', 'registry', 'xml');
  }

  /**
   * Reinitialize this object after it has be unserialized.
   */
  public function __wakeup() {
    $this->document = new DOMDocument();
    $this->document->loadXML($this->xml);
    $this->xpath = new DOMXPath($this->document);
    $this->registry->restore($this->document);
  }

  /**
   * Creates a XMLSchema object.
   * 
   * @param string $schema
   *   The URI to the schema. Maybe null or an empty string.
   * 
   * @return XMLSchema 
   */
  private function createSchema($schema) {
    if (is_non_empty_string($schema)) {
      try {
        return new XMLSchema($schema);
      } catch (Exception $e) {
        drupal_set_message($e->getMessage(), 'error');
      }
    }
    return NULL;
  }

  /**
   * Creates a DOMDocument either from a provided xml string or from scratch.
   * 
   * @param string $root_name
   * @param Namespaces $default_uri
   * @param string $xml
   * 
   * @return DOMDocument
   */
  private function createDocument($root_name, Namespaces $namespaces, $xml) {
    $document = new DOMDocument();
    isset($xml) ? // Load from XML or create the root node.
            $document->loadXML($xml) :
            $document->appendChild(new DOMElement($root_name, NULL, $namespaces->getDefaultURI()));
    $this->namespaces->addNamespacesAttributesTo($document->documentElement);
    return $document;
  }

  /**
   * Get the namespace URI for the given prefix.
   * 
   * @param string $prefix 
   * @return string
   */
  public function getNamespaceURI($prefix = NULL) {
    if (isset($prefix)) {
      isset($this->namespaces[$prefix]) ? $this->namespaces[$prefix] : NULL;
    }
    return $this->defaultURI;
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
    libxml_use_internal_errors(TRUE); // Prevent Drupal from catching the error.  
    $results = isset($context) ? $this->xpath->query($path, $context) : $this->xpath->query($path);
    $error = FALSE;
    $error = libxml_get_last_error();
    libxml_use_internal_errors(FALSE); // Allow Drupal to catch errors
    $failed = ($results === FALSE) || ($error !== FALSE);
    if ($failed) {
      $msg = ($results === FALSE) ?
          "XPath query failed, either the expression is malformed or the contextnode is invalid." :
          $error->message;
      $msg .= '</br>Path: ' . $path;
      $msg .= isset($context) ? '</br>Context: ' . ($context->tagName) : '';
      throw new Exception($msg);
    }
    return $results;
  }

}