<?php

// $Id$

/**
 * @file
 *
 * Associates submitted form values with FormElements.
 */
module_load_include('inc', 'xml_form_api', 'FormValueTracker');

/**
 * This class stores all submitted values. It provides a mechanism for 
 * accessing submitted values via a FormElement object.
 */
class FormValues {

  /**
   * A map of submitted form values where the key is the FormElements hash.
   * 
   * @var array
   */
  protected $values;
  /**
   * A helper class that helps determine what values are associated with what elements.
   * 
   * @var FormValueTracker 
   */
  protected $tracker;

  /**
   * Create a FormValues object.
   * 
   * @param array $form_state
   *   Drupal Form state.
   * @param FormElements $elements 
   *   The elements to associate with submitted values.
   */
  public function __construct(array &$form_state, FormElements $elements) {
    $this->values = array();
    $values = $this->getValuesFromFormState($form_state);
    if (isset($values)) {
      $this->tracker = new FormValueTracker($values);
      $this->setValues($elements->root, $this->tracker);
    }
  }

  /**
   * Gets the post data that stores the submited values.
   * 
   * @param array $form_state 
   * @return array
   */
  private function getValuesFromFormState(array &$form_state) {
    if(isset($form_state['values'])) {
      return $form_state['values'];
    }
    else if(isset($form_state['post']) && count($form_state['post']) > 0) {
      return $form_state['post'];
    }
    return NULL;
  }

  /**
   * Store values.
   * 
   * @param FormElement $element
   *   Element to store value for,
   * @param FormValueTracker $tracker 
   */
  private function setValues(FormElement $element, FormValueTracker $tracker) {
    $this->setValue($element, $tracker);
    foreach ($element as $child) {
      $this->setValues($child, clone $tracker);
    }
  }

  /**
   * Associate the given FormElement, with a submitted value.
   * 
   * @param FormElement $element
   * @param FormValueTracker $tracker 
   */
  private function setValue(FormElement $element, FormValueTracker $tracker) {
    $value = $tracker->getValue($element);
    $this->values[$element->hash] = $value;
  }

  /**
   * Get the value associated with the given FormElement.
   * 
   * @param FormElement $element
   * @return mixed
   *   The submitted value for the given FormElement if found otherwise NULL.
   */
  public function getValue(FormElement $element) {
    if (isset($this->values[$element->hash])) {
      return $this->values[$element->hash];
    }
    return NULL;
  }

  /**
   *
   * @return array 
   */
  public function getValues() {
    return $this->values;
  }

}