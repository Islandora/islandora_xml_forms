<?php

// $Id$

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
  public function getValue() {
    return array(
      'path' => $this->path->path,
      'context' => $this->path->context,
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
    $results = $this->path->query($registry, $element);
    if ($results && $results->length > 0) {
      $document = $registry->document;
      $parent = $results->item(0);
      $created = $this->create($document, $value);
      $this->append($document, $created, $parent);
      $registry->register($element->hash, $created);
      return true;
    }
    return false;
  }

  /**
   *
   * @param Document $document
   * @param mixed $value
   * @return DOMNode 
   */
  private function create(XMLDocument $document, $value) {
    $action = array(self::Element => 'createElement', self::Attribute => 'createAttribute', self::XML => 'createXML');
    try {
      return $this->$action[$this->type]($document, $value);
    } catch (Exception $e) {
      $value = htmlentities($this->value);
      $msg = "Failed to create {$this->type} with value {$value} . Error: {$e->getMessage()}";
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
      $uri = $document->getNamespaceURI($this->prefix);
      if (empty($uri)) {
        $msg = "Failed to find namespace uri for the prefix $this->prefix, used by the element $this->name. Please define this namespace in the form properties.";
        throw new Exception($msg);
      }
      return $document->document->createElementNS($uri, $this->name, $value);
    }
    else {
      $uri = $document->getNamespaceURI(); // Get Default URI.
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
      $uri = $document->getNamespaceURI($this->prefix);
      if (empty($uri)) {
        $msg = "Failed to find namespace uri for the prefix $this->prefix, used by the attribute $this->name. Please define this namespace in the form properties.";
        throw new Exception($msg);
      }
      $attribute = $document->document->createAttributeNS($uri, $this->name);
      $attribute->value = $value;
      return $attribute;
    }
    else {
      $uri = $document->getNamespaceURI(); // Get Default URI.
      $attribute = isset($uri) ?
          $document->document->createAttributeNS($uri, $this->name) :
          $document->document->createAttribute($this->name);
      $attribute->value = $value;
      return $attribute;
    }
    /*
      $namespace = $document->getNamespaceURI($this->prefix);
      $doc = $document->document;
      $name = isset($namespace) ? "{$this->prefix}:{$this->create}" : $this->value;
      $attribute = isset($namespace) ?
      $doc->createAttributeNS($namespace, $name) :
      $doc->createAttribute($name);
      $attribute->value = $value;
      return $attribute;
     */
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
    $default_uri = $document->getNamespaceURI();
    $root_attributes = ($default_uri) ? "xmlns = '$default_uri' " : '';
    foreach ($document->namespaces as $prefix => $uri) {
      $attribute = is_non_empty_string($prefix) ?
          "xmlns:$prefix" :
          "xmlns";
      $root_attributes .= "$attribute = '$uri' ";
    }
    $xml = strtr($this->value, array('%value%' => $value));
    $xml = "<root {$root_attributes}>$xml</root>";
    $this->fragment = $document->document->createDocumentFragment();
    $this->fragment->appendXML($xml);
    $default_uri = $document->getNamespaceURI();
    return $this->fragment->firstChild->firstChild;
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