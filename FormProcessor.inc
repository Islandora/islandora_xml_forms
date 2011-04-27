<?php

// $Id$

/**
 * @file
 *
 */


/**
 * This class might be confusing but it simplifies the process. 
 */
class FormValues {

  /**
   *
   * @var array 
   */
  protected $values;
  /**
   * A reference to the current element in the $values array.
   * 
   * @var mixed 
   */
  protected $ptr;
  /**
   * TRUE if we are tracking a location in the values array.
   * 
   * @var boolean 
   */
  protected $track;

  /**
   *
   * @param array $form_state 
   */
  public function __construct(array &$form_state) {
    if (!isset($form_state['values'])) {
      throw new Exception('Failed to create FormValues, the given form_state was invalid.');
    }
    $this->values = $form_state['values'];
    $this->current = $this->values;
    $this->track = false; // Default value is false.
  }

  /**
   *
   * @param FormElement $element
   * @return type 
   */
  public function getValue(FormElement& $element) {
    $this->track($element);
    return is_array($this->current) ? NULL : $this->current; // Array's are roots of #tree branchs
  }

  /**
   *
   * @param FormElement $element 
   */
  private function track(FormElement &$element) {
    if ($this->shouldStartTracking($element)) {
      $this->track = true;
    }
    else if ($this->shouldStopTracking($element)) {
      $this->track = false;
      $this->reset();
    }
    if(empty($element->key)) {
      throw new Exception('Fatal error, Attempted to get the value of a FormElement that doesn\'t have a key.');
    }
    if ($this->track && isset($this->current[$element->key])) {
      $this->current = &$this->current[$element->key];
    }
  }

  /**
   *
   * @param FormElement $element
   * @return boolean 
   */
  private function shouldStartTracking(FormElement &$element) {
    return $element->tree === TRUE && !$this->track;
  }

  /**
   *
   * @param FormElement $element
   * @return boolean 
   */
  private function shouldStopTracking(FormElement &$element) {
    return $element->tree === FALSE && $this->track;
  }

  /**
   * 
   */
  private function reset() {
    $this->current = &$this->values;
  }

}

/**
 * 
 */
class FormProcessor {

  /**
   * Submitted form values.
   * 
   * @var FormValues 
   */
  protected $values;
  

  /**
   *
   * @param type $form_state
   * @param type $definition 
   */
  public function __construct(FormValues &$values, XMLDocument $document) {
    $this->values = $values;
  }
  
  /**
   *
   * @param XMLDocument $document 
   */
  public function createDocument(XMLDocument $document) {
    
  }
  
  /**
   *
   * @param FormElement $element 
   */
  public function process(FormElement $element) {
    $element->setValue($this->values);
    $this->processElement($element);
  }

  /**
   *
   * @param FormElement[] $elements 
   */
  public function setElementValue($elements) {
    foreach ($elements as $key => &$element) {
      $this->setElementValue($key, $element, $key, $this->values);
    }
    return $elements;
  }

  public function setElementValue($key, FormElement &$element, $values) {
    $element->value = $values->getValue($key, $element);
    foreach ($element->children as $key => $child) {
      
    }
  }

}

