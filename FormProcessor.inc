<?php

// $Id$

/**
 * @file
 *
 */

/**
 * 
 */
class FormProcessAction extends ArrayObject {
  /**
   * Type of action determines how this object will behave.
   */
  const Undefined = 0; // Do Nothing.
  const Create = 1;    // Create a DOMNode
  const Read = 2;      // Not used
  const Update = 3;    // Update a DOMNode
  const Delete = 4;    // Delete a DOMNode
  /**
   * 
   * @var FormElement 
   */

  protected $element;
  /**
   *
   * @var XMLProperties 
   */
  protected $properties;
  /**
   * The form value associated with this action. Actions do not require form values.
   * 
   * @var mixed 
   */
  protected $value;
  /**
   * 
   * @var mixed 
   */
  protected $_type;

  /**
   *
   * @param FormElement $element
   * @param mixed $value
   * @param DOMNode $node 
   */
  public function __construct(FormElement $element, $value = NULL, DOMNode $node = NULL) {
    $this->element = $element;
    $this->properties = $element->properties;
    $this->value = $value;
    $this->node = $node;
    $this->_type = $this->determineType();
  }

  /**
   *
   * @return integer
   */
  private function determineType() {
    $properties = $this->properties;
    if ($properties) {
      if ($this->node) { // Node already existed, either we are going to update or delete it.
        $delete = $properties->delete;
        $update = $properties->update;
        if ($delete && $delete->shouldDelete($this->value)) {
          return self::Delete;
        }
        else if ($update && $update->shouldUpdate($this->value)) {
          return self::Update;
        }
      }
      else {
        $create = $properties->create;
        if ($create && $create->shouldCreate($this->value)) {
          return self::Create;
        }
      }
    }
    return self::Undefined;
  }

  /**
   * 
   */
  public function execute(Document $document) {
    switch ($this->_type) {
      case self::Create:
        $this->create($document);
        break;
      case self::Update:
        $this->update($document);
        break;
      case self::Delete:
        $this->delete($document);
        break;
    }
  }

  private function create(Document $document) {
    $this->properties->create->create($document, $this->value);
  }

  private function update(Document $document) {
    $this->properties->update->update($document, $this->value);
  }

  private function delete(Document $document) {
    $this->properties->delete->delete($document);
  }

  public function __get($name) {
    return ($name == 'type') ? $this->_type : NULL;
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
  protected $registry;
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
  public function __construct(FormValues $values, NodeRegistry $registry) {
    $this->values = $values;
    $this->registry = $registry;
    $this->document = $registry->document;
  }

  /**
   *
   * @param FormElement $element 
   */
  public function process(FormElements $elements) {
    /*$this->createActions($elements);
    $this->sortActions();
    $this->performActions();
    var_dump($this->document->asXML());*/
  }

  /**
   *
   * @param FormElements $elements 
   */
  private function createActions(FormElements $elements) {
    foreach ($elements as $element) {
      $this->createAction($element);
    }
  }

  /**
   *
   * @param FormElement $element
   */
  private function createAction(FormElement $element) {
    $node = $this->document->getRegisteredNode($element);
    $value = $this->values->getValue($element);
    $this->actions[] = new FormProcessAction($element, $value, $node);
  }

  /**
   * 
   */
  private function sortActions() {
    $delete = array();
    $update = array();
    $create = array();
    foreach ($this->actions as $action) {
      switch ($action->type) {
        case FormProcessAction::Create:
          $create[] = $action;
          break;
        case FormProcessAction::Update:
          $update[] = $action;
          break;
        case FormProcessAction::Delete:
          $delete[] = $action;
          break;
      }
    }
    $this->actions = array_merge($update, $delete, $create);
  }

  /**
   * 
   */
  private function performActions() {
    foreach ($this->actions as $action) {
      $action->execute($this->document);
    }
  }

}