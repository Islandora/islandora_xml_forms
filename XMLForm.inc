<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'objective_forms', 'Form');

/**
 * 
 */
class XMLForm extends Form {

  /**
   * The document the form manipulates.
   * 
   * @var XMLDocument
   */
  protected $document;
  /**
   * Stores node references.
   * 
   * @var NodeRegistry
   */
  protected $nodeRegistry;

  /**
   *
   * @param array $form_state
   */
  public function __construct(array &$form_state) {
    parent::__construct(array(), $form_state);
    if ($this->storage->initialized) {
      $this->initializeFromStorage();
    }
  }

  /**
   * Initialize this object from values in storage.
   */
  private function initializeFromStorage() {
    //$this->definition = $this->storage->definition;
    $this->document = $this->storage->document;
    $this->root = $this->storage->root;
    //$this->registry = $this->storage->registry;
    //$this->elements = $this->storage->elements;
  }

  /**
   * Stores information required to rebuild this object.
   */
  private function storePersistantMembers() {
    //$this->storage->definition = $this->definition;
    $this->storage->document = $this->document;
    //$this->storage->registry = $this->registry;
    //$this->storage->elements = $this->elements;
    $this->storage->initialized = TRUE;
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
  public function initialize(XMLFormDefinition $form_definition, Document $document) {
    if (!$this->initialized) {
      //$this->definition = $form_definition;
      $this->document = $document;
      //$this->registry = Registry::getInstance();
      //$this->registry->nodes = new NodeRegistry($document);
      //$this->elements = clone $form_definition->elements;
      //$this->generate(); // Register nodes and generate new elements.
      $this->storePersistantMembers();
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
    return $this->storage->initialized;
  }

  /**
   * Called the first time this form is created, this generates additional elements based on the XMLDocument's nodes.
   * 
   * @return array 
   */
  private function generate() {
    module_load_include('inc', 'xml_form_api', 'FormGenerator');
    $generator = new XMLFormGenerator($this, $this->registry->nodes);
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

}