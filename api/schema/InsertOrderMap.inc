<?php

/**
 * @file
 *
 * Builds a map that defines where elements can be inserted.
 */
module_load_include('inc', 'xml_schema_api', 'Schema');
module_load_include('inc', 'xml_schema_api', 'Element');

/**
 * Models Insert order constraints.
 */
abstract class InsertOrderNode {

  /**
   * The parent of this InsertOrderNode.
   *
   * @var InsertOrderNode
   */
  public $parent;

  /**
   *
   * @var mixed (integer or 'unbounded')
   */
  public $min;

  /**
   *
   * @var mixed (integer or 'unbounded')
   */
  public $max;

  /**
   * Create an InsertOrderNode.
   *
   * @param mixed $min (integer or 'unbounded')
   * @param mixed $max (integer or 'unbounded')
   * @param InsertOrderNode $parent
   */
  public function __construct($min, $max, InsertOrderNode $parent = NULL) {
    $this->min = $min;
    $this->max = $max;
    $this->parent = $parent;
  }

  /**
   * Attempts to store the given DOMElement in a matched constraint.
   *
   * @param DOMElement $element
   * @return boolean
   */
  abstract public function storeMatch(DOMElement $element);

  /**
   * Gets an array of DOMElements in their correct insert order.
   *
   * @return DOMElement[]
   */
  abstract public function getOrder();

  /**
   * Checks if this node or a child node meets this constraint.
   *
   * Does not account for occurance constraints.
   *
   * @param DOMElement $element
   *
   * @return boolean
   */
  abstract public function constraintMet(DOMElement $element);

  /**
   * Has this constraint matched it's max number of occurances.
   *
   * @return boolean
   */
  abstract function maximumConstraintsMet();
  //abstract public function matchesConstraint(DOMElement $element);
  //abstract public function matches(DOMElement $element);
}

/**
 * Models a generic compositor constraint either All, Sequence or Choice.
 */
abstract class InsertOrderCompositor extends InsertOrderNode {

  /**
   * Child Insert Order Constraints. Compositor's can repeat.
   *
   * @var InsertOrderNode[][]
   */
  public $children;

  /**
   * Create an InsertOrderCompositor.
   *
   * @param mixed $min (integer or 'unbounded')
   * @param mixed $max (integer or 'unbounded')
   * @param InsertOrderCompositor $parent
   */
  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
    $this->children = array();
    $this->children[0] = array();
  }

  /**
   * If a parent InsertOrderCompositor, gets repeated this will be called.
   */
  public function __clone() {
    $children = $this->children[0];
    $this->children = array();
    $this->children[0] = array();
    foreach ($children as $child) {
      $this->addChild(clone $child);
    }
  }

  /**
   * Add a child to the Compositor's definition.
   *
   * @param InsertOrderNode $child
   */
  public function addChild(InsertOrderNode $child) {
    $this->children[0][] = $child;
  }

  /**
   * Create new Occurance of this compositor.
   *
   * @return boolean
   */
  public function createOccurrence() {
    if (!$this->maximumConstraintsMet()) {
      $new = clone $this->children[0];
      $this->children[] = $new;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Has this constraint matched it's max number of constraints.
   *
   * @return boolean
   */
  public function maximumConstraintsMet() {
    $count = count($this->children);
    return is_numeric($this->max) ? $count == $this->max : FALSE;
  }

  /**
   * Attempts to store the given DOMElement in a matched constraint.
   *
   * @param DOMElement $element
   * @return boolean
   */
  public function storeMatch(DOMElement $element) {
    if ($this->constraintMet($element)) { // A child constraint matches this element.
      if ($this->attemptStore($element)) {
        return TRUE;
      }
      else {
        if ($this->createOccurrence()) {
          return $this->attemptStore($element); // Attempt to store.
        }
      }
    }
    return FALSE;
  }

  /**
   * A helper function used by store match that attempts to store.
   *
   * @param DOMElement $element
   * @return type
   */
  private function attemptStore(DOMElement $element) {
    $count = count($this->children);
    for ($i = 0; $i < $count; $i++) {
      foreach ($this->children[$i] as $child) {
        if ($child->storeMatch($element)) {
          return TRUE;
        }
      }
    }
    return FALSE;
  }

  /**
   * Gets an array of DOMElements in their correct insert order.
   *
   * @return DOMElement[]
   */
  public function getOrder() {
    $order = array();
    $count = count($this->children);
    for ($i = 0; $i < $count; $i++) {
      foreach ($this->children[$i] as $child) {
        $order = array_merge($order, $child->getOrder());
      }
    }
    return $order;
  }

  /**
   * Checks if this node or a child node meets this constraint.
   *
   * Does not account for occurance constraints.
   *
   * @param DOMElement $element
   *
   * @return boolean
   */
  public function constraintMet(DOMElement $element) {
    foreach ($this->children[0] as $child) {
      if ($child->constraintMet($element)) {
        return TRUE;
      }
    }
    return FALSE;
  }

}

/**
 *
 */
class InsertOrderAll extends InsertOrderCompositor {

  /**
   * Create a InsertOrderAll node.
   *
   * @param mixed $min (integer or 'unbounded')
   * @param mixed $max (integer or 'unbounded')
   * @param InsertOrderCompositor $parent
   */
  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
  }

}

