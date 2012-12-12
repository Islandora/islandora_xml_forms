<?php

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'ActionInterface');
module_load_include('inc', 'xml_form_api', 'Path');
module_load_include('inc', 'php_lib', 'DOMHelpers.inc');

/**
 *
 */
class Update implements Action {

  /**
   * Path to the element to update.
   *
   * @var Path
   */
  protected $path;

  /**
   * Path to the element's definition within its schema.
   *
   * @var string
   */
  protected $schema;

  /**
   *
   * @param array $params
   */
  public function __construct(array &$params) {
    $this->path = new Path($params['path'], new Context(new ContextType($params['context'])));
    $this->schema = isset($params['schema']) ? $params['schema'] : NULL;
  }

  /**
   * Used by the form builder???
   *
   * @return array
   */
  public function toDrupalForm() {
    return array(
      'path' => $this->path->path,
      'context' => (string) $this->path->context,
    );
  }

  /**
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   *
   * @return boolean
   */
  public function shouldExecute(XMLDocument $document, FormElement $element, $value = NULL) {
    return isset($value);
  }

  /**
   * Update the document.
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   */
  public function execute(XMLDocument $document, FormElement $element, $value = NULL) {
    $value = isset($value) ?
        htmlspecialchars(html_entity_decode($value, ENT_QUOTES, 'UTF-8'), ENT_QUOTES, 'UTF-8') :
        NULL; // Filter the value.
    $results = $this->path->query($document, $element);
    $results = dom_node_list_to_array($results);
    foreach ($results as $node) {
      $this->doUpdate($node, $value);
    }
    return TRUE;
  }

  /**
   * Update the given DOMNode with the provided mixed value.
   *
   * @param DOMNode $node
   * @param mixed $value
   */
  private function doUpdate(DOMNode $node, $value) {
    $node->nodeValue = $value;
  }

}
