<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'XMLDocument');
module_load_include('inc', 'php_lib', 'DOMHelpers');

/**
 * Tracks the nodes FormElements repersent.
 */
class NodeRegistry {

  /**
   * A map of FormElements to the DOMNodes the repersent. These nodes are used for populating the form elements
   * as well as a reference to what DOMNodes need to be modified/removed on submit.
   * 
   * @var array
   */
  public $registered;

  /**
   * Create the node registry.
   * 
   * @param Document $document 
   */
  public function __construct() {
    $this->registered = array();
  }

  /**
   * Serialize this object.
   * 
   * @return array
   */
  public function __sleep() {
    return array();
  }

  /**
   * Prepare this object to be unserialized. 
   * 
   * Note that this is not the magic function __sleep, instead this is called from from its owner object.
   * XMLDocument during its __sleep. It should NOT be called from any other location.
   */
  public function export() {
    $paths = array();
    foreach ($this->registered as $key => $node) {
      $paths[$key] = get_dom_node_xpath($node);
    }
    return $paths;
  }

  /**
   * Restore this objects state.
   * 
   * @param DOMDocument $document
   * @param array $paths 
   */
  public function import(DOMDocument $document, array $paths) {
    $xpath = new DOMXPath($document);
    $this->registered = array();
    foreach ($paths as $key => $path) {
      $result = $xpath->query($path);
      if ($result === FALSE) {
        drupal_set_message($path);
      }
      $this->registered[$key] = $result->item(0);
    }
  }

  /**
   * Register a relationship between a FormElement and a DOMNode.
   * 
   * @param hash $hash
   * @param DOMNode $node 
   */
  public function register($hash, DOMNode $node) {
    $name = $node->nodeName;
    $parent = $node->parentNode;
    $this->registered[$hash] = $node;
  }

  /**
   * Is the given DOMNode registered?
   * 
   * @param DOMNode $node 
   * @return boolean
   *   TRUE if the given DOMNode is already registered 
   *
  public function isNodeRegistered(DOMNode $node) {
    $hash = $this->getElementHash($node);
    if (isset($hash)) {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Is the given FormElement registered?
   * 
   * @param hash $hash
   * @return boolean
   *   TRUE if the element is registered FALSE otherwise.
   */
  public function isElementRegistered($hash) {
    return isset($this->registered[$hash]);
  }

  /**
   * Find the hash for the element that corresponds to the given DOMNode.
   * 
   * @param DOMNode $node
   * @return hash
   *
  public function getElementHash(DOMNode $node) {
    $result = array_filter($this->registered, array($node, 'isSameNode'));
    if (count($result) >= 1) {
      $keys = array_keys($result);
      return array_shift($keys);
    }
    return NULL;
  }

  /**
   * Get the registered DOMNode associated with the given FormElement.
   * 
   * @param hash $hash
   * @return DOMNode
   */
  public function getNode($hash) {
    if (isset($this->registered[$hash])) {
      return $this->registered[$hash];
    }
    return NULL;
  }

}