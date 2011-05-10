<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'DocumentProperties');

class FormProperties {

  public $document;

  public static function createFromXMLDefiniton($xml_definition) {
    return self::createFromSimpleXML(simplexml_load_string($xml_definition));
  }

  public static function createFromSimpleXML(SimpleXMLElement $properties) {
    $document_properties = DocumentProperties::createFromSimpleXML($properties->document_properties);
    return new self($document_properties);
  }

  public function __construct(DocumentProperties $document_properties) {
    $this->document = $document_properties;
  }

}