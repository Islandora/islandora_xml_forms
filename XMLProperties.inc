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
   * @param DOMElement $create_properties 
   */
  public function __construct(DOMElement &$create_properties) {
    $create = simplexml_import_dom($create_properties);
    $this->parentPath = isset($create->parent_path) ? (string) $create->parent_path : null;
    $this->schemaPath = isset($create->schema_path) ? (string) $create->schema_path : null;
    if(isset($create->element)) {
      $this->element = (string) $create->element;
      $this->type = XMLCreateType::Element;
    }
    else if (isset($create->attribute)) {
      $this->attribute = (string) $create->attribute;
      $this->type = XMLCreateType::Attribute;
    }
    else {
      $this->xml = (string) $create->xml;
      $this->type = XMLCreateType::xml;
    }
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
   * @param DOMElement $update_properties 
   */
  public function __construct(DOMElement &$update_properties) {
    $update = simplexml_import_dom($update_properties);
    $this->path = (string) $update->path;
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

  public function __construct(DOMElement &$read_properties) {
    
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
   * @param DOMElement $delete_properties 
   */
  public function __construct(DOMElement &$delete_properties) {
    
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
   */
  public function __construct(DOMElement &$xml_properties) {
    $create = $xml_properties->getElementsByTagName('create');
    if ($create->length == 1) {
      $this->create = new XMLCreateProperties($create->item(0));
    }
    $read = $xml_properties->getElementsByTagName('read');
    if ($read->length == 1) {
      $this->read = new XMLCreateProperties($read->item(0));
    }
    $update = $xml_properties->getElementsByTagName('update');
    if ($update->length == 1) {
      $this->create = new XMLCreateProperties($update->item(0));
    }
    $delete = $xml_properties->getElementsByTagName('delete');
    if ($delete->length == 1) {
      $this->create = new XMLCreateProperties($delete->item(0));
    }
  }

}