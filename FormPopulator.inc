<?php

// $Id$

/**
 * @file
 *
 */
class FormPopulator {

  /**
   * Drupal Form API form.
   *  
   * @var array 
   */
  protected $form;

  /**
   * Create form populator
   * 
   * @param array $form 
   */
  public function __construct(array &$form) {
    $this->form = &$form;
  }

  /**
   *
   * @param array $element
   */
  public function populateElement(array &$element) {
    if (isset($element['#xml_form'])) {
      
    }
    if ($element['#children']) {
      
    }
  }

  /**
   * 
   */
  public function populateElements(array &$elements) {
    foreach ($this->form as &$element) {
      $this->populateElement($element);
    }
  }

  /**
   * 
   */
  public function populateForm() {
    foreach ($this->form as &$element) {
      $this->populateElement($element);
      if ($element['#children']) {
        
      }
    }
  }

}