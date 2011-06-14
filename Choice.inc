<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:choice> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'Element');

/** 
 * Models a <xs:choice> element.
 */
class XS_Choice {
  /**
   * Constants.
   */
  const LocalName = 'choice';

  /**
   * Checks to see if the given DOMChoice is a xs:complexType Choice
   * 
   * @param DOMChoice $node 
   * @return boolean
   */
  public static function isChoice(DOMChoice $node) {
    return $node->localName == self::LocalName;
  }

  /**
   * Protected read only members.
   * 
   * @var ReadOnlyProtectedMembers
   */
  protected $protected;

  /**
   * Create a ComplexType Choice.
   * 
   * @param DOMChoice $node 
   */
  public function __construct(DOMChoice $node) {
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
   * @param DOMChoice $node 
   */
  private function validateConstructorArguments(DOMChoice $node) {
    if (!self::isChoice($node)) {
      throw new Exception('Can not create a Choice object from a non Choice DOMChoice: ' . $node->localName);
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
   * Creates Choice the elements that exist within this <xs:Choice> compositor.
   * 
   * @param DOMElement $node 
   * @return array
   */
  private function createElements(DOMElement $node) {
    return array();
  }

}