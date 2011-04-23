<?php

// $Id$

/**
 * @file
 *
 */
class FormGenerator {

  /**
   * The definition of the form to generate.
   * 
   * @var FormDefinition
   */
  protected $definition;
  /**
   * When defined this member is used to prepopulate the form before returning the Drupal API form.
   *
   * @var FormPopulator
   */
  protected $populator;

  /**
   * Creates a form generator.
   * 
   * @param FormDefinition $form_definition
   * @param FormPopulator $form_populator
   */
  public function __construct($form_definition, $form_populator = NULL) {
    if(get_class($form_definition) != 'FormDefinition') {
      throw new Exception('');
    }
    if(isset($form_populator) && get_class($form_populator) != 'FormPopulator') {
      throw new Exception('');
    }
  }

  /**
   * Creates a Drupal API form, from the form definition.
   *
   * @return array
   *   Returns a Drupal API form.
   */
  public function generate() {
    
  }

}