/**
 * dude.......
 */
class InsertOrderChoice extends InsertOrderCompositor {

  /**
   * Create a InsertOrderChoice node.
   *
   * @param mixed $min (integer or 'unbounded')
   * @param mixed $max (integer or 'unbounded')
   * @param InsertOrderCompositor $parent
   */
  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
  }

}

/**
 * ........
 */
class InsertOrderSequence extends InsertOrderCompositor {

  /**
   * Create a InsertOrderSequence node.
   *
   * @param mixed $min (integer or 'unbounded')
   * @param mixed $max (integer or 'unbounded')
   * @param InsertOrderCompositor $parent
   */
  public function __construct($min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
  }

}

/**
 * Models the Elements constraints, DOMElements get stored as matches in this Object.
 */
class InsertOrderElement extends InsertOrderNode {

  /**
   * The name of the Element that this models.
   *
   * @var string
   */
  public $name;

  /**
   * The DOMElement's that match this constraint.
   *
   * @var DOMElement[]
   */
  public $matches;

  /**
   *
   * @param string $name
   * @param mixed $min (integer or 'unbounded')
   * @param mixed $max (integer or 'unbounded')
   * @param InsertOrderCompositor $parent
   */
  public function __construct($name, $min, $max, InsertOrderCompositor $parent = NULL) {
    parent::__construct($min, $max, $parent);
    $this->name = $name;
    $this->matches = array();
  }

  /**
   * Don't copy the matched DOMElement's.
   */
  public function __clone() {
    $this->matches = array();
  }

  /**
   * Has this constraint matched it's max number of constraints.
   *
   * @return boolean
   */
  public function maximumConstraintsMet() {
    $count = count($this->matches);
    return is_numeric($this->max) ? $count == $this->max : FALSE;
  }

  /**
   * Has the minimum set of constraints been met for this InsertOrderNode.
   *
   * @return boolean
   */
  public function minimumConstraintsMet() { // Not being used at the moment!
    $count = count($this->matches);
    $within_min_range = $this->min <= $count;
    $within_max_range = is_numeric($this->max) ? $count <= $this->max : TRUE; // If max is non-numeric its assumed to be 'unbounded'.
    return $within_min_range && $meets_max_range;
  }

  /**
   * Does the given DOMElement match this element's non occurance constraints.
   *
   * @param DOMElement $element
   * @return boolean
   */
  public function constraintMet(DOMElement $element) {
    return $element->localName == $this->name;
  }

