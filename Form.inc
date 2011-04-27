<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormStorage');

class Form {
  /**
   * Root of the form;
   */
  const ROOT = 'xml_form_root';

  /**
   * Is this object ready to use? This object can be initialized from storage or, 
   * from the intialize function.
   * 
   * @var boolean 
   */
  protected $initialized;
  /**
   * Persistent information used for generating the form, 
   * responding to ahah requests, and ultimately processing the form.
   * 
   * @var FormStorage 
   */
  protected $storage;
  /**
   * The definition of the form from which the Drupal Form API repersentation is generated.
   * @var FormDefinition 
   */
  protected $definition;
  /**
   *
   * @var XMLDocument 
   */
  protected $document;

  /**
   * Construct the form class.
   * 
   * @param array $form_state
   * @param FormDefinition $form_definition 
   */
  public function __construct(FormStorage &$storage) {
    $this->initialized = false;
    $this->storage = &$storage;
    if ($this->storage->isInitialized()) {
      $this->initializeFromStorage();
    }
  }

  /**
   * Initialize this object from values in storage.
   */
  private function initializeFromStorage() {
    $this->definition = &$this->storage->getDefinition();
    $this->initialized = true;
  }

  /**
   * Initializes this object members.
   * 
   * This function should be called the first time this object is created for a particular form,
   * on subsequent submit/validation/etc callbacks, Form::initializeFromStorage() will be
   * called.
   * 
   * @param FormDefinition $form_definition 
   * @param XMLDocument $xml_document
   */
  public function initialize(&$form_definition, &$xml_document) {
    if (!$this->initialized) {
      $this->definition = &$form_definition;
      $this->storage->setDefinition($form_definition);
      $this->document = &$xml_document;
      $this->storage->setDocument($xml_document);
      $this->initialized = true;
    }
    else {
      throw new Exception('Attempted to intialized the Form after it has already been intialized.');
    }
  }

  /**
   * Has the form been initialized?
   * 
   * @return boolean 
   */
  public function isInitialized() {
    return $this->initialized;
  }

  /**
   *
   * @return array 
   */
  public function toArray() {
    return $this->definition->toArray();
  }

  /**
   *
   * @param array $form
   * @param array $form_state 
   */
  public function validate(&$form, &$form_state) {
    module_load_include('inc', 'xml_form_api', 'FormValidator');
    $validator = new FormValidator($form_state, $this->storage);
  }

  /**
   *
   * @param array $form
   * @param array $form_state 
   * @return XMLDocument
   */
  public function submit(&$form, &$form_state) {
    module_load_include('inc', 'xml_form_api', 'FormProcessor');
    $processor = new FormProcessor($form_state, $this->storage);
    return $processor->process();
  }

}