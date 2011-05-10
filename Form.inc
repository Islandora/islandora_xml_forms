<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormStorage');
module_load_include('inc', 'xml_form_api', 'FormDefinition');
module_load_include('inc', 'xml_form_api', 'FormElements');
module_load_include('inc', 'xml_form_api', 'Document');
module_load_include('inc', 'xml_form_api', 'NodeRegistry');

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
   * This object should not be modified after initialization. Modify $elements to make changes to the form.
   * 
   * @var FormDefinition 
   */
  protected $definition;
  /**
   * 
   * @var Document
   */
  protected $document;
  /**
   *
   * @var Registry
   */
  public $registry;
  /**
   * 
   * @var FormElements
   */
  protected $elements;
  protected $form_state;

  /**
   * Construct the form class.
   * 
   * @param array $form_state
   */
  public function __construct(array &$form_state) {
    $this->initialized = false;
    $this->form_state = &$form_state;
    $this->storage = new FormStorage($form_state);
    if ($this->storage->isInitialized()) {
      $this->initializeFromStorage();
    }
  }

  /**
   * Initialize this object from values in storage.
   */
  private function initializeFromStorage() {
    $this->definition = $this->storage->definition;
    $this->document = $this->storage->document;
    $this->registry = $this->storage->registry;
    $this->elements = $this->storage->elements;
    $this->initialized = true;
  }

  /**
   * Stores information required to rebuild this object.
   */
  private function initializeStorage() {
    $this->storage->definition = $this->definition;
    $this->storage->document = $this->document;
    $this->storage->registry = $this->registry;
    $this->storage->elements = $this->elements;
  }

  /**
   * Initializes this object members.
   * 
   * This function should be called the first time this object is created for a particular form,
   * on subsequent submit/validation/etc callbacks, Form::initializeFromStorage() will be
   * called.
   * 
   * @param FormDefinition $form_definition 
   * @param Document $xml_document
   */
  public function initialize(FormDefinition $form_definition, Document $document) {
    if (!$this->initialized) {
      $this->definition = $form_definition;
      $this->document = $document;
      $this->registry = Registry::getInstance();
      $this->registry->nodes = new NodeRegistry($document);
      $this->elements = clone $form_definition->elements;
      $this->generate(); // Register nodes and generate new elements.
      $this->initializeStorage();
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
  private function generate() {
    module_load_include('inc', 'xml_form_api', 'FormGenerator');
    $generator = new FormGenerator($this, $this->registry->nodes);
    $generator->generate($this->elements);
  }

  /**
   * Note this clones the existing set of elements, these elements will not be saved in the storage.
   * 
   * @param FormElements $elements
   * @return type 
   */
  public function populate(array &$form_state) {
    module_load_include('inc', 'xml_form_api', 'FormPopulator');
    $elements = $this->elements;
    $form_values = new FormValues($form_state, $elements);
    $populator = new FormPopulator($this->registry->nodes, $form_values);
    $populator->populate($elements);
  }

  /**
   *
   * @param array $form_state 
   */
  public function validate(array &$form_state) {
    module_load_include('inc', 'xml_form_api', 'FormValues');
    module_load_include('inc', 'xml_form_api', 'FormValidator');
    $elements = $this->elements;
    $form_values = new FormValues($form_state, $elements);
    $validator = new FormValidator($form_values);
    return $validator->validate($elements);
  }

  /**
   *
   * @param array $form_state
   * @return type 
   */
  public function submit(array &$form_state) {
    module_load_include('inc', 'xml_form_api', 'FormValues');
    module_load_include('inc', 'xml_form_api', 'FormProcessor');
    $elements = $this->elements;
    $form_values = new FormValues($form_state, $elements);
    $processor = new FormProcessor($form_values, $this->registry);
    return $processor->process($elements);
  }

  /**
   *
   * @return type 
   */
  public function toDrupalForm() {
    $this->populate($this->form_state);
    return $this->elements->toDrupalForm();
  }

  /**
   * Duplicate an existing FormElement from it's original definition.
   * 
   * @param FormElement $element
   *   Either a FormElement or a FormElement's hash.
   * @return FormElement
   *   The newly created FormElement.
   */
  public function duplicate($element) {
    $hash = FormElement::isFormElement($element) ? $element->hash : $element;
    $element = $this->registry->findElement($hash);
    $original = $this->registry->findOriginal($hash);
    if ($element) {
      $duplicate = clone $original;
      if ($element->parent) {
        $element->parent[] = $duplicate;
      }
      return $duplicate;
    }
    return NULL;
  }

  /**
   * Remove an existing FormElement.
   * 
   * @param FormElement $element
   *   Either a FormElement or a FormElement's hash.
   */
  public function remove($element) {
    $hash = FormElement::isFormElement($element) ? $element->hash : $element;
    $this->elements->remove($hash);
  }

}