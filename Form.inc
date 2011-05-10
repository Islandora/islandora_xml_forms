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
   * This object should not be modified after initialization. Modify $elements to make changes to the form.
   * 
   * @var FormDefinition 
   */
  protected $definition;
  /**
   *
   * @var 
   */
  protected $nodeRegistry;
  /**
   * 
   * @var FormElements
   */
  protected $elements;
  /**
   * A map of current elements to thier original definition. For the purpose of duplicating the originals.
   * 
   * @var array
   */
  protected $original;

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
    $this->elements = $this->storage->getElements();
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
      $this->nodeRegistry = $node_registry;
      $this->initializeElements($form_definition->elements);
      $this->store();
      $this->initialized = true;
    }
    else {
      throw new Exception('Attempted to intialized the Form after it has already been intialized.');
    }
  }

  private function initializeElements(FormElements $elements) {
    $this->elements = clone $elements;
    $this->mapElementsToOriginalDefinitions($elements);
    $this->generate();
  }

  private function store() {
    $this->storage->setDefinition($this->definition);
    $this->storage->setNodeRegistry($this->nodeRegistry);
    $this->storage->setElements($this->elements);
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
   * @param FormElements $elements 
   */
  private function mapElementsToOriginalDefinitions(FormElements $elements) {
    $iterator = new MultipleIterator();
    $iterator->attachIterator($this->elements->getIterator());
    $iterator->attachIterator($elements->getIterator());
    foreach ($iterator as $result) {
      list($element, $definition) = $result;
      $this->original[$element->hash] = $definition;
    }
  }

  /**
   *
   * @return array 
   */
  private function generate() {
    module_load_include('inc', 'xml_form_api', 'FormGenerator');
    $generator = new FormGenerator($this->nodeRegistry);
    $generator->generate($this->elements);
  }

  /**
   * Note this clones the existing set of elements, these elements will not be saved in the storage.
   * 
   * @param FormElements $elements
   * @return type 
   */
  public function &populate(array &$form, array &$form_state) {
    module_load_include('inc', 'xml_form_api', 'FormPopulator');
    $elements = $this->elements;
    $form_values = new FormValues($form_state, $elements);
    $populator = new FormPopulator($this->nodeRegistry, $elements, $form_values);
    $populator->populate($form);
    return $form;
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
    $processor = new FormProcessor($form_values, $this->nodeRegistry);
    return $processor->process($elements);
  }

  /**
   *
   * @return type 
   */
  public function toDrupalForm() {
    return $this->elements->toDrupalForm();
  }

  /**
   *
   * @param type $hash
   * @return type 
   */
  public function findElement($hash) {
    return $this->elements->getRegisteredElement($hash);
  }

  public function findOriginalElementDefinition($hash) {
    if (isset($this->original[$hash])) {
      return $this->original[$hash];
    }
    return NULL;
  }

  /**
   *
   * @param type $hash
   * @return type 
   */
  public function duplicateElement($hash) {
    return $this->elements->duplicateElement($hash);
  }

  /**
   *
   * @param type $hash
   * @return type 
   */
  public function removeElement($hash) {
    return $this->elements->removeElement($hash);
  }

}