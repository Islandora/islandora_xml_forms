<?php



/**
 * @file
 *
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'php_lib', 'LazyMembers');

/**
 * Contains functions common to the all schema element tags.
 */
abstract class XMLSchemaNode {

  /**
   *
   * @var type
   */
  protected static $nodes = array();
  /**
   * The top level schema.
   *
   * @var XMLSchema
   */
  protected $schema;
  /**
   *
   * @var ReadOnlyProtectedMembers
   */
  protected $protected;

  /**
   *
   * @param XMLSchemaNode $node
   */
  public static function register(XMLSchemaNode $node) {
    self::$nodes[] = $node;
  }

  /**
   *
   * @param DOMElement $element
   * @return boolean
   */
  public static function isRegistered(DOMElement $element) {
    foreach (self::$nodes as $node) {
      if ($element->isSameNode($node->node)) {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   *
   * @param DOMElement $element
   */
  public static function find(DOMElement $element) {
    foreach (self::$nodes as $node) {
      if ($element->isSameNode($node->node)) {
        return $node;
      }
    }
    return NULL;
  }

  /**
   * Create a XMLSchemaNode
   *
   * @param DOMElement $node
   */
  public function __construct(XMLSchema $schema, DOMElement $node) {
    $this->protected = new LazyMembers(array('node' => $node));
    self::register($this);
    $this->validateConstructorArguments($node);
    $this->schema = $schema;
  }

  /**
   * Validate the constructor's arguments.
   *
   * @param DOMElement $node
   */
  private function validateConstructorArguments(DOMElement $node) {
    if ($node->localName != $this->getLocalName()) {
      throw new Exception("Argument invalided expected local name: {$this->getLocalName()} got: {$node->localName}.");
    }
  }

  /**
   * Gets the subclasses local name.
   *
   * @return string
   */
  abstract protected function getLocalName();

  /**
   * Used to convert this object to an InsertOrderMapNode
   */
  abstract public function asInsertOrderNode(InsertOrderNode $parent = NULL);

  /**
   *
   * @param string $name
   * @return boolean
   */
  public function __isset($name) {
    return isset($this->protected->$name);
  }

  /**
   *
   * @param string $name
   */
  public function __unset($name) {
    unset($this->protected->$name);
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
   * Find all immediate children with the given local name.
   *
   * @param string $local_name
   * @return array
   */
  protected function findChildren($local_name = NULL) {
    return isset($local_name) ?
        dom_node_filter_children($this->node, 'DOMElement', 'localName', $local_name) :
        dom_node_children($this->node, 'DOMElement');
  }

  /**
   * Finds a unique child amoung the immediate children of this element.
   *
   * @param string $local_name
   */
  protected function findChild($local_name) {
    $children = $this->findChildren($local_name);
    if (count($children) == 1) {
      return array_shift($children);
    }
    return NULL;
  }

  /**
   * Create all child elements of the given type.
   *
   * @param string $local_name
   * @return array
   */
  protected function createChildren($local_name = NULL) {
    $output = array();
    $children = $this->findChildren($local_name);
    foreach ($children as $child) {
      $output[] = $this->createNodeTypedObject($child);
    }
    return $output;
  }

  /**
   * Create a child element of the given type.
   *
   * @param string $local_name
   * @param DOMElement $child
   *
   * @return mixed
   */
  protected function createChild($local_name) {
    $child = $this->findChild($local_name);
    return isset($child) ? $this->createNodeTypedObject($child) : NULL;
  }

  /**
   * Creates an a object defined by the local name of the given DOMNode if such a class exists.
   *
   * @param DOMElement $node
   * @return mixed
   */
  protected function createNodeTypedObject(DOMElement $node) {
    $class_name = "XMLSchema{$node->localName}";
    $object = self::find($node);
    if (isset($object)) {
      return $object;
    }
    elseif (class_exists($class_name)) {
      return new $class_name($this->schema, $node);
    }
    return NULL;
  }

  /**
   * Get the min and max occurs.
   *
   * @return array
   */
  public function getMinMaxOccurs() {
    $min = $this->node->hasAttribute('minOccurs') ? $this->node->getAttribute('minOccurs') : 1;
    $max = $this->node->hasAttribute('maxOccurs') ? $this->node->getAttribute('maxOccurs') : 1;
    return array($min, $max);
  }

}
