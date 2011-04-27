<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormControl');
module_load_include('inc', 'xml_form_api', 'XMLProperties');

class FormElement {
  /**
   * The parent element of this element if it exists.
   * @var FormElement 
   */
  protected $parent;
  /**
   * The form controls that define this element.
   * 
   * @var FormControl[]
   */
  protected $controls;
  /**
   * This elements children if they exist.
   * 
   * @var FormElement[]
   */
  protected $children;
  /**
   *
   * @var XMLProperties 
   */
  protected $xmlProperties;
  /**
   * 
   * @var string 
   */
  protected $name;

  /**
   *
   * @param SimpleXMLElement $element
   * @param FormElement $parent 
   */
  public function __construct(SimpleXMLElement &$element, FormElement &$parent = NULL) {
    $this->parent = $parent;
    $this->initialize($element);
  }

  /**
   *
   * @param SimpleXMLElement $element 
   */
  private function initialize(SimpleXMLElement &$element) {
    $this->name = isset ($element['name']) ? $element['name'] : null;
    $this->createFormControls($element);
    $this->createXMLProperties($element);
    $this->createChildren($element);
  }

  /**
   *
   * @param SimpleXMLElement $element 
   */
  private function createFormControls(SimpleXMLElement &$element) {
    $form_controls = $element->xpath("child::*[local-name() != 'children' and local-name() != 'xml_properties']");
    foreach($form_controls as $form_control) {
      $name = $form_control->getName();
      $this->controls[$name] = new FormControl($form_control);
    }
  }

  /**
   *
   * @param SimpleXMLElement $element 
   */
  private function createXMLProperties(SimpleXMLElement &$element) {
    if(isset($element->xml_properties)) {
      $this->xmlProperties = new XMLProperties($element->xml_properties);
    }
  }

  /**
   *
   * @param SimpleXMLElement $element 
   */
  private function createChildren(SimpleXMLElement &$element) {
    $children = &$element->children;
    foreach($children->children() as $child) {
      $this->children[] = new FormElement($child, $this);
    }
  }

  /**
   * Gets this elements parent, if it exists.
   * 
   * @return FormElement 
   */
  public function getParent() {
    return isset($this->parent) ? $this->parent : NULL;
  }

  /**
   * Gets this elements children, if they exist.
   * 
   * @return FormElements[] 
   */
  public function getChildren() {
    return isset($this->children) ? $this->children : NULL;
  }

  /**
   * Returns this elements form controls.
   * 
   * @return FormControl[] 
   */
  public function getFormControls() {
    return $this->controls;
  }

  /**
   * 
   * @return XMLProperties 
   */
  public function getXMLProperties() {
    return (isset($this->xmlProperties)) ? $this->xmlProperties : NULL;
  }

  /**
   * 
   * @return array
   *   
   */
  public function toArray() {
    $output = $this->getFormControlsAsArray();
    $output = array_merge($output, $this->getChildrenAsArray());

    return array($this->name, $output);
  }

  /**
   *
   * @return array 
   */
  private function getFormControlsAsArray() {
    $output = array();
    foreach ($this->controls as $form_control) {
      $output = array_merge($output, $form_control->toArray());
    }
    return $output;
  }

  /**
   *
   * @return array 
   */
  private function getChildrenAsArray() {
    $output = array();
    if (isset($this->children)) {
      foreach ($this->children as $child) {
        list($name, $value) = $child->toArray();
        if ($name) {
          $output[$name] = $value;
        }
        else {
          $output[] = $value;
        }
      }
    }
    return $output;
  }

}