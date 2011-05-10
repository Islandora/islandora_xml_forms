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
    $root_prefix = (string) $properties->root['prefix'];
    $root_name = (string) $properties->root['local_name'];
    $root_uri = (string) $properties->root['uri'];
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
  public function __construct($root_prefix, $root_name, $root_uri, $schema, array &$namespaces) {
    $this->prefix = $root_prefix;
    $this->name = $root_name;
    $this->uri = $root_uri;
    $this->schema = $schema;
    $this->namespaces = $namespaces;
  }

}