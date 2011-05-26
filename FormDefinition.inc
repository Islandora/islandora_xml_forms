<?php

// $Id$

/**
 * @file
 * @author Nigel Banks
 *
 * Defines a FormDefinition class that allows XML form defintions to be processed by the FormGenerator, and other
 * classes.
 */
module_load_include('inc', 'xml_form_api', 'FormProperties');
module_load_include('inc', 'xml_form_api', 'FormElements');

/**
 * Describes a form
 */
class FormDefinition {

  /**
   * @var FormProperties
   */
  public $properties;
  /**
   * A Depth first iterator containing all the elements in the form definition, plus a default root element.
   * 
   * @var FormElements
   */
  public $elements;

  /**
   * Create a FormDefinition from an XML String.
   * 
   * @param string $xml_definition
   * @return FormDefinition
   */
  public static function createFromXMLDefiniton($xml_definition) {
    return self::createFromSimpleXML(simplexml_load_string($xml_definition));
  }

  /**
   *
   * @param type $root_name
   * @param SimpleXMLElement $elements 
   */
  public static function createFromSimpleXML(SimpleXMLElement $form) {
    if (self::isValid($form)) {
      $properties = FormProperties::createFromSimpleXML($form->properties);
      $elements = FormElements::createFromSimpleXML(Form::ROOT, $form->elements);
      return new self($properties, $elements);
    }
    else {
      throw new Exception('Failed to create FormDefinition from SimpleXML, the definition was invalid.');
    }
  }

  /**
   * Checks to see if a xml form definition is valid.
   * 
   * @param mixed $definition
   *   The xml form definition to validated. Can be a string a DOMDocument or a SimpleXMLElement
   * @return boolean
   *   TRUE if $definition is a valid form definition, FALSE otherwise.
   */
  public static function isValid($definition) {
    $path = drupal_get_path('module', 'xml_form_api') . '/FormDefinition.xsd';
    if (is_string($definition)) {
      $document = new DOMDocument();
      $document->loadXML($definition);
      return $document->schemaValidate($path);
    }
    else if (get_class($definition) == 'DOMDocument') {
      return $definition->schemaValidate($path);
    }
    else if (get_class($definition) == 'SimpleXMLElement') {
      $form = dom_import_simplexml($definition);
      $document = new DOMDocument('1.0');
      $form = $document->importNode($form, true);
      $form = $document->appendChild($form);
      return $document->schemaValidate($path);
    }
    return FALSE;
  }

  /**
   * Creates a FormDefintion.
   */
  public function __construct(FormProperties $properties, FormElements $elements) {
    $this->properties = $properties;
    $this->elements = $elements;
  }

  /**
   *
   * @return type 
   */
  public function asXML() {
    $document = new DOMDocument();
    $form = $this->asNode($document);
    $document->appendChild($form);
    return $document->saveXML();
  }

  /**
   * 
   * @return string
   */
  public function asNode(DOMDocument $document) {
    $form = $document->createElement('form');
    $properties = $this->properties->asNode($document);
    $form->appendChild($properties);
    $elements = $this->elements->asNode($document);
    $form->appendChild($elements);
    return $form;
  }

}

