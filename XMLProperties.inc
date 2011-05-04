<?php

// $Id$

/**
 * @file
 *
 */

/**
 * XML Create type, enum for action.
 */
class XMLCreateType {
  const __default = 0;
  const Element = 0;
  const Attribute = 1;
  const XML = 2;
}

/**
 * Defines the set of properties that allow us to create an XML element.
 */
class XMLCreateProperties {

  /**
   * 
   */
  public $element;
  /**
   * The xpath to this elements parent element. The newly created xml node will be appended to this 
   * element if specified. If it is not specified a xml node will be expected to be this elements parent.
   * 
   * @var string
   */
  protected $parentPath;
  /**
   * The xpath for selecting this elements definition from a Schema, if defined.
   * @var string
   */
  protected $schemaPath;
  /**
   *
   * @var type 
   */
  protected $type;
  /**
   *
   * @var string 
   */
  protected $attribute;
  /**
   *
   * @var string 
   */
  protected $xml;

  /**
   *
   * @param FormElement $element
   * @param SimpleXMLElement $create 
   */
  public function __construct(FormElement $element, SimpleXMLElement $create) {
    $this->element = $element;
    $this->parentPath = isset($create->parent_path) ? (string) $create->parent_path : null;
    $this->schemaPath = isset($create->schema_path) ? (string) $create->schema_path : null;
    if (isset($create->element)) {
      $this->element = (string) $create->element;
      $this->type = XMLCreateType::Element;
    }
    else if (isset($create->attribute)) {
      $this->attribute = (string) $create->attribute;
      $this->type = XMLCreateType::Attribute;
    }
    else {
      $this->xml = (string) $create->xml;
      $this->type = XMLCreateType::XML;
    }
  }
  
  public function shouldCreate($value) {
    
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
    $this->path = new DocumentPath($element, (string)$read->path, (string)$read->path['context']);
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
    $this->path = new DocumentPath($element, $update->path, $update->path['context']);
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
      case 'DOMElement':
        break;
      case 'DOMNode':
        break;
      default:
        throw new Exception('Invalid DOMNode Type. ' . $class);
    }
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
  }
  
  /**
   *
   * @param mixed $value 
   */
  public function shouldDelete($value) {
    
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
  public function __construct(FormElement $element, SimpleXMLElement $xml_properties) {
    $this->element = $element;
    $create = $xml_properties->create;
    if (isset($create)) {
      $this->create = new XMLCreateProperties($element, $create);
    }
    $read = $xml_properties->read;
    if (isset($read)) {
      $this->read = new XMLReadProperties($element, $read);
    }
    $update = $xml_properties->update;
    if (isset($update)) {
      $this->update = new XMLUpdateProperties($element, $update);
    }
    $delete = $xml_properties->delete;
    if (isset($delete)) {
      $this->delete = new XMLDeleteProperties($element, $delete);
    }
  }

}