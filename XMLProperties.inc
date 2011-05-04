<?php

// $Id$

/**
 * @file
 *
 */

/**
 * Defines the set of properties that allow us to create an XML element.
 */
class XMLCreateProperties {
  /**
   * Create types.
   */
  const Element = 0;
  const Attribute = 1;
  const XML = 2;
  /**
   * 
   */
  public $element;
  /**
   * 
   * @var DocumentPath
   */
  protected $path;
  /**
   * 
   */
  protected $schema;
  /**
   *
   * @var type 
   */
  protected $type;
  /**
   *
   * @var string
   */
  protected $create;
  /**
   *
   * @var string 
   */
  protected $prefix;

  /**
   *
   * @param FormElement $element
   * @param SimpleXMLElement $create 
   */
  public function __construct(FormElement $element, SimpleXMLElement $create) {
    $this->element = $element;
    $this->path = new DocumentPath($element, (string) $create->path, (string) $create->path['context']);
    $this->schema = isset($create->schema_path) ? (string) $create->schema_path : null;
    $this->setCreateType($create);
  }

  /**
   *
   * @param SimpleXMLElement $element 
   */
  private function setCreateType(SimpleXMLElement $create) {
    if (isset($create->element)) {
      $this->create = (string) $create->element;
      $this->prefix = (string) $create->element['prefix'];
      $this->type = self::Element;
    }
    else if (isset($create->attribute)) {
      $this->create = (string) $create->attribute;
      $this->prefix = (string) $create->attribute['prefix'];
      $this->type = self::Attribute;
    }
    else {
      $this->create = (string) $create->xml;
      $this->type = self::XML;
    }
  }

  public function shouldCreate($value) {
    return TRUE;
  }

  public function create(Document $document, $value) {
    switch ($this->type) {
      case self::Element:
        $results = $this->path->query($document);
        if ($results) {
          $count = $results->length;
          for ($i = 0; $i < $count; $i++) {
            $node = $results->item($i);
            $element = $this->createElement($document, $value);
            $node->appendChild($element);
          }
        }
        break;
      case self::Attribute:

        break;
      case self::XML:

        break;
    }
  }

  private function createElement(Document $document, $value) {
    $name = isset($this->prefix) ? "{$this->prefix}:{$this->create}" : $this->create;
    $name = $this->create;
    $namespace = $document->getNamespace($this->prefix);
    return isset($namespace) ?
        new DOMElement($name, $value, $namespace) :
        new DOMElement($name, $value);
  }

  private function appendElement(DOMNode $node) {

    $node->appendChild($element);
  }

  private function createAttribute() {
    
  }

  private function appendAttribute(DOMNode $node) {
    
  }

  private function createXML() {
    
  }

  private function appendXML(Document $document, DOMNode $node) {
    
  }

}

/**
 * 
 */
class XMLReadProperties {

  /**
   * Path used to locate the form element.
   * 
   * @var string 
   */
  public $element;
  /**
   * 
   * @var DocumentPath 
   */
  public $path;

  /**
   *
   * @param FormElement $element
   * @param SimpleXMLElement $read 
   */
  public function __construct(FormElement $element, SimpleXMLElement $read) {
    $this->element = $element;
    $this->path = new DocumentPath($element, (string) $read->path, (string) $read->path['context']);
  }

  /**
   *
   * @param Document $document
   * @return array
   */
  public function read(Document $document) {
    $results = $this->path->query($document);
    if ($results) {
      $count = $results->length;
      for ($i = 0; $i < $count; $i++) {
        $node = $results->item($i);
        $contains_elements = $document->query('child::*', $node)->length > 0;
        $value = $contains_elements ? NULL : $node->nodeValue;
        $output[] = array($node, $value);
      }
    }
    return isset($output) ? $output : NULL;
  }

}

/**
 * 
 */
class XMLUpdateProperties {

  /**
   * Path used to locate the form element.
   * 
   * @var string 
   */
  protected $path;
  /**
   *
   * @var FormElement
   */
  public $element;

  /**
   *
   * @param SimpleXMLElement $update 
   */
  public function __construct(FormElement $element, SimpleXMLElement $update) {
    $this->element = $element;
    $this->path = new DocumentPath($element, (string) $update->path, (string) $update->path['context']);
  }

  /**
   *
   * @param Document $document
   * @return array
   */
  public function update(Document $document, $value) {
    $results = $this->path->query($document);
    if ($results) {
      $count = $results->length;
      for ($i = 0; $i < $count; $i++) {
        $node = $results->item($i);
        $this->updateNode($node, $value);
      }
    }
  }

  /**
   *
   * @param DOMNode $node
   * @param type $value 
   */
  private function updateNode(DOMNode $node, $value) {
    $class = get_class($node);
    switch ($class) {
      case 'DOMAttr':
        $node->value = (string) $value;
        break;
      case 'DOMElement':
        $node->nodeValue = (string) $value;
        break;
      default:
        throw new Exception('Unknown Node Type');
    }
  }

  /**
   *
   * @return type 
   */
  public function shouldUpdate() {
    return TRUE;
  }

}

/**
 * 
 */
class XMLDeleteProperties {

  /**
   * If defined it is used as a path to the element that we want to delete. This path is excuted in the selected node's 
   * context.
   * 
   * @param DOMElement $delete_properties 
   */
  protected $parentPath;
  public $element;

  /**
   *
   * @param SimpleXMLElement $delete 
   */
  public function __construct(FormElement $element, SimpleXMLElement $delete) {
    $this->element = $element;
    $this->path = new DocumentPath($element, (string) $delete->path, (string) $delete->path['context']);
  }

  /**
   *
   * @param mixed $value 
   */
  public function shouldDelete($value) {
    return $value == NULL || $value == '';
  }

}

/**
 * XML properties for a single form element.
 */
class XMLProperties {

  /**
   *
   * @var FormElement 
   */
  public $element;
  /**
   *
   * @var XMLCreateProperties 
   */
  public $create;
  /**
   *
   * @var XMLReadProperties 
   */
  public $read;
  /**
   *
   * @var XMLUpdateProperties 
   */
  public $update;
  /**
   *
   * @var XMLDeleteProperties 
   */
  public $delete;

  /**
   *
   * @param SimpleXMLElement $xml_properties 
   */
  public function __construct(FormElement $element, SimpleXMLElement $properties) {
    $this->element = $element;
    if (isset($properties->create)) {
      $this->create = new XMLCreateProperties($element, $properties->create);
    }
    if (isset($properties->read)) {
      $this->read = new XMLReadProperties($element, $properties->read);
    }
    if (isset($properties->update)) {
      $this->update = new XMLUpdateProperties($element, $properties->update);
    }
    if (isset($properties->delete)) {
      $this->delete = new XMLDeleteProperties($element, $properties->delete);
    }
  }

}