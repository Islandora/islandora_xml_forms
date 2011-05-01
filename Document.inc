<?php

// $Id$

/**
 * @file
 *
 */
class XMLDocumentProperties {

  /**
   *
   * @var string 
   */
  public $root;
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
    $this->root = (string) $properties->root;
    if (isset($properties->schema)) {
      $this->schema = (string) $properties->schema;
    }
    if (isset($properties->namespaces)) {
      foreach ($properties->namespaces as $namespace) {
        $prefix = $namespace['prefix'];
        $uri = (string) $namespace;
        $prefix = $prefix == '' ? 'default' : $prefix;
        $this->namespaces[$prefix] = $uri;
      }
    }
  }

}

/**
 * 
 */
class XMLDocument {

  /**
   * 
   * @var XMLDocumentProperties 
   */
  protected $properties;
  /**
   *
   * @var string 
   */
  protected $xml;

  /**
   *
   * @param XMLDocumentProperties $document_properties
   * @param string $xml 
   */
  public function __construct(XMLDocumentProperties $document_properties, &$xml = NULL) {
    $this->properties = $document_properties;
  }

}