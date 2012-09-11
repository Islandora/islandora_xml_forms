<?php



/**
 * @file
 *
 */
module_load_include('inc', 'objective_forms', 'FormPropertyInterface');
module_load_include('inc', 'xml_form_api', 'Create');
module_load_include('inc', 'xml_form_api', 'Read');
module_load_include('inc', 'xml_form_api', 'Update');
module_load_include('inc', 'xml_form_api', 'Delete');

/**
 * Container Class for all actions.
 */
class Actions implements FormPropertyInterface {

  /**
   * If executed this object will create one or more DOMNodes in the Document.
   *
   * @var Create
   */
  public $create;

  /**
   * If executed this object will read one or more DOMNodes in the Document. It is used to populate the form.
   *
   * @var Read
   */
  public $read;

  /**
   * If executed this object will update one or more DOMNodes values in the Document.
   *
   * @var Update
   */
  public $update;

  /**
   * If executed this object will delete one or more DOMNodes in the Document.
   * @var Delete
   */
  public $delete;

  /**
   * Creates and Actions instance.
   *
   * @param array $value
   */
  public function __construct($value) {
    if (isset($value['create']) && is_array($value['create'])) {
      $this->create = new Create($value['create']);
    }
    if (isset($value['read']) && is_array($value['read'])) {
      $this->read = new Read($value['read']);
    }
    if (isset($value['update']) && is_array($value['update'])) {
      $this->update = new Update($value['update']);
    }
    if (isset($value['delete']) && is_array($value['delete'])) {
      $this->delete = new Delete($value['delete']);
    }
  }

  /**
   * Get the appropriate Action to perform.
   *
   * This is called by the FormProcesor, depending on the state of certian variables one of
   * the following actions will be returned.
   *
   * Note that these cases are the default, depending on other settings within the action
   * element the return Action may differ.
   *
   * Create
   *  The FormElement in question doesn't have a registered DOMNode.
   * Update
   *  The FormElement that corresponds to the Node in question is still part of the definition.
   * Delete
   *  The FormElement in question, no longer exists in the form and it has a registered DOMNode.
   *
   * @param XMLDocument $document
   * @param DOMNode $node
   * @param mixed $value
   *
   * @return array
   *  Gets the action type and the Action object. Example:
   *    array(string, Action);
   */
  public function getAppropriateAction(XMLDocument $document, FormElement $element, $value = NULL) {
    if ($this->shouldDelete($document, $element, $value)) {
      return array(Action::Delete, $this->delete);
    }
    elseif ($this->shouldUpdate($document, $element, $value)) {
      return array(Action::Update, $this->update);
    }
    elseif ($this->shouldCreate($document, $element, $value)) { // Node is not registered
      return array(Action::Create, $this->create);
    }
    return array(Action::Undefined, NULL);
  }

  /**
   * Check the Delete Action's properties to determine if we should remove this FormElement.
   *
   * @param XMLDocument $document
   * @param FormElement $element
   * @param mixed $value
   *
   * @return boolean
   */
  private function shouldDelete(XMLDocument $document, FormElement $element, $value = NULL) {
    if (isset($this->delete)) {
      return $this->delete->shouldExecute($document, $element, $value);
    }
    return FALSE;
  }

  /**
   * Check the Update Action's properties to determine if we should update this FormElement.
   *
   * @param XMLDocument $document
   * @param FormElement $element
   * @param mixed $value
   * @return boolean
   */
  private function shouldUpdate(XMLDocument $document, FormElement $element, $value = NULL) {
    if (isset($this->update)) {
      return $this->update->shouldExecute($document, $element, $value);
    }
    return FALSE;
  }

  /**
   * Check the Create Action's properties to determine if we should create this FormElement.
   *
   * @param XMLDocument $document
   * @param FormElement $element
   * @param mixed $value
   *
   * @return boolean
   */
  private function shouldCreate(XMLDocument $document, FormElement $element, $value = NULL) {
    if (isset($this->create)) {
      return $this->create->shouldExecute($document, $element, $value);
    }
    return FALSE;
  }

  /**
   * Converts this object to a Drupal Form Property value.
   *
   * @return mixed
   *   Returns the array repersentation of this class, which can be used within a Drupal Form.
   */
  public function toDrupalForm() {
    return array(
      'create' => isset($this->create) ? $this->create->toDrupalForm() : NULL,
      'read' => isset($this->read) ? $this->read->toDrupalForm() : NULL,
      'update' => isset($this->update) ? $this->update->toDrupalForm() : NULL,
      'delete' => isset($this->delete) ? $this->delete->toDrupalForm() : NULL,
    );
  }

}
