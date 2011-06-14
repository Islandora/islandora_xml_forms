<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:complexContent> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'Extension');
module_load_include('inc', 'schema_api', 'Restriction');

/**
 * Models a <xs:complexContent> element.
 */
class XS_ComplexContent {
  /**
   * Constants.
   */
  const LocalName = 'complexContent';

  /**
   * Checks to see if the given DOMElement is a xs:complexType element
   * 
   * @param DOMElement $node 
   * @return boolean
   */
  public static function isComplexContent(DOMElement $node) {
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
              'extension' => $this->createExtension($node),
              'restriction' => $this->createRestriction($node)
            )
    );
  }

  /**
   * Checks to see if this type can be created correctly from the arguments passed to the constructor.
   * 
   * @param DOMElement $node 
   */
  private function validateConstructorArguments(DOMElement $node) {
    if (!self::isComplexContent($node)) {
      throw new Exception('Can not create a ComplexContent object from a non complexContent DOMElement: ' . $node->localName);
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
  private function createExtension(DOMElement $node) {
    $result = dom_node_filter_children($node, 'DOMElement', 'localName', XS_Extension::LocalName);
    if (count($result) == 1) {
      return new XS_Extension(array_unshift($result));
    }
    return NULL;
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

}