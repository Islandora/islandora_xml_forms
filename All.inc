<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:all> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'Element');

/**
 * Models a <xs:all> element.
 */
class XS_All {
  /**
   * Constants.
   */
  const LocalName = 'all';

  /**
   * Checks to see if the given DOMAll is a xs:complexType All
   * 
   * @param DOMAll $node 
   * @return boolean
   */
  public static function isAll(DOMAll $node) {
    return $node->localName == self::LocalName;
  }

  /**
   * Protected read only members.
   * 
   * @var ReadOnlyProtectedMembers
   */
  protected $protected;

  /**
   * Create a ComplexType All.
   * 
   * @param DOMAll $node 
   */
  public function __construct(DOMAll $node) {
    $this->validateConstructorArguments($node);
    /**
     * Initialize members, many of these will be NULL.
     */
    $this->protected = new ReadOnlyProtectedMembers(
            array(
              'node' => $node,
              'element' => $this->createElements($node)
            )
    );
  }

  /**
   * Checks to see if this type can be created correctly from the arguments passed to the constructor.
   * 
   * @param DOMAll $node 
   */
  private function validateConstructorArguments(DOMAll $node) {
    if (!self::isAll($node)) {
      throw new Exception('Can not create a All object from a non All DOMAll: ' . $node->localName);
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
   * Creates all the elements that exist within this <xs:all> compositor.
   * 
   * @param DOMElement $node 
   * @return array
   */
  private function createElements(DOMElement $node) {
    return array();
  }

}