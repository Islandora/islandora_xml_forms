<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'Document');

/**
 * Tracks the nodes FormElements repersent.
 */
class NodeRegistry implements IteratorAggregate {

  /**
   * Only valid when we serialize this object. It stores the paths to registered nodes 
   * so they can be properly reinitialized on unserialize.
   * 
   * @var array
   */
  protected $paths;
  /**
   * A map of FormElements to the DOMNodes the repersent. These nodes are used for populating the form elements
   * as well as a reference to what DOMNodes need to be modified/removed on submit.
   * 
   * @var array
   */
  protected $registered;
  protected $elements; // @todo implement.
  /**
   * The document that the registered nodes come from.
   * 
   * @var document
   */
  public $document;

  /**
   * Create the node registry.
   * 
   * @param Document $document 
   */
  public function __construct(Document $document) {
    $this->document = $document;
    $this->registered = array();
  }

  /**
   * Allows iteration over the registered DOMNodes.
   * 
   * @return ArrayIterator 
   */
  public function getIterator() {
    return new ArrayIterator($this->registered);
  }

  /**
   * Determine what member variables to serialize.
   * 
   * @return array
   */
  public function __sleep() {
    $this->paths = $this->registered;
    array_walk($this->paths, array('Utils', 'convertNodeToPath'));
    return array('paths', 'document');
  }

  /**
   * Reinitialize this object after it has be unserialized.
   */
  public function __wakeup() {
    $this->registered = array();
    foreach ($this->paths as $key => $path) {
      $result = $this->document->query($path);
      $this->registered[$key] = $result->item(0);
    }
  }

  /**
   * Register a relationship between a FormElement and a DOMNode.
   * 
   * @param FormElement $element
   * @param DOMNode $node 
   */
  public function register(FormElement $element, DOMNode $node) {
    $key = $element->hash;
    $this->registered[$key] = $node;
  }

  /**
   * Is the given DOMNode registered?
   * 
   * @param DOMNode $node 
   * @return boolean
   *   TRUE if the given DOMNode is already registered 
   */
  public function isNodeRegistered(DOMNode $node) {
    $hash = $this->getElementHash($node);
    if (isset($hash)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Find the hash for the element that corresponds to the given DOMNode.
   * 
   * @param DOMNode $node
   * @return hash
   */
  public function getElementHash(DOMNode $node) {
    $result = array_filter($this->registered, array($node, 'isSameNode'));
    if (count($result) == 1) {
      $keys = array_keys($result);
      return array_shift($keys);
    }
    return NULL;
  }

  /**
   * Is the given FormElement registered?
   * 
   * @param mixed $element 
   *   Either a FormElement, or a FormElements hash.
   * @return boolean
   *   TRUE if the element is registered FALSE otherwise.
   */
  public function isElementRegistered($element) {
    return $this->getNode($element) != NULL;
  }

  /**
   * Get the registered DOMNode associated with the given FormElement.
   * 
   * @param mixed $hash
   *   Either a FormElement, or a FormElements hash.
   * @return DOMNode
   */
  public function getNode($element) {
    $hash = FormElement::isFormElement($element) ?
        $element->hash :
        $element;
    if (isset($this->registered[$hash])) {
      return $this->registered[$hash];
    }
    return NULL;
  }

}