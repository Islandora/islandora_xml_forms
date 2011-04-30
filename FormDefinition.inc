<?php

// $Id$

/**
 * @file
 * @author Nigel Banks
 *
 * Defines a FormDefinition class that allows XML form defintions to be processed by the FormGenerator, and other
 * classes.
 */
module_load_include('inc', 'xml_form_api', 'FormElement');
module_load_include('inc', 'xml_form_api', 'XMLDocument');

/**
 * Describes a form
 */
class FormDefinition {

  /**
   * The XML form definiton.
   * @var string 
   */
  protected $xml;
  /**
   * The Schema used for validating/generating the XML document.
   * 
   * @var Schema
   */
  protected $schema;
  /**
   * A Depth first iterator containing all the elements in the form definition, plus a default root element.
   * 
   * @var FormElements
   */
  public $elements;
  /**
   *
   * @var XMLDocumentProperties 
   */
  protected $documentProperties;

  /**
   * Creates a form defintion object from an XML form definition.
   *
   * @param string $form_definition
   *   XML form definition, it should be valid according to FormDefiniton.xsd, if it is not an exception will be thrown.
   */
  public function __construct($form_definition) {
    if (!is_string($form_definition) || trim($form_definition) == '') {
      throw new Exception('The argument $form_definition must be a non-empty string.');
    }
    $this->xml = $form_definition;
    $this->initialize();
  }

  /**
   * Checks to see if a xml form definition is valid.
   * 
   * @param string or DOMDocument
   *   The xml form definition to validated.
   * @return boolean
   *   TRUE if $definition is a valid form definition, FALSE otherwise.
   */
  public static function isValid($definition) {
    $path = drupal_get_path('module', 'xml_form_api') . '/FormDefinition.xsd';
    if (is_string($definition)) {
      $doc = new DOMDocument();
      $doc->loadXML($definition);
      return $doc->schemaValidate($path);
    }
    else if (get_class($definition) == 'DOMDocument') {
      return $definition->schemaValidate($path);
    }
    return FALSE;
  }

  /**
   * 
   */
  private function validate() {
    $definition = new DOMDocument();
    $definition->loadXML($this->xml);
    if (!self::isValid($definition)) {
      throw new Exception('Could not create FormDefintion object, the XML provided was not valid.');
    }
    return $definition;
  }

  /**
   * 
   */
  private function initialize() {
    $document = $this->validate();
    $form = simplexml_import_dom($document);
    $this->initializeProperties($form->properties);
    $this->createElements($form->elements);
  }

  /**
   * @TODO Implement this function.
   * @param DOMElement $properties 
   */
  private function initializeProperties(SimpleXMLElement $properties) {
    $this->documentProperties = new XMLDocumentProperties($properties->document_properties);
  }

  /**
   *
   * @param DOMNodeList $elements 
   */
  private function createElements(SimpleXMLElement $elements) {
    $this->elements = new FormElements(Form::ROOT);
    foreach ($elements->children() as $element) {
      $form_element = new FormElement($element);
      $this->elements->add($form_element);
    }
  }

  /**
   *
   * @return XMLDocumentProperties 
   */
  public function getDocumentProperties() {
    return $this->documentProperties;
  }

  /**
   *
   * @return array 
   */
  public function toArray() {
    return $this->elements->root->toArray();
  }

}

