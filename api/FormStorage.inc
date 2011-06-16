<?php

// $Id$

/**
 * @file
 *
 */

/**
 * Class for controling persistant data for the form.
 */
class XMLFormStorage {
  /**
   * The root element where we store all the required info.
   */
  const STORAGE_ROOT = 'xml_form_storage';

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
   * Create FormStorage.
   * @param array $form_state 
   */
  public function __construct(array &$form_state) {
    if (empty($form_state['storage'])) {
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
   * Get a value from storage.
   * 
   * @param mixed $name
   * @return mixed 
   */
  public function __get($name) {
    if (isset($this->storage[$name])) {
      return $this->storage[$name];
    }
    return NULL;
  }

  /**
   * Store a value.
   * 
   * @param mixed $name
   * @param mixed $value 
   */
  public function __set($name, $value) {
    $this->storage[$name] = $value;
  }

}