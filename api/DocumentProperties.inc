<?php

// $Id$

/**
 * @file
 *
 */
class DocumentProperties {

  /**
   * The local name of the root node.
   * 
   * @var string
   */
  public $name;
  /**
   * The namespace prefix of the root node. '' possible.
   * 
   * @var string
   */
  public $prefix;
  /**
   * The namespace URI for the root node.
   * 
   * @var string
   */
  public $uri;
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
   * Create DocumentProperties object from an XML string definition.
   * 
   * @param string $xml_definition
   * @return DocumentProperties
   */
  public static function createFromXMLDefiniton($xml_definition) {
    return self::createFromSimpleXML(simplexml_load_string($xml_definition));
  }

  /**
   * Create DocumentProperties object from an SimpleXMLElement.
   * 
   * @param SimpleXMLElement $properties
   * @return DocumentProperties
   */
  public static function createFromSimpleXML(SimpleXMLElement $properties) {
    $root = $properties->root;
    $root_prefix = (string) $root->prefix;
    $root_name = (string) $root->local_name;
    $root_uri = (string) $root->uri;
    $schema = isset($properties->schema) ? (string) $properties->schema : NULL;
    $namespaces = array();
    if (isset($properties->namespaces)) {
      $children = $properties->namespaces->children();
      foreach ($children as $namespace) {
        $prefix = (string) $namespace['prefix'];
        $uri = (string) $namespace;
        $prefix = $prefix == '' ? 'default' : $prefix;
        $namespaces[$prefix] = $uri;
      }
    }
    return new self($root_prefix, $root_name, $root_uri, $schema, $namespaces);
  }

  /**
   * Create DocumentProperties.
   * 
   * @param string $root_prefix
   * @param string $root_name
   * @param string $root_uri
   * @param string $schema
   * @param array $namespaces 
   */
  public function __construct($root_prefix, $root_name, $root_uri, $schema, array $namespaces) {
    $this->prefix = $root_prefix;
    $this->name = $root_name;
    $this->uri = $root_uri;
    $this->schema = $schema;
    $this->namespaces = $namespaces;
  }

  public function asNode(DOMDocument $document) {
    $document_properties = $document->createElement('document_properties');
    $root = $document->createElement('root');
    // Root Info
    if (isset($this->prefix)) {
      $prefix = $document->createElement('prefix', $this->prefix);
      $root->appendChild($prefix);
    }
    if (isset($this->name)) {
      $local_name = $document->createElement('local_name', $this->name);
      $root->appendChild($local_name);
    }
    if (isset($this->uri)) {
      $uri = $document->createElement('uri', $this->uri);
      $root->appendChild(uri);
    }
    $document_properties->appendChild($root);
    // Namespaces
    if (count($this->namespaces) > 0) {
      $namespaces = $document->createElement('namespaces');
      foreach ($this->namespaces as $prefix => $uri) {
        $namespace = $document->createElement('namespace', $uri);
        $namespace->setAttribute('prefix', $prefix);
        $namespaces->appendChild($namespace);
      }
      $document_properties->appendChild($namespaces);
    }
    // Schema
    if (isset($this->schema)) {
      $schema = $document->createElement('schema', $this->schema);
      $document_properties->appendChild($schema);
    }
    return $document_properties;
  }

}