<?php

// $Id$

/**
 * @file
 *
 */

/**
 * Interface that all actions support.
 */
interface ActionInterface {

  /**
   * Perform the designated action.
   * 
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   */
  public function execute(NodeRegistry $registry, FormElement $element, $value = NULL);
}