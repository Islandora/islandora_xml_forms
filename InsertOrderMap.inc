<?php

// $Id$

/**
 * @file
 *
 * Builds a map that defines where elements can be inserted
 */
module_load_include('inc', 'schema_api', 'Schema');
module_load_include('inc', 'schema_api', 'Element');

class XMLSchemaInsertOrderMapNode {

  public $parent; // Parent Node.
  public $min; // Occurs
  public $max; // Occurs
  public $type; // compositor, element.
  public $compositor; // all, choice, sequence
  public $element; // Tag name of the element.
  public $children; // Child nodes

  public function __construct($min, $max, $type, $name, XMLSchemaInsertOrderMapNode $parent = NULL) {
    // $this->validateConstructorArguments($parent, $min, $max, $type, $name); Implement later.
    $this->parent = $parent;
    $this->min = $min;
    $this->max = $max;
    $this->type = $type;
    if ($type == 'compositor') {
      $this->compositor = $name;
    }
    else {
      $this->element = $name;
    }
  }

}

/**
 * A tree like structure that models where elements can be insert into.
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
  public $element;
  /**
   *
   * @var XMLSchemaInsertOrderMapNode 
   */
  public $map;

  /**
   * Create an InsertOrderMap
   * 
   * @param Schema $schema
   * @param string $path 
   */
  public function __construct(XMLSchema $schema, $path) {
    $this->schema = $schema;
    $this->element = new XMLSchemaElement($this->schema, $this->schema->getUniqueNode($path));
    $this->map = $this->element->complexType->asInsertOrderMapNode();
  }

  /**
   * Inserts the given child element into the right positon of its parent element.
   * 
   * @param DOMElement $parent
   * @param DOMElement $child 
   */
  public function insert(DOMElement $parent, DOMElement $child) {
    // Walk through the map eating children of parent until we find a suitable resting place for child.
  }

}