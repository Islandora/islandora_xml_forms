<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'XMLProperties');
module_load_include('inc', 'xml_form_api', 'FormControl');

class FormElement {

  /**
   * The form definition this element belongs to.
   * 
   * @var FormDefinition 
   */
  //protected $form;
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
   * @param DOMElement $element
   * @param FormElement $parent 
   */
  public function __construct(DOMElement $element, FormElement $parent = NULL) {
    $this->parent = $parent;
    $this->initialize($element);
  }

  /**
   *
   * @param DOMElement $element 
   */
  private function initialize(DOMElement $element) {
    $xpath = new DOMXPath($element->ownerDocument);
    $name = $xpath->query("@name", $element);
    $this->setName($name);
    $form_controls = $xpath->query("child::*[local-name() != 'children' and local-name() != 'xml_form']", $element);
    $this->setFormControls($form_controls);
    $xml_properties = $xpath->query("child::xml_properties");
    $this->setXMLProperties($xml_properties);
    $children = $xpath->query("child::children/element", $element);
    $this->createChildren($children);
  }

  /**
   *
   * @param DOMNodeList $name 
   */
  private function setName(DOMNodeList $name) {
    if ($name && $name->length == 1) {
      $this->name = $name->item(0)->value;
    }
  }

  /**
   *
   * @param DOMNodeList $form_controls 
   */
  private function setFormControls(DOMNodeList $form_controls) {
    $count = $form_controls->length;
    for ($i = 0; $i < $count; $i++) {
      $form_control = $form_controls->item($i);
      $form_control = new FormControl($form_control);
      $this->controls[$form_control->getName()] = &$form_control;
    }
  }

  /**
   * @param DOMNodeList $xml_form_properties
   */
  private function setXMLProperties(DOMNodeList &$xml_form) {
    if ($xml_form && $xml_form->length == 1) {
      $this->xmlProperties = new XMLProperties($xml_form->item(0));
    }
  }

  /**
   *
   * @param DOMNodeList $children 
   */
  private function createChildren(DOMNodeList $children) {
    $count = $children->length;
    for ($i = 0; $i < $count; $i++) {
      $element = $children->item($i);
      $this->children[] = new FormElement($element, $this);
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