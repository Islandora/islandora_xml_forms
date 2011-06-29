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
   * The namespace for the Element or Attribute we are creating.
   * @var string
   */
  protected $prefix;
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
    $this->path = new Path($params['path'], $params['context']);
    $this->schema_path = isset($params['schema']) ? $params['schema'] : NULL;
    $this->type = $params['type'];
    $this->prefix = isset($params['prefix']) ? $params['prefix'] : NULL;
    $this->value = $params['value'];
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
    }
  }

  /**
   *
   * @param Document $document
   * @param mixed $value
   * @return DOMNode 
   */
  private function create(XMLDocument $document, $value) {
    $action = array(self::Element => 'createElement', self::Attribute => 'createAttribute', self::XML => 'createXML');
    return call_user_method($action[$this->type], $this, $document, $value);
  }

  /**
   *
   * @param Document $document
   * @param mixed $value
   * @return DOMElement
   */
  private function createElement(XMLDocument $document, $value) {
    $namespace = $document->getNamespace($this->prefix);
    $name = isset($namespace) ? "{$this->prefix}:{$this->value}" : $this->value;
    return isset($namespace) ?
        new DOMElement($name, $value, $namespace) :
        new DOMElement($name, $value);
  }

  /**
   *
   * @param Document $document
   * @param mixed $value
   * @return DOMAttr 
   */
  private function createAttribute(XMLDocument $document, $value) {
    $namespace = $document->getNamespace($this->prefix);
    $doc = $document->document;
    $name = isset($namespace) ? "{$this->prefix}:{$this->create}" : $this->value;
    $attribute = isset($namespace) ?
        $doc->createAttributeNS($namespace, $name) :
        $doc->createAttribute($name);
    $attribute->value = $value;
    return $attribute;
  }

  /**
   *
   * @param Document $document
   * @param DOMElement $value 
   */
  private function createXML(XMLDocument $document, $value) {
    // @todo Implement Me !!!!
  }

  /**
   *
   * @param DOMNode $child
   * @param DOMElement $parent 
   */
  private function append(XMLDocument $document, DOMNode $child, DOMElement $parent) {
    if (isset($document->schema)) {
      $insert_order = new InsertOrderMap($document->schema);
      $insert_order->insert($this->schema_path, $parent, $child);
    }
    else {
      $parent->appendChild($child);
    }
  }

  /**
   * 
   * @return DOMElement
   */
  public function asNode(DOMDocument $document) {
    $create = $document->createElement('create');
    if (isset($this->path)) {
      $create->appendChild($this->path->asNode());
    }
    // @todo Add schema info to the out put...
    switch ($this->type) {
      case Create::Attribute:
        $attribute = $document->createElement('attribute', $this->value);
        if (isset($this->prefix)) {
          $attribute->setAttribute('prefix', $this->prefix);
        }
        $create->appendChild($attribute);
        break;
      case Create::Element:
        $element = $document->createElement('element', $this->value);
        if (isset($this->prefix)) {
          $element->setAttribute('prefix', $this->prefix);
        }
        $create->appendChild($element);
        break;
      case Create::XML:
        $xml = $document->createElement('xml', $this->value);
        $create->appendChild($xml);
        break;
    }
    return $create;
  }

}