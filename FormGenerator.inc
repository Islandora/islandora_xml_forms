<?php

// $Id$

/**
 * @file
 *
 */

/**
 * 
 */
class FormGenerator {

  /**
   * The definition of the form to generate.
   * 
   * @var FormDefinition
   */
  protected $definition;

  /**
   * Creates a form generator.
   * 
   * @param FormDefinition $form_definition
   */
  public function __construct(&$form_definition) {
    if (get_class($form_definition) != 'FormDefinition') {
      throw new Exception('');
    }
    $this->definition = &$form_definition;
  }

  /**
   *
   * @param DOMNodeList $elements
   */
  private function generateElements(DOMNodeList &$elements) {
    $output = array();
    $count = $elements->length;
    for ($i = 0; $i < $count; $i++) {
      $element = $elements->item($i);
      $output[] = $this->generateElement($element);
    }
    return $output;
  }

  /**
   * Generates a drupal form API form element from its xml definition.
   * 
   * @param DOMElement $element 
   *   A form element as defined by FormDefinition.xsd
   * @return array
   *   Returns the drupal form API repersentation of that element.
   */
  private function generateElement(DOMElement &$element) {
    $xpath = new DOMXPath($element->ownerDocument);
    $form_controls = $xpath->query("child::*[local-name() != 'children']", $element);
    if ($form_controls && $form_controls->length > 0) {
      $output = $this->processFormControls($form_controls);
      $children = $xpath->query("child::children", $element);
      if ($children && $children->length == 1) {
        $child_elements = $xpath->query("child::element", $children->item(0));
        $output['children'] = $this->generateElements($child_elements);
      }
      return $output;
    }
    else {
      throw new Exception('Invalid element definition.');
    }
  }

  /**
   * Generates a drupal form API form element from a collection of form controls
   * defined in xml.
   * 
   * @param DOMNodeList $form_controls 
   * @return array
   *   Returns the drupal form API repersentation from a set of form controls.
   */
  private function processFormControls(&$form_controls) {
    module_load_include('inc', 'xml_form_api', 'FormControlGeneratorFunctions');
    $output = array();
    $count = $form_controls->length;
    for ($i = 0; $i < $count; $i++) {
      $form_control = $form_controls->item($i);
      $name = $form_control->tagName;
      $generator_function = FormControlGeneratorFunctions::getGeneratorFunctionFromName($name);
      $output['#' . $name] = $generator_function($form_control);
    }
  }

  /**
   * Creates a Drupal API form, from the form definition.
   *
   * @return array
   *   Returns a Drupal API form.
   */
  public function generate() {
    $elements = $this->definition->getElements();
    return $this->generateElements($elements);
  }

}

