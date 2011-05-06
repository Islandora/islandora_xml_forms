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
class Update implements ActionInterface {

  /**
   *
   * @var Path 
   */
  protected $path;
  /**
   *
   * @var string 
   */
  protected $schemaName;
  /**
   *
   * @var string
   */
  protected $schemaPath;

  /**
   *
   * @param string $xml_definition
   * @return Update
   */
  public static function createFromXMLDefiniton($xml_definition) {
    return self::createFromSimpleXML(simplexml_load_string($xml_definition));
  }

  /**
   *
   * @param SimpleXMLElement $update
   * @return Update
   */
  public static function createFromSimpleXML(SimpleXMLElement $update) {
    $path = (string) $update->path;
    $context = (string) $update->path['context'];
    $schema_path = isset($update->schema_path) ? (string) $update->schema_path : null;
    $schema_name = isset($update->schema_path) ? (string) $update->schema_path['name'] : null;
    $params = array(
      'path' => new Path($path, $context),
      'schema_name' => $schema_name,
      'schema_path' => $schema_path
    );
    return new self($params);
  }

  /**
   *
   * @param array $params 
   */
  public function __construct(array &$params) {
    $this->path = $params['path'];
    $this->schemaName = isset($params['schema_name']) ? $params['schema_name'] : null;
    $this->schamePath = isset($params['schema_path']) ? $params['schema_path'] : null;
    if (!$this->valid()) {
      throw new Exception('Failed to initialize Update.');
    }
  }

  /**
   * Determine if this object has been initialized correctly.
   * 
   * @return boolean
   *   TRUE if this object is valid FALSE otherwise.
   */
  public function valid() {
    return isset($path);
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
      $this->update($node, $value);
    }
  }

  /**
   * Update the given DOMNode with the provided mixed value.
   * 
   * @param DOMNode $node
   * @param mixed $value 
   */
  private function update(DOMNode $node, $value) {
    $node->nodeValue = $value;
  }

}