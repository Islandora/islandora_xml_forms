<?php

// $Id$

/**
 * @file
 *
 */
class FormProcessor {

  /**
   * Submitted form values.
   * @var array 
   */
  protected $values;

  /**
   *
   * @param type $form_state
   * @param type $definition 
   */
  public function __construct(&$form_state, &$definition) {
    $this->values = $form_state['values'][Form::ROOT];
    
  }

}

