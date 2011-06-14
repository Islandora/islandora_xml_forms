<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:SimpleType> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'Extension');
module_load_include('inc', 'schema_api', 'Restriction');

/**
 * Models a <xs:SimpleType> element.
 */
class XS_SimpleType {
  /**
   * Constants.
   */
  const LocalName = 'SimpleType';

  /**
   * Checks to see if the given DOMElement is a xs:complexType element
   * 
   * @param DOMElement $node 
   * @return boolean
   */
  public static function isSimpleType(DOMElement $node) {
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
              'restriction' => $this->createRestriction($node),
              'list' => $this->createList($node),
              'union' => $this->createUnion($node),
            )
    );
  }

  /**
   * Checks to see if this type can be created correctly from the arguments passed to the constructor.
   * 
   * @param DOMElement $node 
   */
  private function validateConstructorArguments(DOMElement $node) {
    if (!self::isSimpleType($node)) {
      throw new Exception('Can not create a SimpleType object from a non SimpleType DOMElement: ' . $node->localName);
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
  private function createRestriction(DOMElement $node) {
    $result = dom_node_filter_children($node, 'DOMElement', 'localName', XS_Restriction::LocalName);
    if (count($result) == 1) {
      return new XS_Restriction(array_unshift($result));
    }
    return NULL;
  }

  /**
   * Creates the member if its definition exists.
   * 
   * @return DOMElement
   */
  private function createList(DOMElement $node) {
    $result = dom_node_filter_children($node, 'DOMElement', 'localName', XS_List::LocalName);
    if (count($result) == 1) {
      return new XS_List(array_unshift($result));
    }
    return NULL;
  }

  /**
   * Creates the member if its definition exists.
   * 
   * @return DOMElement
   */
  private function createUnion(DOMElement $node) {
    $result = dom_node_filter_children($node, 'DOMElement', 'localName', XS_Union::LocalName);
    if (count($result) == 1) {
      return new XS_Union(array_unshift($result));
    }
    return NULL;
  }

}