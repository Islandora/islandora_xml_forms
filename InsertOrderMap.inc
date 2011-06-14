<?php

// $Id$

/**
 * @file
 *
 * Builds a map that defines where elements can be inserted
 */
module_load_include('inc', 'schema_api', 'Schema');
module_load_include('inc', 'schema_api', 'Element');

/**
 * A structure that models where elements can be insert into.
 */
class XMLSchemaInsertOrderMap {

  /**
   * The schema.
   *
   * @var Schema
   */
  protected $schema;
  /**
   * The element defined.
   * 
   * @var XS_Element
   */
  protected $element;

  /**
   * Create an InsertOrderMap
   * 
   * @param Schema $schema
   * @param string $path 
   */
  public function __construct(XMLSchema $schema, $path) {
    $this->schema = $schema;
    $this->element = new XMLSchemaElement($this->schema->getUniqueNode($path));
    $this->buildMap();
  }

  /**
   * Constructs the insert order map.
   */
  private function buildMap() {
    
  }

  /**
   * Inserts the given child element into the right positon of its parent element.
   * 
   * @param DOMElement $parent
   * @param DOMElement $child 
   */
  public function insert(DOMElement $parent, DOMElement $child) {
    
  }

}