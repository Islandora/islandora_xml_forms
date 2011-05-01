<?php

// $Id$

/**
 * @file
 *
 */

/**
 * This class utilizes scope and a reference pointer to track where 
 * the current value for a given element is. 
 */
class FormValues {

  /**
   * Submitted values from the form.
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
   * TRUE if we are tracking a location in the values array.
   * 
   * @var boolean 
   */
  protected $track;

  /**
   * Creates a FormValues object.
   * 
   * @param array $form_state 
   */
  public function __construct(array &$form_state) {
    $this->values = &$form_state['values'];
    $this->current = &$this->values;
    $this->track = false; // Default value is false.
  }

  /**
   *
   * @param FormElement $element
   * @return mixed 
   */
  public function getValue(FormElement $element) {
    $value = $this->track($element);
    return is_array($value) ? null : $value; // Array's are roots of #tree branchs
  }

  /**
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
   *
   * @param FormElement $element
   * @return boolean 
   */
  private function shouldStartTracking(FormElement $element) {
    return $element->tree === TRUE && !$this->track;
  }

  /**
   *
   * @param FormElement $element
   * @return boolean 
   */
  private function shouldStopTracking(FormElement $element) {
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
 * Types of FormProcessActions.
 */
class FormProcessActionType {
  const __default = 0;
  const Undefined = 0;
  const Create = 1;
  const Modify = 2;
  const Delete = 3;
}

/**
 * 
 */
class FormProcessAction extends ArrayObject {

  /**
   * The form value associated with this action. Actions do not require form values.
   * 
   * @var mixed 
   */
  protected $value;
  /**
   *
   * @var XMLProperties 
   */
  protected $properties;

  /**
   *
   * @param FormValues $form_values
   * @param FormElement $element 
   */
  public function __construct(mixed $value, XMLProperties $properties, FormProcessAction $parent = NULL) {
    $this->value = $value;
    $this->properties = $properties;
    $this->parent = $parent;
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
  protected $actions;

  /**
   *
   * @param type $form_state
   * @param type $definition 
   */
  public function __construct(FormValues $values) {
    $this->values = $values;
  }

  /**
   *
   * @param FormElement $element 
   */
  public function process(FormElements $elements) {
    $this->actions = $this->createProcessActions($this->values, $elements->root);
    return $this->actions;
  }

  /**
   *
   * @param FormElement $element
   * @param FormValues $form_value
   * @return type 
   */
  public function createProcessActions(FormValues $form_value, FormElement $element, FormProcessAction $parent = NULL) {

    /**
     * Traverse elements match values to elements.
     * 
     * Find the nodes as we traverse the graph.
     * 
     * Have the document contain the process actions.
     * When we add a new process action it returns the parent selection...
     * 
     * 
     */
    $value = $form_value->getValue($element);
    $action = isset($element->properties) ?
        new FormProcessAction($value, $element->properties, $parent) :
        $parent;
    foreach ($element as $child) {
      if (isset($action)) {
        $action[] = $this->createProcessActions(clone $form_value, $child, $action);
      }
      else {
        $action = $this->createProcessActions(clone $form_value, $child, $action);
      }
    }
    return $action;
  }

}