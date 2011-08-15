<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'XMLDocument');

/**
 * 
 */
class XMLFormProcessAction {

  /**
   *
   * @var XMLDocument
   */
  public $document;

  /**
   * FormElement to be processed.
   * 
   * @var FormElement 
   */
  public $element;

  /**
   * Submitted Form Value.
   * 
   * @var mixed 
   */
  public $value;

  /**
   * Action to perform.
   * 
   * @var Action
   */
  public $action;

  /**
   * Type of action defined by Action::CONST.
   * 
   * @var mixed 
   */
  public $type;

  /**
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   */
  public function __construct(XMLDocument $document, FormElement $element, $value = NULL) {
    $this->document = $document;
    $this->element = $element;
    $this->value = $value;
    list($this->type, $this->action) = $this->getTypeAndAction($document, $element, $value);
  }

  /**
   *
   * @param XMLDocument $document
   * @param FormElement $element
   * @param mixed $value
   * @return array
   */
  private function getTypeAndAction(XMLDocument $document, FormElement $element, $value = NULL) {
    if (isset($element->actions)) {
      return $element->actions->getAppropriateAction($document, $element, $value);
    }
    return array(Action::Undefined, NULL);
  }

  /**
   *
   * @param int $type
   * @param Action $action 
   */
  public function setTypeAndAction($type, Action $action) {
    $this->type = $type;
    $this->action = $action;
  }

  /**
   * 
   */
  public function execute() {
    if ($this->action) {
      return $this->action->execute($this->document, $this->element, $this->value);
    }
    return true;
  }

}

/**
 * Process the form generate a Document.
 */
class XMLFormProcessor {

  /**
   * Submitted form values.
   * 
   * @var FormValues 
   */
  protected $values;
  
  /**
   * 
   * @var XMLDocument
   */
  protected $document;

  /**
   *
   * @var NodeRegistry
   */
  protected $nodeRegistry;

  /**
   *
   * @var FormElementRegistry
   */
  protected $elementRegistry;

  /**
   *
   * @param type $form_state
   * @param type $definition 
   */
  public function __construct(FormValues $values, XMLDocument $document) {
    $this->values = $values;
    $this->document = $document;
    $this->nodeRegistry = $document->registry;
    $this->elementRegistry = FormElementRegistry::getInstance();
  }

  /**
   *
   * @param FormElement $element 
   */
  public function process(FormElement $element) {
    $actions = &$this->createActions($element);
    $actions = &$this->sortActions($actions);
    return $this->performActions($actions);
  }

  /**
   *
   * @param FormElements $elements
   * @return array
   */
  private function &createActions(FormElement $element) {
    $actions = new ArrayObject();
    $element->each(array($this, 'createAction'), $actions);
    $this->addDeleteActionsForRemovedFormElements($element, $actions);
    return $actions;
  }

  /**
   * If the registered node's element is no longer in the form, 
   * add an action to delete it.
   * 
   * @param XMLFormProcessAction $actions 
   */
  private function addDeleteActionsForRemovedFormElements(FormElement $element, $actions) {
    $remaning_elements = $this->getRemainingElements($element);
    $registered = $this->nodeRegistry->registered;
    foreach ($registered as $hash => $node) {
      if (array_search($hash, $remaning_elements) === FALSE) {
        $element = $this->elementRegistry->get($hash);
        $delete = $element->actions->delete;
        if (isset($delete)) {
          $action = new XMLFormProcessAction($this->nodeRegistry, $element);
          $action->setTypeAndAction(Action::Delete, $delete);
          $actions[] = $action;
        }
      }
    }
  }

  /**
   * Gets an array of hash identifier for the FormElements that are still in the Form.
   * 
   * @param FormElement $element
   * @return array
   */
  public function getRemainingElements(FormElement $element) {
    $remaining_elements = new ArrayObject();
    $element->each(array($this, 'storeHash'), $remaining_elements);
    return $remaining_elements->getArrayCopy();
  }

  /**
   * Stores the hash for the given element in the provided array object.
   * 
   * @param FormElement $element
   * @param ArrayObject $hashes
   * @return boolean
   */
  public function storeHash(FormElement $element, ArrayObject $hashes) {
    $hashes[] = $element->hash;
    return TRUE;
  }

  /**
   *
   * @param FormElement $element
   * @return XMLFormProcessAction 
   */
  public function createAction(FormElement $element, $actions) {
    $value = $this->values->getValue($element->hash);
    $actions[] = new XMLFormProcessAction($this->document, $element, $value);
    return TRUE;
  }

  /**
   *
   * @param array $actions
   * @return array
   */
  private function &sortActions($actions) {
    $delete = array();
    $update = array();
    $create = array();
    foreach ($actions as $action) {
      switch ($action->type) {
        case Action::Create:
          $create[] = $action;
          break;
        case Action::Update:
          $update[] = $action;
          break;
        case Action::Delete:
          $delete[] = $action;
          break;
      }
    }
    $actions = array('create' => $create, 'update' => $update, 'delete' => $delete);
    return $actions;
  }

  /**
   *
   * @param array $actions
   * @return Document
   */
  private function performActions(array &$actions) {
    /**
     * The create actions are loop in a while statement to allow for out of order construction of elements.
     * 
     * The order of action execution here is important, create actions should take place before update, 
     * as we can update newly created elements. Delete must come last as we want the update actions to run on elements 
     * that may then later be removed.
     */
    do {
      $continue = false;
      foreach ($actions['create'] as $key => $action) {
        if ($action->execute()) {
          $continue = true;
          unset($actions['create'][$key]);
        }
      }
    } while ($continue);
    foreach ($actions['update'] as $key => $action) {
      $action->execute();
    }
    foreach ($actions['delete'] as $key => $action) {
      $action->execute();
    }
    return $this->document;
  }

}