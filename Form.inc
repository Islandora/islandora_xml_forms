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
   * @var NodeRegistry
   */
  protected $registry;
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
    $this->definition = $this->storage->definition;
    $this->document = $this->storage->document;
    $this->original = $this->storage->original;
    $this->registry = $this->storage->registry;
    $this->elements = $this->storage->elements;
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
  public function initialize(FormDefinition $form_definition, Document $document) {
    if (!$this->initialized) {
      $this->definition = $form_definition;
      $this->document = $document;
      $this->registry = new NodeRegistry($document);
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
    $iterator = new MultipleIterator();
    $iterator->attachIterator($this->elements->getIterator());
    $iterator->attachIterator($elements->getIterator());
    foreach ($iterator as $result) {
      list($element, $definition) = $result;
      $this->original[$element->hash] = $definition;
      var_dump($element->hash);
    }
    $this->generate();
  }

  private function store() {
    $this->storage->definition = $this->definition;
    $this->storage->document = $this->document;
    $this->storage->original = $this->original;
    $this->storage->registry = $this->registry;
    $this->storage->elements = $this->elements;
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
    $generator = new FormGenerator($this, $this->registry);
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
    $populator = new FormPopulator($this->registry, $elements, $form_values);
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
    $processor = new FormProcessor($form_values, $this->registry);
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
   * Duplicate an existing FormElement from it's original definition.
   * 
   * @param FormElement $element
   *   Either a FormElement or a FormElement's hash.
   * @return FormElement
   *   The newly created FormElement.
   */
  public function duplicate($element) {
    $hash = FormElement::isFormElement($element) ? $element->hash : $element;
    if (isset($this->original[$hash])) {
      $original = $this->original[$hash];
      // Copy
      $new = clone $original;
      $this->original[$new->hash] = $original;
      // Store reference to original
      $iterator1 = new RecursiveIteratorIterator(new RecursiveArrayIterator($new), RecursiveIteratorIterator::SELF_FIRST);
      $iterator2 = new RecursiveIteratorIterator(new RecursiveArrayIterator($original), RecursiveIteratorIterator::SELF_FIRST);
      $iterator = new MultipleIterator();
      $iterator->attachIterator($iterator1);
      $iterator->attachIterator($iterator2);
      foreach($iterator as $item) {
        list($duplicate, $orig) = $item;
        $this->original[$duplicate->hash] = $orig;
      }
      // Find element
      $element = $this->elements->findElement($hash);
      // Add to parent
      $element->parent[] = $new;
      $this->elements->registerElement($new); // blah...
      return $new;
    }
    echo ('//---</br>');
    var_dump($element->hash);
    echo ('//---</br>');
    var_dump(array_keys($this->original));
    return null;
  }

  /**
   * Remove an existing FormElement.
   * 
   * @param FormElement $element
   *   Either a FormElement or a FormElement's hash.
   */
  public function remove($element) {
    $hash = FormElement::isFormElement($element) ? $element->hash : $element;
    $this->elements->removeElement($hash);
  }

}