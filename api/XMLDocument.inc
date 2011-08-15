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
   * When this object is put to sleep, the current state of the document is stored in this variable.
   * 
   * @var string
   */
  protected $xml;
  
  /**
   * When this object is put to sleep, the current state of the registry is stored in this variable.
   * 
   * @var array
   */
  protected $paths;

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
   * @param string $default_uri
   * @param array $namespaces
   * @param string $schema
   * @param string $xml 
   */
  public function __construct($root_name, $default_uri, $schema_uri, array &$namespaces, &$xml = NULL) {
    $this->namespaces = $namespaces;
    $this->defaultURI = $default_uri;
    $this->schema = $this->createSchema($schema_uri);
    $this->document = $this->createDocument($root_name, $default_uri, $xml);
    $this->xpath = new DOMXPath($this->document);
    $this->registry = new NodeRegistry();
    //$this->registerNamespaces();
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
    $this->paths = $this->registry->export();
    return array('namespaces', 'defaultURI', 'schema', 'registry', 'xml', 'paths');
  }

  /**
   * Reinitialize this object after it has be unserialized.
   */
  public function __wakeup() {
    $this->document = new DOMDocument();
    $this->document->loadXML($this->xml);
    $this->xpath = new DOMXPath($this->document);
    $this->registry->import($this->document, $this->paths);
    //$this->registerNamespaces();
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
   * @param string $default_uri
   * @param string $xml
   * 
   * @return DOMDocument
   */
  private function createDocument($root_name, $default_uri, $xml) {
    $document = new DOMDocument();
    isset($xml) ? // Load from XML or create the root node.
            $document->loadXML($xml) :
            $document->appendChild(new DOMElement($root_name, NULL, $default_uri));
    $root = $document->documentElement; // Make sure the required namespaces exist.
    foreach ($this->namespaces as $prefix => $uri) {
      $prefix = preg_match('/xmlns:/', $prefix) ? $prefix : "xmlns:$prefix";
      $root->setAttributeNS(self::xmlns, $prefix, $uri);
    }
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
    $results = $this->xpath->query($path, $context);
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

//class XMLDocument {
//  /**
//   * Constants
//   */
//  const xmlns = 'http://www.w3.org/2000/xmlns/';
//
//  /**
//   * The xml string that this document was created from. It can be NULL.
//   * 
//   * @var string
//   */
//  protected $xml;
//
//  /**
//   * A array of properties related to the root element of the document.
//   * 
//   * $root['local_name'] = The local name of the root element;
//   * $root['prefix'] = The namespace prefix of the root element;
//   * $root['uri'] = The local name of the root element;
//   * 
//   * @var array
//   */
//  protected $root;
//
//  /**
//   * A map of the namespaces used in the generated XML document.
//   * 
//   * array('prefix' => 'uri');
//   * 
//   * @var array 
//   */
//  public $namespaces;
//
//  /**
//   * The uri to the schema.
//   * 
//   * @var string
//   */
//  public $schema;
//
//  /**
//   * Stores associations between form elements and DOMNodes.
//   * 
//   * @var NodeRegisty
//   */
//  public $registry;
//
//  /**
//   * 
//   * @var DOMDocument
//   */
//  public $document;
//
//  /**
//   * 
//   * @var DOMXPath
//   */
//  protected $xpath;
//
//  /**
//   * Create an XMLDocument.
//   * 
//   * @param array $root
//   * @param array $namespaces
//   * @param string $schema
//   * @param string $xml 
//   */
//  public function __construct(array &$root, array &$namespaces, $schema, &$xml = NULL) {
//
//    $this->root = $root;
//    $this->namespaces = $namespaces;
//    try {
//      if (is_non_empty_string($schema)) {
//        $this->schema = new XMLSchema($schema);
//      }
//    } catch (Exception $e) {
//      $this->schema = NULL;
//      drupal_set_message($e->getMessage(), 'error');
//    }
//    $this->xml = $xml;
//    $this->registry = new NodeRegistry($this);
//    $this->document = $this->createDocument();
//    $this->xpath = new DOMXPath($this->document);
//    $this->registerNamespaces();
//  }
//
//  /**
//   * Determine what member variables to serialize. 
//   * 
//   * DOMDocument's and DOMNodes can not be serialized.
//   * 
//   * @return array
//   */
//  public function __sleep() {
//    //$this->registry->sleep();
//    return array('root', 'namespaces', 'schema', 'xml', 'registry');
//  }
//
//  /**
//   * Reinitialize this object after it has be unserialized.
//   */
//  public function __wakeup() {
//    $this->document = $this->createDocument($this->xml);
//    $this->xpath = new DOMXPath($this->document);
//    $this->registerNamespaces();
//    //$this->registry->wakeup($this);
//  }
//
//  /**
//   * Create this document from either an existing document.
//   *
//   * @param string $xml
//   * 
//   * @return DOMDocument 
//   */
//  private function createDocument($xml) {
//    if (isset($xml)) {
//      $this->xml = $xml;
//      $doc = new DOMDocument();
//      $doc->loadXML($xml);
//
//      if ($this->schema) {
//        $doc->schemaValidate($this->schema->filename);
//      }
//      return $doc;
//    }
//    else {
//      $document = new DOMDocument();
//      $root = $document->appendChild($this->createRootElement());
//      foreach ($this->namespaces as $prefix => $uri) {
//        $prefix = preg_match('/xmlns/', $prefix) ? $prefix : "xmlns:$prefix";
//        $root->setAttributeNS(self::xmlns, $prefix, $uri);
//      }
//      return $document;
//    }
//  }
//
//  /**
//   * 
//   */
//  private function createFromExistingDocument() {
//    $doc = new DOMDocument();
//    $doc->loadXML($this->xml);
//    if ($this->schema) {
//      $doc->schemaValidate($this->schema->filename);
//    }
//    return $doc;
//  }
//
//  /**
//   * Creates the root element of the document.
//   * 
//   * @return DOMElement 
//   */
//  private function createRootElement() {
//    $prefix = $this->root['prefix'];
//    $name = $this->root['local_name'];
//    $uri = $this->root['uri'];
//    $has_prefix = isset($prefix) && $prefix != '';
//    $name = $has_prefix ? "{$prefix}:{$name}" : $name;
//    $uri = $uri != '' ? $uri : NULL;
//    if (empty($name) && $name == '') {
//      throw new Exception('Missing local name for the root element in the form properties.');
//    }
//    return new DOMElement($name, NULL, $uri);
//  }
//
//  /**
//   * Register the namespaces for XPath to work correctly.
//   */
//  private function registerNamespaces() {
//    foreach ($this->namespaces as $prefix => $uri) {
//      $this->xpath->registerNamespace($prefix, $uri);
//    }
//  }
//
//  /**
//   * Checks to see if this document existed before this clas
//   * 
//   * @return boolean
//   */
//  public function exists() {
//    return is_non_empty_string($this->xml);
//  }
//
//  /**
//   * Get the namespace URI for the given prefix.
//   * 
//   * @param string $prefix 
//   * @return string
//   */
//  public function getNamespaceURI($prefix = NULL) {
//    if (isset($prefix)) {
//      return isset($this->namespaces[$prefix]) ? $this->namespaces[$prefix] : NULL;
//    }
//    else {
//      if (isset($this->root['uri'])) {
//        return $this->root['uri'];
//      }
//      return NULL;
//    }
//  }
//
//  /**
//   * Converts the document to an XML string.
//   * 
//   * @return string 
//   */
//  public function saveXML() {
//    return $this->document->saveXML();
//  }
//
//  /**
//   * Query the document.
//   * 
//   * @param string $path
//   *   XPath string.
//   * @param DOMNode $context
//   *   Context node, if NULL the context of the path will be the entire document.
//   * 
//   * @return DOMNodeList 
//   */
//  public function query($path, DOMNode $context = NULL) {
//    libxml_use_internal_errors(TRUE); // Prevent Drupal from catching the error.
//    $results = isset($context) ?
//        $this->xpath->query($path, $context) :
//        $this->xpath->query($path);
//    $error = FALSE;
//    $error = libxml_get_last_error();
//    libxml_use_internal_errors(FALSE); // Allow Drupal to catch errors.s
//    $failed = ($results === FALSE) || ($error !== FALSE);
//    if ($failed) {
//      $msg = ($results === FALSE) ?
//          "XPath query failed, either the expression is malformed or the contextnode is invalid." :
//          $error->message;
//      $msg .= '</br>Path: ' . $path;
//      $msg .= isset($context) ? '</br>Context: ' . ($context->tagName) : '';
//      throw new Exception($msg);
//    }
//    return $results;
//  }
//
//}