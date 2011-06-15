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
 * Maybe a different node type for each kind of constraint.
 */
abstract class InsertOrderNode {

  public $parent; // Parent Node.
  public $min; // Occurs
  public $max; // Occurs

  public function __construct($min, $max, InsertOrderNode $parent = NULL) {
    $this->min = $min;
    $this->max = $max;
    $this->parent = $parent;
  }

  abstract public function matchesConstraint(DOMElement $element);

  abstract public function getOrder();
}

abstract class InsertOrderCompositor extends InsertOrderNode {

  public $children;

  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
    $this->children = array();
  }

  public function addChild(InsertOrderNode $child) {
    $this->children[] = $child;
  }

  public function getOrder() {
    $order = array();
    foreach ($this->children as $child) {
      $order = array_merge($order, $child->getOrder());
    }
    return $order;
  }

}

class InsertOrderAll extends InsertOrderCompositor {

  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
  }

  public function matchesConstraint(DOMElement $element) {
    
  }

}

class InsertOrderChoice extends InsertOrderCompositor {

  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
  }

  public function matchesConstraint(DOMElement $element) {
    
  }

}

class InsertOrderSequence extends InsertOrderCompositor {

  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
  }

  public function matchesConstraint(DOMElement $element) {
    foreach ($this->children as $child) {
      if ($child->matchesConstraint($element)) {
        return TRUE;
      }
    }
    return FALSE;
  }

}

class InsertOrderElement extends InsertOrderNode {

  public $name;
  public $matches; // The nodes that matched this element

  public function __construct($name, $min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
    $this->name = $name;
    $this->matches = array();
  }

  public function matchesConstraint(DOMElement $element) {
    $same_type = $element->localName == $this->name;
    $count = count($this->matches);
    if ($same_type && $count < $this->max) {
      $matches[] = $element;
      return TRUE;
    }
    return FALSE;
  }

  public function getOrder() {
    return $this->matches;
  }

}

/**
 * A tree like structure that models where elements can be insert into.
 */
class InsertOrderMap {

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
  }

  /**
   * 
   * @return InsertOrderNode
   */
  public function getInsertOrderMap() {
    return $this->element->complexType->asInsertOrderNode();
  }

  /**
   * Inserts the given child element into the right positon of its parent element.
   * 
   * @param DOMElement $parent
   * @param DOMElement $new
   */
  public function insert(DOMElement $parent, DOMElement $new) {
    $children = dom_node_children($parent, 'DOMElement'); // Child Elements.
    $children[] = $new; // Add the new child to the set of children.
    // Match constaints. The map stores the matched constaints.
    $map = $this->getInsertOrderMap();
    do {
      $matches = FALSE;
      foreach ($children as $key => $child) {
        if ($map->matchesConstraint($child)) {
          unset($children[$key]);
          $matches = TRUE;
        }
      }
    } while ($matches == TRUE);
    // Now use the map to generate the new order for elements. This doesn't work with mixed content!
    $ordered_children = array();
    $ordered_children = $map->getOrder();
    /**
     * Allows for out of order composition, when the final element is
     * added this block should not be entered.
     */
    if (count($children) > 0) {
      $ordered_children = array_merge($ordered_children, $children);
    }
    // Remove all child elements
    $children = dom_node_children($parent, 'DOMElement'); // Child Elements.
    foreach ($children as $child) {
      if (isset($child->parentNode)) {
        $child->parentNode->removeChild($child);
      }
    }
    // Re-insert the child elements, in the correct order.
    foreach ($ordered_children as $child) {
      $parent->appendChild($child);
    }
    /**
     *  We aren't iterating over the children, we are iterating of the map of constaints.
     *  When we encounter a node that excepts a given child, we move that child off the
     *  Children list and append it to the "Ordered Children's" list.
     * 
     *  The node must be aware that it matched the given element, so that the following children may not,
     *  be incorrectly matched to it.
     * 
     *  For a particular 
     * 
     * We could iterate over the children repeatedly until none were removed from the list. When that occured move to the next constraint.
     * 
     * 
     * Only the final case really matters, when all elements are created that the order needs to be determined, 
     * We must assume that it is possible...
     */
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
     * Maybe. if we just assume no order of children at the begining of processing and match them to their first matching positon.
     * 
     * So the processing should be like so.
     * 
     * At position.
     * 
     * Attempt to match all children (including the new one) to the given position, once the position is full or all elements have been run though
     * move to the next position regardless if the current position has been filed (Out of order!). 
     * 
     * Continue on building the new order of all children.
     * 
     * Still have to be weary of the rules surrounding compositors, When determining if we can move to the next node. 
     * 
     * Or do we?
     * 
     * If we move our positon only to compositors (Not to the depth of the element requirements)... 
     *   We run though all elements untill the compositor can take no more children, then move to the next compositor, at the same depth...
     * 
     *   So what happens if we say have a sequence that contains a choice that contains an element and a sequence. 
     *   How do we traverse it???  
     *  
     *   How to deal with these constraints...
     * 
     *  Choice/Sequence ...
     *  
     * We encounter a choice, that has min=max=1, it has two elements.
     * 
     * One element matches the first, so the constraint has been filled. Move on to the sibling composition.
     * 
     * If element and its min/max has been filled, move to the next.
     * 
     * Need some way to model the constaints and the order of elements
     *   
     * -----* -----* -----* -----* -----* -----* -----* -----* -----* -----* -----* -----* -----*
     */
  }

}