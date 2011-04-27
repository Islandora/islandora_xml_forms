<?php

// $Id$

/**
 * @file
 * @author Nigel Banks
 *
 * Defines a FormDefinition class that allows XML form defintions to be processed by the FormGenerator, and other
 * classes.
 */

/**
 * Describes a form
 */
class FormDefinition {

  const FORM_ROOT = 'xml_form_root';
  
  /**
   * The XML form definiton.
   * @var string 
   */
  protected $xml;
  /**
   * 
   * @var array 
   */
  protected $elements;

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
    $this->createDefinitionFromXML();
  }

  /**
   * 
   */
  private function createDefinitionFromXML() {
    $doc = new DOMDocument();
    $doc->loadXML($this->xml);
    if (!self::validate($doc)) {
      throw new Exception('Could not create FormDefintion object, the XML provided was not valid.');
    }
    $xpath = new DOMXPath($doc);
    $properties = $xpath->query('//properties/child::*');
    $this->initializeProperties($properties);
    $elements = $xpath->query('//elements/child::*');
    $this->createElements($elements);
  }

  /**
   * @TODO Implement this function.
   * @param DOMNodeList $properties 
   */
  private function initializeProperties(DOMNodeList &$properties) {
    
  }

  /**
   *
   * @param DOMNodeList $elements 
   */
  private function createElements(DOMNodeList &$elements) {
    module_load_include('inc', 'xml_form_api', 'FormElement');
    $count = $elements->length;
    for ($i = 0; $i < $count; $i++) {
      $element = $elements->item($i);
      $this->elements[] = new FormElement($element);
    }
  }

  /**
   * Checks to see if a xml form definition is valid.
   * 
   * @param string or DOMDocument
   *   The xml form definition to validated.
   * @return boolean
   *   TRUE if $definition is a valid form definition, FALSE otherwise.
   */
  public static function validate($definition) {
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
  public function getProperties() {
    $this->schema = new Schema();
  }

  /**
   * Gets the form elements that define this form.
   * 
   * @return FormElement[]
   */
  public function getElements() {
    return $this->elements;
  }

  /**
   *
   * @return array 
   */
  public function toArray() {
    $output = array();
    foreach ($this->elements as $element) {
      list($name, $value) = $element->toArray();
      if (isset($name)) {
        $output[$name] = $value;
      }
      else {
        $output[] = $value;
      }
    }
    return array(self::FORM_ROOT => $output);
  }

}

