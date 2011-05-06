<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'Actions');

/**
 * 
 */
class Create implements Action {
  /**
   * Create types.
   */
  const Element = 0;
  const Attribute = 1;
  const XML = 2;

  /**
   * Path to the parent element, where the DOMNode will be created.
   * 
   * @var Path
   */
  protected $path;
  /**
   * Name of the schema that contains the definition for this element.
   * 
   * @var path
   */
  protected $schemaName;
  /**
   * Path to this elements schema definition.
   * 
   * @var path
   */
  protected $schemaPath;
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
   *
   * @param string $xml_definition
   * @return Create
   */
  public static function createFromXMLDefiniton($xml_definition) {
    return self::createFromSimpleXML(simplexml_load_string($xml_definition));
  }

  /**
   *
   * @param SimpleXMLElement $create 
   * @return Create
   */
  public static function createFromSimpleXML(SimpleXMLElement $create) {
    $path = (string) $create->path;
    $context = (string) $create->path['context'];
    $schema_path = isset($create->schema_path) ? (string) $create->schema_path : null;
    $schema_name = isset($create->schema_path) ? (string) $create->schema_path['name'] : null;
    $type = self::getCreateType($create);
    $prefix = self::getPrefix($create, $type);
    $params = array(
      'path' => new Path($path, $context),
      'type' => $type,
      'prefix' => $prefix,
      'schema_name' => $schema_name,
      'schema_path' => $schema_path
    );
    return new self($params);
  }

  /**
   * Get the type of DOMNode to create.
   * 
   * @param SimpleXMLElement $create
   * @return integer
   *   Either Create::Element, Create::Attribute, Create::XML, or NULL if $create is not valid. 
   */
  public static function getCreateType(SimpleXMLElement $create) {
    $types = array('element' => self::Element, 'attribute' => self::Element, 'xml' => self::XML);
    foreach ($types as $key => $type) {
      if (isset($create->$key)) {
        return $type;
      }
    }
    return NULL;
  }

  /**
   * Construct Create Action.
   * 
   * @param array $params
   *   The parameters that initialize this object.
   */
  public function __construct(array &$params) {
    $this->path = $params['path'];
    $this->type = $params['type'];
    $this->context = $params['context'];
    $this->schemaName = isset($params['schema_name']) ? $params['schema_name'] : null;
    $this->schamePath = isset($params['schema_path']) ? $params['schema_path'] : null;
    if ($this->valid()) {
      throw new Exception('Failed to initialize Create.');
    }
  }

  /**
   * Determine if this object has been initialized correctly.
   * 
   * @return boolean
   *   TRUE if this object is valid FALSE otherwise.
   */
  public function valid() {
    return!(empty($this->path) || empty($this->type));
  }

  /**
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value 
   */
  public function execute(NodeRegistry $registry, FormElement $element, $value) {
    $results = $this->path->query($registry, $element);
    if ($results) {
      $parent = $results->item(0);
      $created = $this->create($registry->document, $value);
      $this->append($created, $parent);
    }
  }

  /**
   *
   * @param Document $document
   * @param mixed $value
   * @return DOMNode 
   */
  private function create(Document $document, $value) {
    $action = array(self::Element => 'createElement', self::Attribute => 'createAttribute', self::XML => 'createXML');
    return call_user_method($action[$this->type], $this, $document, $value);
  }

  /**
   *
   * @param Document $document
   * @param mixed $value
   * @return DOMElement
   */
  private function createElement(Document $document, $value) {
    $namespace = $document->getNamespace($this->prefix);
    $name = isset($namespace) ? "{$this->prefix}:{$this->create}" : $this->create;
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
  private function createAttribute(Document $document, $value) {
    $namespace = $document->getNamespace($this->prefix);
    $doc = $document->document;
    $name = isset($namespace) ? "{$this->prefix}:{$this->create}" : $this->create;
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
  private function createXML(Document $document, $value) {
    // @todo Implement Me !!!!
  }

  /**
   *
   * @param DOMNode $child
   * @param DOMElement $parent 
   */
  private function append(DOMNode $child, DOMElement $parent) {
    // @todo Use the schema element to insert the element at the proper spot.
    $parent->appendChild($child);
  }

}