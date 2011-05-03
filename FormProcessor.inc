<?php

// $Id$

/**
 * @file
 *
 */

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
  /**
   *
   * @var Document 
   */
  protected $document;
  /**
   *
   * @var type 
   */
  protected $actions;

  /**
   *
   * @param type $form_state
   * @param type $definition 
   */
  public function __construct(FormValues $values, Document $document) {
    $this->values = $values;
    $this->document = $document;
  }

  /**
   *
   * @param FormElement $element 
   */
  public function process(FormElements $elements) {
    
    
    var_dump($this->document->asXML());
  }

}