<?php



/**
 * @file
 *
 * Defines a Class for associating DOMNodes with the 'self' context of FormElements.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');

/**
 * Registers the DOMNodes that defined the 'self' context of FormElements. A FormElement can have at most one DOMNode
 * repersenting its 'self' context, but a DOMNode can repersent the 'self' context for multiple FormElements.
 */
class NodeRegistry implements Serializable {

  /**
   * A map of FormElements to the DOMNodes that repersent the FormElements 'self' context.
   * Where the keys of the array are the unique #hash property of a FormElement, and the value is the DOMNode
   * repersenting the 'self' context of the FormElement.
   *
   * When this object is serialized the registered DOMNodes are converted to xpaths (since we can't serialize resources).
   * Upon unserializing this object NodeRegistry::restored() must be called to convert the xpaths back to DOMNodes.
   *
   * @var array
   */
  protected $registered;

  /**
   * TRUE if this object needs to be restored. FALSE otherwise.
   *
   * To restore this object call the NodeRegistry::restored() function.
   *
   * @var boolean
   */
  protected $restoredRequired;

  /**
   * Creates an instance of the NodeRegistry.
   */
  public function __construct() {
    $this->registered = array();
    $this->restoredRequired = FALSE;
  }

  /**
   * Serializes this object.
   *
   * The registered DOMNodes are converted into xpaths, since DOMNodes cannot be serialized.
   *
   * @return string
   */
  public function serialize() {
    $data = array();
    foreach ($this->registered as $key => $node) {
      $data[$key] = get_dom_node_xpath($node);
    }
    return serialize($data);
  }

  /**
   * Unserializes this object.
   *
   * Note that for this object to be fully functional NodeRegistry::restore() must also be called after unserialization.
   *
   * @param string $serialized
   */
  public function unserialize($serialized) {
    $this->registered = unserialize($serialized);
    $this->restoredRequired = TRUE;
  }

  /**
   * Restores the registry, querying the registered xpaths and replacing them with the actual DOMNodes
   * that they repersent.
   *
   * @param DOMDocument $document
   */
  public function restore(DOMDocument $document) {
    if ($this->restoredRequired) {
      $xpath = new DOMXPath($document);
      foreach ($this->registered as $key => $path) {
        $result = $xpath->query($path);
        if ($result->length == 1) {
          $this->registered[$key] = $result->item(0);
        }
        else {
          throw new Exception('Fatal error: Failed to restore DOMNode from stored path. Key: ' . $key . ' Path: ' . $path);
        }
      }
      $this->restoredRequired = FALSE;
    }
  }

  /**
   * Registers a 'self' context relationship between a FormElement identified by its
   * unique #hash property and the given DOMNode.
   *
   * @param hash $hash
   * @param DOMNode $node
   */
  public function register($hash, DOMNode $node) {
    if ($this->restoredRequired) {
      throw new Exception("DOMNodes can not be registered until this object has been restored. Make sure 'NodeRegistry::restore()' has been called after this object has been unserialized.");
    }
    $this->registered[$hash] = $node;
  }

  /**
   * Checks if a the given FormElement is registered.
   *
   * @param hash $hash
   *
   * @return boolean
   *   TRUE if the element is registered FALSE otherwise.
   */
  public function isRegistered($hash) {
    return isset($this->registered[$hash]);
  }

  /**
   * Get the registered DOMNode associated with the given FormElement.
   *
   * @param hash $hash
   * @return DOMNode
   */
  public function get($hash) {
    if ($this->restoredRequired) {
      throw new Exception("DOMNodes can not be retrieved until this object has been restored. Make sure 'NodeRegistry::restore()' has been called after this object has been unserialized.");
    }
    return $this->isRegistered($hash) ? $this->registered[$hash] : NULL;
  }

  /**
   * Gets all the registered DOMNodes.
   *
   * @return array
   *   An array containing all registered DOMNodes where the array keys are the
   *   FormElement's hashes and the values are the registered DOMNodes.
   */
  public function getRegistered() {
    if ($this->restoredRequired) {
      throw new Exception("DOMNodes can not be retrieved until this object has been restored. Make sure 'NodeRegistry::restore()' has been called after this object has been unserialized.");
    }
    return $this->registered;
  }

}
