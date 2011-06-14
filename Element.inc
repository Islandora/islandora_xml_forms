<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:element> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'All');
module_load_include('inc', 'schema_api', 'Choice');
module_load_include('inc', 'schema_api', 'Sequence');
module_load_include('inc', 'schema_api', 'Group');

/**
 * Models a <xs:element> element.
 */
class XS_Element {
  /**
   * Constants.
   */
  const LocalName = 'element';

  /**
   * Checks to see if the given DOMElement is a xs:complexType element
   * 
   * @param DOMElement $node 
   * @return boolean
   */
  public static function isElement(DOMElement $node) {
    return $node->localName == self::LocalName;
  }

  /**
   * Protected read only members.
   * 
   * @var ReadOnlyProtectedMembers
   */
  protected $protected;

  /**
   * Create a ComplexType element.
   * 
   * @param DOMElement $node 
   */
  public function __construct(DOMElement $node) {
    $this->validateConstructorArguments($node);
    /**
     * Initialize members, many of these will be NULL.
     */
    $this->protected = new ReadOnlyProtectedMembers(
            array(
              'node' => $node,
              'all' => $this->createAll($node),
              'choice' => $this->createChoice($node),
              'sequence' => $this->createSequence($node),
              'group' => $this->createGroup($node),
            )
    );
    /**
     * We need to do this after cause this function accesses 
     * protected read only members.
     */
    $this->protected->add('compositor', $this->getCompositor());
  }

  /**
   * Checks to see if this type can be created correctly from the arguments passed to the constructor.
   * 
   * @param DOMElement $node 
   */
  private function validateConstructorArguments(DOMElement $node) {
    if (!self::isElement($node)) {
      throw new Exception('Can not create a Element object from a non Element DOMElement: ' . $node->localName);
    }
  }

  /**
   * Get a protected member for read access.
   * 
   * @param string $name 
   */
  public function __get($name) {
    return $this->protected->$name;
  }

  /**
   * Set the protected members value.
   * 
   * @param string $name
   * @param mixed $value 
   */
  public function __set($name, $value) {
    $this->protected->$name = $value;
  }

  /**
   * Creates the member if its definition exists.
   * 
   * @return DOMElement
   */
  private function createAll(DOMElement $node) {
    $result = dom_node_filter_children($node, 'DOMElement', 'localName', XS_All::LocalName);
    if (count($result) == 1) {
      return new XS_All(array_unshift($result));
    }
    return NULL;
  }

  /**
   * Creates the member if its definition exists.
   * 
   * @return DOMElement
   */
  private function createChoice(DOMElement $node) {
    $result = dom_node_filter_children($node, 'DOMElement', 'localName', XS_Choice::LocalName);
    if (count($result) == 1) {
      return new XS_Choice(array_unshift($result));
    }
    return NULL;
  }

  /**
   * Creates the member if its definition exists.
   * 
   * @return DOMElement
   */
  private function createSequence(DOMElement $node) {
    $result = dom_node_filter_children($node, 'DOMElement', 'localName', XS_Sequence::LocalName);
    if (count($result) == 1) {
      return new XS_Sequence(array_unshift($result));
    }
    return NULL;
  }

  /**
   * Creates the member if its definition exists.
   * 
   * @return DOMElement
   */
  private function createGroup(DOMElement $node) {
    $result = dom_node_filter_children($node, 'DOMElement', 'localName', XS_Group::LocalName);
    if (count($result) == 1) {
      return new XS_Group(array_unshift($result));
    }
    return NULL;
  }

  /**
   * Returns this objects compositor if defined.
   * 
   * @return DOMElement
   */
  private function getCompositor() {
    if (isset($this->all)) {
      return $this->all;
    }
    else if (isset($this->choice)) {
      return $this->choice;
    }
    else if (isset($this->sequence)) {
      return $this->sequence;
    }
    return NULL;
  }

}