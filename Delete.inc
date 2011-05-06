<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'ActionInterface');
module_load_include('inc', 'xml_form_api', 'Path');
module_load_include('inc', 'xml_form_api', 'Utils');

/**
 * 
 */
class Delete implements ActionInterface {

  /**
   *
   * @var Path 
   */
  protected $path;

  /**
   *
   * @param string $xml_definition
   * @return Delete
   */
  public static function createFromXMLDefiniton($xml_definition) {
    return self::createFromSimpleXML(simplexml_load_string($xml_definition));
  }

  /**
   *
   * @param SimpleXMLElement $delete
   * @return Delete
   */
  public static function createFromSimpleXML(SimpleXMLElement $delete) {
    $path = (string) $delete->path;
    $context = (string) $delete->path['context'];
    $params = array(
      'path' => new Path($path, $context),
    );
    return new self($params);
  }

  /**
   *
   * @param array $params 
   */
  public function __construct(array &$params) {
    $this->path = $params['path'];
    if (!$this->valid()) {
      throw new Exception('Failed to initialize Delete.');
    }
  }

  /**
   * Determine if this object has been initialized correctly.
   * 
   * @return boolean
   *   TRUE if this object is valid FALSE otherwise.
   */
  public function valid() {
    return isset($this->path);
  }

  /**
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value 
   */
  public function execute(NodeRegistry $registry, FormElement $element, $value = NULL) {
    $results = $this->path->query($registry, $element);
    $results = Utils::DOMNodelistToArray($results);
    foreach ($results as $node) {
      $this->delete($node);
    }
  }

  /**
   *
   * @param DOMNode $node 
   */
  private function delete(DOMNode $node) {
    $node->parentNode->removeChild($node);
  }

}