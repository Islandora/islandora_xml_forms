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
 * Current position, moves along the map.
 */
class XMLSchemaInsertOrderMapNodeTracker {
  
}

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
// How are we gonna do this.
    /**
     * foreach child we attempt to match it to a position in the map. Storing our current positon in the map, 
     * when we come across a element node that excepts the type our child is (identified by local_name). 
     * 
     * At position.
     * 
     * Match child with position. If position is fully occupied move on to next position.
     *   Potential problems:
     *     + Nothing obvious...
     * 
     * If child doesn't match the current position, but a match is required to proceed what should we do? // aka out of order construction...
     * Options:
     *   Ignore it? 
     *     + Continue trying to match the following children to this position in the map.
     *       Potential problems: 
     *         - One of the following children may match to this position but is supposed to match to a later position. 
     *            + Is this really important? They will be of the same type, order dependance of a element is a syntatic difference not a semantic one, under the same parent in XML?
     *         - 
     *     + Move to the next position in the map with the same child trying to find a match.
     * 
     * If child doesn't match the current position, but a match isn't required to proceed?
     * Options:
     *    + Continue trying to match the following children to this position in the map.
     *       Potential problems:
     *        - Any time we editing or creating a element we should be able to assume it was in the correct order to the best of its ability (missing elements), when we process it?
     *    + Ignore and continue to the next node in the map with the same child.
     *       Potential problems:
     *        - If we can't assume that the existing children were already in the correct order to the best of their ability, then we could end up with a dangling element, one 
     *          that followed after the current but was supposed to be matched to this positon, messing up the rest of our processing.
     *
     * How to deal with compositors.
     *    If <xs:all> is present just insert at the end of the element.
     *
     *    Choices can contain sequences...
     * 
     *    We require intelligent backtracking to deal with choices. As the choices can appear out of order and an 'unbounded' amount of times. 
     *    What of unbounded sequences? Their components have to appear in order if the first element is present.
     * 
     * What does it mean to be out of order what can we expect to find later, out of order, how does that affect what I'm currently processing?
     * 
     * Match then test if we can insert the new child?
     * 
     * We test if we can insert it into this position. How???
     * 
     * If the following element in the child array is not of the same type as the child.
     * 
     * Topology and discrete mathematics would come in handy now.
     * 
     * -----* -----* -----* -----* -----* -----* -----* -----* -----* -----* -----* -----* -----*
     * Could we just collect all the children at each insertion and reorder the elements, not just find the 
     * insert position for the new element ???
     * 
     * This is dependant on the order having no semantic value for the same types. A schema could assume this. But it would be
     * up to the users to enforce it, as xsd's don't allow for different typed elements sharing the same name under an element.
     * I think we can safely assume that schema designers wouldn't create this semantic difference under the same parent.
     * 
     * Is this actually easier than determining the insert positon of the new child?
     * 
     * 
     * -----* -----* -----* -----* -----* -----* -----* -----* -----* -----* -----* -----* -----*
     */
    
  }

}