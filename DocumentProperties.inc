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

  public function asXML() {
    $document_properties = new SimpleXMLElement('<document_properties></document_properties>');
    // Root
    $root = $document_properties->addChild('root');
    if ($this->prefix) {
      $root->addChild('prefix', $this->prefix);
    }
    if ($this->name) {
      $root->addChild('local_name', $this->name);
    }
    if ($this->uri) {
      $root->addChild('uri', $this->uri);
    }
    // Namespaces
    if (count($this->namespaces) > 0) {
      $namespaces = $document_properties->addChild('namespaces');
      foreach ($this->namespaces as $prefix => $uri) {
        $namespace = $namespaces->addChild('namespace', $uri);
        $namespace->addAttribute('prefix', $prefix);
      }
    }
    // Schema
    if ($this->schema) {
      $schema = $document_properties->addChild('schema', $this->schema);
    }
    return $document_properties->asXML();
  }

}