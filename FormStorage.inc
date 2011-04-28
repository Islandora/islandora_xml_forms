<?php

// $Id$

/**
 * @file
 *
 */

/**
 * 
 */
class FormStorage {
  /**
   * The root element where we store all the required info.
   */
  const STORAGE_ROOT = 'xml_form_storage';
  const DEFINITION = 'definition';
  const DOCUMENT = 'document';

  /**
   *
   * @var array 
   */
  protected $storage;
  /**
   *
   * @var boolean 
   */
  protected $initialized;

  /**
   *
   * @param array $form_state 
   */
  public function __construct(array &$form_state) {
    if(empty($form_state['storage'])) {
      $form_state['storage'] = array();
    }
    $this->setStorage($form_state['storage']);
  }

  /**
   * Sets the storage member to the correct index and determines if the storage array
   * has been initialized.
   * 
   * @param array $storage
   *   Storage array.
   */
  private function setStorage(array &$storage) {
    if (empty($storage[self::STORAGE_ROOT])) {
      $storage[self::STORAGE_ROOT] = array();
      $this->storage = &$storage[self::STORAGE_ROOT];
      $this->initialized = false; // Created for the first time.
    }
    else {
      $this->storage = &$storage[self::STORAGE_ROOT];
      $this->initialized = true; // Created for the nth time.
    }
  }
  
  /**
   * Has the FormStorage been initialized?
   * 
   * @return boolean 
   */
  public function isInitialized() {
    return $this->initialized;
  }
  
  /**
   *
   * @param FormDefinition $definition 
   */
  public function setDefinition(FormDefinition $definition) {
    $this->storage[self::DEFINITION] = $definition;
  }
  
  /**
   *
   * @return FormDefinition
   */
  public function getDefinition() {
    return $this->storage[self::DEFINITION];
  }
  
  /**
   *
   * @param XMLDocument $document 
   */
  public function setDocument(XMLDocument $document) {
    $this->storage[self::DOCUMENT] = $document;
  }
  
  /**
   *
   * @return XMLDocument 
   */
  public function getDocument() {
    return $this->storage[self::DOCUMENT];
  }

}