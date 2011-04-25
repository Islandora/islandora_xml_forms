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

  /**
   * The XML form definiton.
   * @var string 
   */
  protected $xml;
  /**
   * The XML form definition.
   * @var DOMDocument 
   */
  protected $doc;

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
    $this->doc = new DOMDocument();
    $this->doc->loadXML($this->xml);
    $this->validate();
  }

  /**
   * Validates the document.
   */
  public function validate() {
    $path = drupal_get_path('module', 'xml_form_api') . '/FormDefinition.xsd';
    if ($this->doc->schemaValidate($path) == FALSE) {
      throw new Exception('Could not create FormDefintion object, the XML provided was not valid.');
    }
  }
  
  /**
   * 
   */
  public function getProperties() {
    
  }
  
  /**
   * 
   */
  public function getElements() {
    
  }
}

