<?php



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
    $this->path = new Path($params['path'], new Context(new ContextType($params['context'])));
  }

  /**
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
   * @return boolean
   */
  public function shouldExecute(XMLDocument $document, FormElement $element, $value = NULL) {
    return TRUE; // Its always safe to read!
  }

  /**
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   */
  public function execute(XMLDocument $document, FormElement $element, $value = NULL) {
    //drupal_set_message('Read (Action)');
    //drupal_set_message('&nbsp;->Location: ' . $element->getLocation());
    //drupal_set_message('&nbsp;->' . $this->path);
    try {
      $results = $this->path->query($document, $element);
    }
    catch (XMLFormsContextNotFoundException $e) {
      /**
       * Ignore context not found, in this case the context node
       * should be created during the submit process.
       */
      $results = new DOMNodeList();
    }
    /* if ($results->length) {
     $result = $results->item(0);
     drupal_set_message('&nbsp;->Result: ' . $result->tagName);
     }
     else {
     drupal_set_message('&nbsp;->Result: None');
     } */
    return $results;
  }

}
