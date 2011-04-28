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
  protected $element;
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
   * @param SimpleXMLElement $create 
   */
  public function __construct(SimpleXMLElement $create) {
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
  protected $path;

  /**
   *
   * @param SimpleXMLElement $read 
   */
  public function __construct(SimpleXMLElement $read) {
    
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
   * @param SimpleXMLElement $update 
   */
  public function __construct(SimpleXMLElement $update) {
    $this->path = (string) $update->path;
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

  /**
   *
   * @param SimpleXMLElement $delete 
   */
  public function __construct(SimpleXMLElement $delete) {
    
  }

}

/**
 * XML properties for a single form element.
 */
class XMLProperties {

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
  public function __construct(SimpleXMLElement $xml_properties) {
    if (isset($xml_properties->create)) {
      $this->create = new XMLCreateProperties($xml_properties->create);
    }
    if (isset($xml_properties->read)) {
      $this->read = new XMLReadProperties($xml_properties->read);
    }
    if (isset($xml_properties->update)) {
      $this->update = new XMLUpdateProperties($xml_properties->update);
    }
    if (isset($xml_properties->delete)) {
      $this->delete = new XMLDeleteProperties($xml_properties->delete);
    }
  }

}