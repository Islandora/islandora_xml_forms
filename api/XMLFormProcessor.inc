<?php

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'XMLDocument');

/**
 * Encapsulates the required data to perform a CRUD action.
 */
class XMLFormProcessAction {

  /**
   * Action to perform.
   *
   * @var Action
   */
  public $action;

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
   * Creates a XMLFormProcessAction instance.
   *
   * @param Action $action
   * @param FormElement $element
   * @param mixed $value
   */
  public function __construct(Action $action, FormElement $element, $value = NULL) {
    $this->action = $action;
    $this->element = $element;
    $this->value = $value;
  }

  /**
   * Executes the action.
   *
   * @param XMLDocument $document
   *
   * @return boolean
   */
  public function execute(XMLDocument $document) {
    return $this->action->execute($document, $this->element, $this->value);
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
   * Creates a XMLFormProcessor instance.
   *
   * This is used to modify the given document with the values and actions specified by the submitted form.
   *
   * @param FormValues $values
   *   The submitted values for this form.
   * @param XMLDocument $definition
   *   The document to be modified with the submitted values.
   * @param FormElementRegistry $registry
   *   The registry used to access form element information required to modify the document correctly.
   */
  public function __construct(FormValues $values, XMLDocument $document, FormElementRegistry $registry) {
    $this->values = $values;
    $this->document = $document;
    $this->nodeRegistry = $document->registry;
    $this->elementRegistry = $registry;
  }

  /**
   *
   * @param FormElement $element
   */
  public function process(FormElement $element) {
    /**
     * The order of action execution here is important, create actions should take place before update,
     * newly created elements can be registered as we can update newly created elements. Delete must come
     * last as we want the update actions to run on elements that may then later be removed.
     */
    $elements = $element->flatten();
    $this->createNodes($this->getActions($elements, 'create'));
    $this->modifyNodes($this->getActions($elements, 'update'));
    $this->modifyNodes($this->getActions($elements, 'delete'));
    $this->modifyNodes($this->getRemovedFormElementsDeleteActions($element));
    return $this->document;
  }

  /**
   *
   * @param array $elements
   * @param string $type
   *
   * @return array
   */
  private function getActions(array &$elements, $type) {
    $actions = array();
    foreach ($elements as $key => $element) {
      $value = $this->values->getValue($element->hash);
      $action = isset($element->actions->$type) ? $element->actions->$type : NULL;
      if (isset($action) && $action->shouldExecute($this->document, $element, $value)) {
        $actions[] = new XMLFormProcessAction($action, $element, $value);
        unset($elements[$key]); // Remove from the list of actionable elements.
      }
    }
    return $actions;
  }

  /**
   * Creates DOMNodes in the document.
   *
   * @param array $actions
   */
  private function createNodes(array $actions) {
    /**
     * The create actions are loop in a while statement to allow for out of order construction of elements.
     */
    do {
      $continue = FALSE;
      foreach ($actions as $key => $action) {
        if ($action->execute($this->document)) {
          $continue = TRUE;
          unset($actions[$key]);
        }
      }
    } while ($continue);
  }

  /**
   * Updates/Delete's DOMNodes in the document.
   *
   * @param array $actions
   */
  private function modifyNodes(array $actions) {
    foreach ($actions as $action) {
      $action->execute($this->document);
    }
  }

  /**
   * If the registered node's element is no longer in the form,
   * add an action to delete it.
   *
   * @param array $actions
   */
  private function getRemovedFormElementsDeleteActions(FormElement $element) {
    $actions = array();
    $elements = $element->flatten();
    $registered = $this->nodeRegistry->getRegistered();
    foreach ($registered as $hash => $node) {
      if (isset($elements[$hash]) === FALSE) {
        $element = $this->elementRegistry->get($hash);
        $delete = isset($element->actions->delete) ? $element->actions->delete : NULL;
        if (isset($delete)) {
          $actions[] = new XMLFormProcessAction($delete, $element);
        }
      }
    }
    return $actions;
  }

}
