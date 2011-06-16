<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'ActionInterface');
module_load_include('inc', 'xml_form_api', 'Path');

/**
 * 
 */
class Read implements Action {

  /**
   * Path to the parent element, where the DOMNode will be created.
   * 
   * @var Path
   */
  protected $path;

  /**
   * Construct Read Action.
   * 
   * @param array $params
   *   The parameters that initialize this object.
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
   * @param mixed $value
   * @return boolean 
   */
  public function shouldExecute(NodeRegistry $registry, FormElement $element, $value = NULL) {
    return TRUE;
  }

  /**
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value 
   */
  public function execute(NodeRegistry $registry, FormElement $element, $value = NULL) {
    return $this->path->query($registry, $element);
  }

  /**
   * 
   * @return DOMElement
   */
  public function asNode(DOMDocument $document) {
    $read = $document->createElement('read');
    if (isset($this->path)) {
      $read->appendChild($this->path->asNode());
    }
    return $read;
  }

}