<?php

// $Id$

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
  public function getValue() {
    return array(
      'path' => $this->path->path,
      'context' => $this->path->context,
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
    return $document->registry->isElementRegistered($element->hash); // Under what cases should we not update an existing value? It must have been read first.
  }

  /**
   * Update the document.
   * 
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value 
   */
  public function execute(XMLDocument $document, FormElement $element, $value = NULL) {
    $results = $this->path->query($document, $element);
    if ($results !== FALSE) {
      $results = dom_node_list_to_array($results);
      foreach ($results as $node) {
        $this->update($node, $value);
      }
      return true;
    }
    else {
      throw new Exception("Failed to update {$element->getIndex()} <br/>path: {$this->path->path}<br/>context: {$this->path->context}");
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