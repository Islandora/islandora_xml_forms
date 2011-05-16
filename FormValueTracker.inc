<?php

// $Id$

/**
 * @file
 *
 */

/**
 * This class utilizes scope and a reference pointer to track where 
 * the current value for a given FormElement. 
 */
class FormValueTracker {

  /**
   * Submitted values from the form. A reference to $form_state['values'].
   * 
   * @var array 
   */
  public $values;
  /**
   * A reference to a position in $values.
   * 
   * @var mixed
   */
  protected $current;
  /**
   * TRUE if we are tracking a location in the values array, FALSE if not.
   * 
   * @var boolean 
   */
  protected $track;

  /**
   * Creates a FormValues object.
   * 
   * @param array $values 
   */
  public function __construct(array &$values) {
    $this->values = &$values;
    $this->current = &$this->values;
    $this->track = false; // Default value is false.
  }

  /**
   * Gets the value for a given FormElement.
   * 
   * Tracks the current position in the $values array if applicable.
   * 
   * @param FormElement $element
   * @return mixed 
   *   Submitted value for the given FormElement if found, NULL otherwise.
   */
  public function getValue(FormElement $element) {
    $value = $this->track($element);
    return is_array($value) ? NULL : $value; // Array's are roots of #tree branchs
  }

  /**
   * Update the $current pointer so that it is pointing at the correct value.
   * 
   * @param FormElement $element 
   */
  private function track(FormElement $element) {
    if ($this->shouldStartTracking($element)) {
      $this->track = true;
    }
    if ($this->shouldStopTracking($element)) {
      $this->track = false;
      $this->reset();
    }
    if ($this->track && isset($this->current[$element->key])) {
      $this->current = &$this->current[$element->key];
    }
    return $this->current;
  }

  /**
   * Checks if the tracker should begin tracking.
   * 
   * @param FormElement $element
   * @return boolean 
   *   TRUE if the tracker should start to track values, FALSE if it is already tracking or shouldn't start tracking.
   */
  private function shouldStartTracking(FormElement $element) {
    return $element->tree === TRUE && !$this->track;
  }

  /**
   * Checks if the tracker should stop tracking.
   * 
   * @param FormElement $element
   * @return boolean 
   *   TRUE if the tracker should stop tracking values, FALSE if it is not tracking or should continue tracking.
   */
  private function shouldStopTracking(FormElement $element) {
    return $element->tree === FALSE && $this->track;
  }

  /**
   * Reset the $current pointer to the top level of the $values array.
   */
  private function reset() {
    $this->current = &$this->values;
  }

}