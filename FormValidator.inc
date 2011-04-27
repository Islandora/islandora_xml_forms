<?php

// $Id$

/**
 * @file
 *
 */

class FormValidator {
  /**
   *
   * @var array 
   */
  protected $values;
  
  public function __construct(&$form_state) {
    $this->values = &$form_state['values'][Form::ROOT];
  }
}