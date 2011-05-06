<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'Create');
module_load_include('inc', 'xml_form_api', 'Read');
module_load_include('inc', 'xml_form_api', 'Update');
module_load_include('inc', 'xml_form_api', 'Delete');

/**
 * Container Class for all actions.
 */
class Actions {

  /**
   *
   * @var Create
   */
  public $create;
  /**
   *
   * @var Read
   */
  public $read;
  /**
   *
   * @var Update
   */
  public $update;
  /**
   *
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
    $delete = isset($actions->update) ? Delete::createFromSimpleXML($actions->delete) : NULL;
    return new self($create, $read, $update, $delete);
  }

  /**
   * Create Actions.
   * 
   * @param Create $create
   * @param Read $read
   * @param Update $update
   * @param Delete $delete 
   */
  public function __construct(Create $create = NULL, Read $read = NULL, Update $update = NULL, Delete $delete = NULL) {
    $this->create = $create;
    $this->read = $read;
    $this->update = $update;
    $this->delete = $delete;
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
   * @return Action
   */
  public function getAppropriateAction() {
    
  }

}