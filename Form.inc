<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormStorage');
module_load_include('inc', 'xml_form_api', 'FormDefinition');
module_load_include('inc', 'xml_form_api', 'FormElement');
module_load_include('inc', 'xml_form_api', 'FormControl');
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
   * @var FormDefinition 
   */
  protected $definition;
  /**
   *
   * @var Document 
   */
  protected $nodeRegistry;

  /**
   * Construct the form class.
   * 
   * @param array $form_state
   */
  public function __construct(array &$form_state) {
    $this->initialized = false;
    $this->storage = new FormStorage($form_state);
    if ($this->storage->isInitialized()) {
      $this->initializeFromStorage();
    }
  }

  /**
   * Initialize this object from values in storage.
   */
  private function initializeFromStorage() {
    $this->definition = $this->storage->getDefinition();
    $this->nodeRegistry = $this->storage->getNodeRegistry();
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
   * @param Document $xml_document
   */
  public function initialize(FormDefinition $form_definition, NodeRegistry $node_registry) {
    if (!$this->initialized) {
      $this->definition = $form_definition;
      $this->storage->setDefinition($form_definition);
      $this->nodeRegistry = $node_registry;
      $this->storage->setNodeRegistry($node_registry);
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
  public function generate() {
    module_load_include('inc', 'xml_form_api', 'FormGenerator');
    $generator = new FormGenerator($this->nodeRegistry);
    return $generator->generate($this->definition->elements);
  }
  
  /**
   * Note this clones the existing set of elements, these elements will not be saved in the storage.
   * 
   * @param FormElements $elements
   * @return type 
   */
  public function populate() {
    module_load_include('inc', 'xml_form_api', 'FormPopulator');
    $populator = new FormPopulator($this->nodeRegistry);
    return $populator->populate(clone $this->definition->elements);
  }

  /**
   *
   * @param array $form_state 
   */
  public function validate(array &$form_state) {
    module_load_include('inc', 'xml_form_api', 'FormValues');
    module_load_include('inc', 'xml_form_api', 'FormValidator');
    $elements = $this->definition->elements;
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
    $elements = $this->definition->elements;
    $form_values = new FormValues($form_state, $elements);
    $processor = new FormProcessor($form_values, $this->nodeRegistry);
    return $processor->process($elements);
  }

  /**
   *
   * @param type $hash
   * @return type 
   */
  public function findElement($hash) {
    return $this->definition->elements->getRegisteredElement($hash);
  }

  /**
   *
   * @param type $hash
   * @return type 
   */
  public function duplicateElement($hash) {
    return $this->definition->elements->duplicateElement($hash);
  }

  /**
   *
   * @param type $hash
   * @return type 
   */
  public function removeElement($hash) {
    return $this->definition->elements->removeElement($hash);
  }

}