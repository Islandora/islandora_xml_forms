<?php

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'ActionInterface');
module_load_include('inc', 'xml_form_api', 'Path');
module_load_include('inc', 'xml_schema_api', 'InsertOrderMap');

/**
 *
 */
class Create implements Action {
  /**
   * Create types.
   */
  const Element = 'element';
  const Attribute = 'attribute';
  const XML = 'xml';

  /**
   * Path to the parent element, where the DOMNode will be created.
   *
   * @var Path
   */
  protected $path;

  /**
   * Path the this elements definition in the schema.
   *
   * @var path
   */
  protected $schema_path;

  /**
   * Either Create::Element, Create::Attribute, Create::XML
   * @var integer
   */
  protected $type;

  /**
   * The full name for the Element or Attribute we are creating.
   * @var string
   */
  protected $name;

  /**
   * The namespace prefix for the Element or Attribute we are creating.
   * @var string
   */
  protected $prefix;

  /**
   * The local name for the Element or Attribute we are creating.
   * @var string
   */
  protected $local_name;

  /**
   * The XML Snippit we are creating.
   * @var string
   */
  protected $xml;

  /**
   * The value used to create the Element, Attribute or XML Snippit.
   * @var string
   */
  protected $value;

  /**
   *
   * @param array $params
   */
  public function __construct(array &$params) {
    $this->path = new Path($params['path'], new Context(new ContextType($params['context'])));
    $this->schema_path = isset($params['schema']) ? $params['schema'] : NULL;
    $this->value = $params['value'];
    $this->type = $params['type'];
    switch ($this->type) {
      case self::XML:
        $this->xml = $this->value;
        break;
      case self::Element:
      case self::Attribute:
        $this->name = $this->value;
        $tag_name = explode(':', $this->name);
        if (count($tag_name) > 1) {
          list($this->prefix, $this->local_name) = $tag_name;
        }
        else {
          list($this->local_name) = $tag_name;
        }
        break;
    }
  }

  /**
   *
   * @return array
   */
  public function toDrupalForm() {
    return array(
      'path' => $this->path->path,
      'context' => (string) $this->path->context,
      'schema' => $this->schema_path,
      'type' => $this->type,
      'prefix' => $this->prefix,
      'value' => $this->value,
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
    /**
     * @todo add additonal parameters to determine if an element should be created.
     * At the moment we create elements if it did not exist when the form was first rendered.
     */
    return $document->registry->isRegistered($element->hash) === FALSE;
  }

  /**
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   */
  public function execute(XMLDocument $document, FormElement $element, $value = NULL) {
    $results = $this->path->query($document, $element);
    if ($results && $results->length > 0) {
      $value = isset($value) ?
          htmlspecialchars(html_entity_decode($value, ENT_QUOTES, 'UTF-8'), ENT_QUOTES, 'UTF-8') :
          NULL; // Filter the value.
      $parent = $results->item(0);
      $created = $this->doCreate($document, $value);
      $this->append($document, $created, $parent);
      $document->registry->register($element->hash, $created);
      return TRUE;
    }
    return FALSE;
  }

  /**
   *
   * @param Document $document
   * @param mixed $value
   * @return DOMNode
   */
  private function doCreate(XMLDocument $document, $value) {
    $action = array(
      self::Element => 'createElement',
      self::Attribute => 'createAttribute',
      self::XML => 'createXML',
    );
    try {
      return $this->$action[$this->type]($document, $value);
    }
    catch (Exception $e) {
      $value = htmlentities($this->value);
      $path = (string) $this->path;
      $context = (string) $this->path->context;
      $msg = "Failed to create {$this->type}. Value: {$value}.</br> Path: {$path}<br/>Context: {$context}</br> Error: {$e->getMessage()}";
      throw new Exception($msg);
    }
  }

  /**
   *
   * @param Document $document
   * @param mixed $value
   * @return DOMElement
   */
  private function createElement(XMLDocument $document, $value) {
    if (isset($this->prefix)) {
      $uri = $document->namespaces->getURI($this->prefix);
      return $document->document->createElementNS($uri, $this->name, $value);
    }
    else {
      $uri = $document->namespaces->getDefaultURI();
      return (isset($uri)) ?
          $document->document->createElementNS($uri, $this->name, $value) :
          $document->document->createElement($this->name, $value);
    }
  }

  /**
   *
   * @param Document $document
   * @param mixed $value
   * @return DOMAttr
   */
  private function createAttribute(XMLDocument $document, $value) {
    if (isset($this->prefix)) {
      $uri = $document->namespaces->getURI($this->prefix);
      $attribute = $document->document->createAttributeNS($uri, $this->name);
      $attribute->value = $value;
      return $attribute;
    }
    else {
      $attribute = $document->document->createAttribute($this->name);
      $attribute->value = $value;
      return $attribute;
    }
  }

  /**
   *
   * @param Document $document
   * @param DOMElement $value
   */
  private function createXML(XMLDocument $document, $value) {
    // Sigh... Oh DOM-API, your so backwards and useless.
    // We have to do this for namespaces to work correctly, there is no clean work around for this.
    // User will have to put both the prefix and namespace on elements where they want the prefix to stay.
    $root_attributes = '';
    foreach ($document->namespaces as $prefix => $uri) {
      $attribute = is_non_empty_string($prefix) ?
          "xmlns:$prefix" :
          "xmlns";
      $root_attributes .= "$attribute = '$uri' ";
    }
    $xml = strtr($this->value, array('%value%' => $value));
    $xml = "<root {$root_attributes}>$xml</root>";
    $this->fragment = $document->document->createDocumentFragment();
    libxml_use_internal_errors(TRUE); // Prevent Drupal from catching the error.
    $number_of_errors_before = count(libxml_get_errors());
    $this->fragment->appendXML($xml);
    $number_of_errors_after = count(libxml_get_errors());
    libxml_use_internal_errors(FALSE); // Allow Drupal to catch errors
    if ($number_of_errors_after > $number_of_errors_before) {
      $error = libxml_get_last_error();
      throw new Exception($error->message);
    }
    $root = $this->fragment->firstChild->firstChild;
    return $root;
  }

  /**
   *
   * @param DOMNode $child
   * @param DOMElement $parent
   */
  private function append(XMLDocument $document, DOMNode $child, DOMElement $parent) {
    $has_schema = isset($document->schema);
    $has_schema_path = isset($this->schema_path) && trim($this->schema_path) != '';
    if ($has_schema && $has_schema_path) {
      $insert_order = new InsertOrderMap($document->schema);
      $insert_order->insert($this->schema_path, $parent, $child);
    }
    else {
      $parent->appendChild($child);
    }
  }

}
