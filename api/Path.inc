<?php



/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'Context');

/**
 * A class used to store/query XPath's.
 */
class Path {

  /**
   * XPath to query with.
   *
   * @var string
   */
  public $path;

  /**
   * Query Context, either document, parent or self.
   *
   * @var string
   */
  public $context;

  /**
   * Creates a Path instance.
   *
   * @param string $path
   * @param Context $context
   */
  public function __construct($path, Context $context) {
    $this->path = $path;
    $this->context = $context;
  }

  /**
   * Queries the path with the defined context as defined by the provided FormElement
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   *
   * @throws
   * @see Context::getNode()
   *
   * @return DOMNodeList
   */
  public function query(XMLDocument $document, FormElement $element) {
    $context = $this->context->getNode($document, $element);
    return $document->query($this->path, $context);
  }

  /**
   *
   */
  public function __toString() {
    return $this->path;
  }

}
