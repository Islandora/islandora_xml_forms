<?php

// $Id$

/**
 * @file
 *
 */

/**
 * XML properties for a single form element.
 */
class XMLProperties {

  protected $path;
  protected $parent_path;
  protected $schema;
  protected $schema_path;
  protected  $create_type;
  protected $xml;
  protected $element;
  protected $attribute;
 

  public function __construct(DOMElement &$xml_properties) {
    
  }

}