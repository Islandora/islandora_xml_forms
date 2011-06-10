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
    $this->path = new Path($params['path'], $params['context']);
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
  public function shouldExecute(NodeRegistry $registry, FormElement $element, $value = NULL) {
    return isset($value) && $value == '';
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

  /**
   * 
   * @return DOMElement
   */
  public function asNode(DOMDocument $document) {
    $delete = $document->createElement('delete');
    if (isset($this->path)) {
      $delete->appendChild($this->path->asNode());
    }
    return $delete;
  }

}