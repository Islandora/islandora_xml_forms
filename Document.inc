<?php

// $Id$

/**
 * @file
 *
 */
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

class DocumentPath {
  /**
   * Constants that define the possible contexts for this path to be executed in.
   */
  const DocumentContext = 'document';
  const ParentContext = 'parent'; // Only works if this element is nested within an element that has a full path.
  const SelfContext = 'self'; // Only works if a node is registered, with the given element.
  /**
   *
   * @var FormElement
   */

  public $element;
  /**
   *
   * @var string
   */
  public $path;
  /**
   *
   * @var string 
   */
  public $context;

  /**
   *
   * @param FormElement $element
   * @param type $path
   * @param type $context 
   */
  public function __construct(FormElement $element, $path, $context) {
    $this->element = $element;
    $this->path = $path;
    $this->context = $context;
  }

  /**
   *
   * @param Document $document
   * @return DOMNode
   */
  private function getContext(Document $document) {
    switch ($this->context) {
      case self::DocumentContext:
        return NULL;
      case self::ParentContext:
        return $this->getParent($document);
      case self::SelfContext:
        return $this->getSelf($document);
      default:
        throw new Exception('Invalid Context.');
    }
  }

  /**
   *
   * @param Document $document
   * @return DOMNode
   */
  private function getParent(Document $document) {
    $element = $this->element->parent;
    while ($element) {
      $node = $document->getRegisteredNode($element);
      if ($node) {
        return $node;
      }
    }
  }

  /**
   *
   * @param Document $document
   * @return DOMNode 
   */
  private function getSelf(Document $document) {
    $node = $document->getRegisteredNode($this->element);
    if ($node) {
      return $node;
    }
    else {
      throw new Exception('Failed to get self this element path is not registered');
    }
  }

  /**
   *
   * @param Document $document
   * @return DOMNodeList 
   */
  public function query(Document $document) {
    $context = $this->getContext($document);
    return $document->query($this->path, $context);
  }

}

/**
 * 
 */
class DocumentAction {

  public function __construct() {
    
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
  protected $xml; // Existing document in string form from which this document is built
  /**
   * 
   * @var DOMDocument 
   */
  protected $document;
  /**
   *
   * @var DOMXPath 
   */
  protected $xpath;
  /**
   * @var DocumentActions
   */
  protected $actions;
  
  /**
   *
   * @var array 
   */
  protected $registered;
  protected $nodes_in_the_form;

  /**
   *
   * @param XMLDocumentProperties $document_properties
   * @param string $xml 
   */
  public function __construct(DocumentProperties $document_properties, &$xml = NULL) {
    $this->nodes_in_the_form = array();
    $this->properties = $document_properties;
    $this->xml = $xml;
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
   * 
   */
  public function addAction() {
    
  }

  /**
   *
   * @param type $path
   * @param type $context
   * @return type 
   */
  public function query($path, $context = NULL) {
    if ($context) {
      return $this->xpath->query($path, $context);
    }
    else {
      return $this->xpath->query($path);
    }
  }

  // TODO rename this to something that makes sense.
  public function register(DOMNode $node, FormElement $form_element) {
    $this->nodes_in_the_form[$form_element->hash] = $node;
  }

  public function getRegisteredNode(FormElement $element) {
    if (isset($this->nodes_in_the_form[$element->hash])) {
      return $this->nodes_in_the_form[$element->hash];
    }
    else {
      return NULL;
    }
  }
  
  public function isRegistered(FormElement $element) {
    return $this->getRegisteredNode($element) === NULL;
  }

}