  /**
   * Attempt to store DOMElement if it mets this Insert Order Node's constraints.
   *
   * @param DOMElement $element
   * @return boolean
   */
  public function storeMatch(DOMElement $element) {
    if ($this->constraintMet($element) && !$this->maximumConstraintsMet()) {
      $this->matches[] = $element;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Gets the DOMElement's matched by this constraint.
   *
   * @return DOMElement[]
   */
  public function getOrder() {
    return $this->matches;
  }

}

/**
 * Models the Any Element constraint. DOMElements get stored as matches in this Object.
 */
class InsertOrderAny extends InsertOrderNode {

  /**
   * The names of the Element's that this models.
   *
   * @var string[]
   */
  public $names;

  /**
   * The DOMElement's that match this constraint.
   *
   * @var DOMElement[]
   */
  public $matches;

  /**
   * Create an Any Constraint
   *
   * @param array $names
   * @param mixed $min (integer or 'unbounded')
   * @param mixed $max (integer or 'unbounded')
   * @param InsertOrderNode $parent
   */
  public function __construct(array $names, $min, $max, InsertOrderNode $parent = NULL) {
    parent::__construct($min, $max, $parent);
    $this->names = $names;
    $this->matches = array();
  }

  /**
   * Don't copy the matched DOMElement's.
   */
  public function __clone() {
    $this->matches = array();
  }

  /**
   * Has this constraint matched it's max number of constraints.
   *
   * @return boolean
   */
  public function maximumConstraintsMet() {
    $count = count($this->matches);
    return is_numeric($this->max) ? $count == $this->max : FALSE;
  }

  /**
   * Has the minimum set of constraints been met for this InsertOrderNode.
   *
   * @return boolean
   */
  public function minimumConstraintsMet() { // Not being used at the moment!
    $count = count($this->matches);
    $within_min_range = $this->min <= $count;
    $within_max_range = is_numeric($this->max) ? $count <= $this->max : TRUE; // If max is non-numeric its assumed to be 'unbounded'.
    return $within_min_range && $meets_max_range;
  }

  /**
   * Does the given DOMElement match this element's non occurance constraints.
   *
   * @param DOMElement $element
   * @return boolean
   */
  public function constraintMet(DOMElement $element) {
    $local_name = $element->localName;
    foreach ($this->names as $name) {
      if ($name == $local_name) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * Attempt to store DOMElement if it mets this Insert Order Node's constraints.
   *
   * @param DOMElement $element
   * @return boolean
   */
  public function storeMatch(DOMElement $element) {
    if ($this->constraintMet($element) && !$this->maximumConstraintsMet()) {
      $this->matches[] = $element;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Gets the DOMElement's matched by this constraint.
   *
   * @return DOMElement[]
   */
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
   * @var XMLSchema
   */
  protected $schema;

  /**
   * Create an InsertOrderMap
   *
   * @param Schema $schema
   * @param string $path
   */
  public function __construct(XMLSchema $schema) {
    $this->schema = $schema;
  }

  /**
   * Gets a tree of InsertOrderNode's that defines how to arrange the insert order of DOMElements.
   *
   * @param string $path
   *
   * @return InsertOrderNode
   */
  public function getInsertOrderMap($path) {
    $element = new XMLSchemaElement($this->schema, $this->schema->getUniqueNode($path));
    if (isset($element->complexType)) {
      return $element->complexType->asInsertOrderNode();
    }
  }

  /**
   * Inserts the given child element into the right positon of its parent element.
   *
   * @param string $path
   * @param DOMElement $parent
   * @param DOMElement $new
   */
  public function insert($path, DOMElement $parent, DOMElement $new) {
    $map = $this->getInsertOrderMap($path);
    $remaining_children = $this->populateInsertOrderMap($map, $this->getChildrenToReorder($parent, $new));
    $insert_order = $this->getChildrenInsertOrder($map, $remaining_children);
    $this->removeChildElements($parent);
    $this->appendChildElements($parent, $insert_order);
  }

  /**
   * Stores children DOMElement's insert order. Any remaining DOMElements are returned.
   *
   * @param InsertOrderMap $map
   * @param array $children
   *
   * @return array
   */
  private function populateInsertOrderMap(InsertOrderNode $map, array $children) {
    do {
      $matches = FALSE;
      foreach ($children as $key => $child) {
        if ($map->storeMatch($child)) {
          unset($children[$key]);
          $matches = TRUE;
        }
      }
    } while ($matches == TRUE);
    return $children;
  }

  /**
   * Gets a list of child DOMElements to reorder.
   *
   * @param DOMElement $parent
   * @param DOMElement $new
   *
   * @return DOMElement[]
   */
  private function getChildrenToReorder(DOMElement $parent, DOMElement $new) {
    $children = dom_node_children($parent, 'DOMElement'); // Existing child elements.
    $children[] = $new; // Add the new child to the set of children.
    return $children;
  }

  /**
   * Get an array of child DOMElements in there correct insert order.
   *
   * @param InsertOrderMap $map
   * @param array $remaining_children
   *
   * @return array
   */
  private function getChildrenInsertOrder(InsertOrderNode $map, array $remaining_children) {
    // Now use the map to generate the new order for elements. This doesn't work with mixed content!
    $insert_order = array();
    $insert_order = $map->getOrder();
    /**
     * Allows for out of order composition, when the final element is
     * added this block should not be entered.
     */
    if (count($remaining_children) > 0) {
      $insert_order = array_merge($insert_order, $remaining_children);
    }
    return $insert_order;
  }

  /**
   * Remove all child elements
   *
   * @param DOMElement $parent
   */
  private function removeChildElements(DOMElement $parent) {
    $children = dom_node_children($parent, 'DOMElement'); // Child Elements.
    foreach ($children as $child) {
      if (isset($child->parentNode)) {
        $child->parentNode->removeChild($child);
      }
    }
  }

  /**
   * Re-insert the child elements, in the correct order.
   *
   * @param DOMElement $parent
   * @param array $children
   */
  private function appendChildElements(DOMElement $parent, array $children) {
    foreach ($children as $child) {
      $parent->appendChild($child);
    }
  }

}
