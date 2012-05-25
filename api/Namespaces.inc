<?php



/**
 * @file
 *
 */

/**
 * Exceptions that may occur when building.
 */
class XMLFormsNamespaceException extends Exception {

  public function __construct($message = "", $code = 0, $previous = NULL) {
    parent::__construct($message, $code, $previous);
  }

}

/**
 * This class is meant to encapsulate namespaces.
 */
class Namespaces implements IteratorAggregate {
  /**
   * Constants
   */
  const xmlns = 'http://www.w3.org/2000/xmlns/';

  /**
   * The default namespace, it can be NULL.
   *
   * @var string
   */
  protected $default;

  /**
   * An array of namespaces declarations where the key is the prefix and the value is the uri.
   *
   * For example: array('prefix' => 'uri');
   *
   * @var array
   */
  protected $namespaces;

  /**
   * Creates an Namespaces Instance.
   *
   * @param array $namespaces
   *   An array of namespaces declarations where the key is the prefix and the value is the uri.
   * @param string $default
   *   The default namespace for the document.
   */
  public function __construct(array &$namespaces = NULL, $default = NULL) {
    $this->default = $default;
    $this->namespaces = isset($namespaces) ? $namespaces : array();
  }

  /**
   * Gets the URI associated with the given prefix.
   *
   * @param string $prefix
   */
  public function getURI($prefix) {
    if (isset($this->namespaces[$prefix])) {
      return $this->namespaces[$prefix];
    }
    throw new XMLFormsNamespaceException("Namespace URI with the prefix '$prefix' was not defined.");
  }

  /**
   * Gets the URI associated with the given prefix.
   *
   * @param string $prefix
   */
  public function getPrefix($uri) {
    $prefix = array_search($uri, $this->namespaces);
    if ($prefix !== FALSE) {
      return $prefix;
    }
    throw new XMLFormsNamespaceException("Namespace URI '$uri' was not defined, could not find the prefix.");
  }

  /**
   * Gets the default namespace URI.
   *
   * @return string
   */
  public function getDefaultURI() {
    return $this->default;
  }

  /**
   * Adds all the namespace as attributes to a given element.
   *
   * @param DOMElement $element
   */
  public function addNamespacesAttributesTo(DOMElement $element) {
    if (isset($this->default)) {
      $element->setAttribute('xmlns', $this->default);
    }
    foreach ($this->namespaces as $prefix => $uri) {
      $element->setAttributeNS(self::xmlns, 'xmlns:' . $prefix, $uri);
    }
  }

  /**
   * Gets an iterator that allows the user to iterate over the namespaces.
   *
   * @return ArrayIterator
   */
  public function getIterator() {
    $namespaces = $this->namespaces;
    if (isset($this->default)) {
      $namespaces[''] = $this->default;
    }
    return new ArrayIterator($namespaces);
  }

}
