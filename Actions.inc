<?php

// $Id$

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
   * Create a Actions object from an XML Definition defined by FormDefinition.xsd
   * 
   * @param string $xml_definition
   * @return Actions
   */
  public static function createFromXMLDefiniton($xml_definition) {
    return self::createFromSimpleXML(simplexml_load_string($xml_definition));
  }

  /**
   * Create a Actions object from an XML Definition defined by FormDefinition.xsd
   * 
   * @param SimpleXMLElement $actions 
   * @return Actions
   */
  public static function createFromSimpleXML(SimpleXMLElement $actions) {
    $create = isset($actions->create) ? Create::createFromSimpleXML($actions->create) : NULL;
    $read = isset($actions->read) ? Read::createFromSimpleXML($actions->read) : NULL;
    $update = isset($actions->update) ? Update::createFromSimpleXML($actions->update) : NULL;
    $delete = isset($actions->delete) ? Delete::createFromSimpleXML($actions->delete) : NULL;
    return new self($create, $read, $update, $delete);
  }

  /**
   * 
   */
  public function __construct($value) {
    if (isset($value['read'])) {
      $this->read = new Read($value['read']);
    }
  }

  /**
   * Create Actions.
   * 
   * @param Create $create
   * @param Read $read
   * @param Update $update
   * @param Delete $delete 
   *
    public function __construct(Create $create = NULL, Read $read = NULL, Update $update = NULL, Delete $delete = NULL) {
    $this->create = $create;
    $this->read = $read;
    $this->update = $update;
    $this->delete = $delete;
    } */

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
   * @param FormElement $element
   * @param DOMNode $node
   * @param mixed $value
   * 
   * @return Action type and Action
   */
  public function getAppropriateAction(NodeRegistry $registry, FormElement $element, $value = NULL) {
    $node = $registry->getNode($element);
    $modify_or_delete = isset($node);
    if ($modify_or_delete) {
      if ($this->shouldDelete($registry, $element, $value)) {
        return array(Action::Delete, $this->delete);
      }
      else if ($this->shouldUpdate($registry, $element, $value)) {
        return array(Action::Update, $this->update);
      }
    }
    else if ($this->shouldCreate($registry, $element, $value)) {
      return array(Action::Create, $this->create);
    }
  }

  /**
   * Check the Delete Action's properties to determine if we should remove this FormElement.
   * 
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   * @return boolean
   */
  private function shouldDelete(NodeRegistry $registry, FormElement $element, $value = NULL) {
    if (isset($this->delete)) {
      return $this->delete->shouldExecute($registry, $element, $value);
    }
    return FALSE;
  }

  /**
   * Check the Update Action's properties to determine if we should update this FormElement.
   * 
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   * @return boolean
   */
  private function shouldUpdate(NodeRegistry $registry, FormElement $element, $value = NULL) {
    if (isset($this->update)) {
      return $this->update->shouldExecute($registry, $element, $value);
    }
    return FALSE;
  }

  /**
   * Check the Create Action's properties to determine if we should create this FormElement.
   * 
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   * @return boolean
   */
  private function shouldCreate(NodeRegistry $registry, FormElement $element, $value = NULL) {
    if (isset($this->create)) {
      return $this->create->shouldExecute($registry, $element, $value);
    }
    return FALSE;
  }

  /**
   * 
   * @return DOMNode
   */
  public function asNode(DOMDocument $document) {
    $actions = $document->createElement('actions');
    if (isset($this->create)) {
      $actions->appendChild($this->create->asNode($document));
    }
    if (isset($this->read)) {
      $actions->appendChild($this->read->asNode($document));
    }
    if (isset($this->update)) {
      $actions->appendChild($this->update->asNode($document));
    }
    if (isset($this->delete)) {
      $actions->appendChild($this->delete->asNode($document));
    }
    return $actions;
  }

  /**
   *
   * @return array
   */
  public function getValue() {
    return array(
      'create' => isset($this->create) ? $this->create->getValue() : NULL,
      'read' => isset($this->read) ? $this->read->getValue() : NULL,
      'update' => isset($this->update) ? $this->update->getValue() : NULL,
      'delete' => isset($this->delete) ? $this->delete->getValue() : NULL,
    );
  }

}