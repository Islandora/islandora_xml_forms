<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'ActionInterface');
module_load_include('inc', 'xml_form_api', 'Path');
module_load_include('inc', 'php_lib', 'DOMHelpers');

/**
 * 
 */
class Delete implements Action {

  /**
   *
   * @var Path 
   */
  protected $path;

  /**
   *
   * @param array $params 
   */
  public function __construct(array &$params) {
    $this->path = new Path($params['path'], new Context(new ContextType($params['context'])));
  }

  /**
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
   * @param type $value
   * 
   * @return boolean
   */
  public function shouldExecute(XMLDocument $document, FormElement $element, $value = NULL) {
    return FALSE;//isset($value) && $value == '';
  }

  /**
   *
   * @param XMLDc $registry
   * @param FormElement $element
   * @param mixed $value 
   */
  public function execute(XMLDocument $document, FormElement $element, $value = NULL) {
    $results = $this->path->query($document, $element);
    $results = dom_node_list_to_array($results);
    drupal_set_message('DELETE!!');
    foreach ($results as $node) {
      $this->delete($node);
    }
    return true;
  }

  /**
   *
   * @param DOMNode $node 
   */
  private function delete(DOMNode $node) {
    $node->parentNode->removeChild($node);
  }

}