<?php

// $Id$

/**
 * @file
 *
 */

/**
 * Interface that all actions support.
 */
interface Action {
  /**
   * Constants
   */
  const Undefined = 0;
  const Create = 1;    // Create a DOMNode
  const Read = 2;      // Read from a DOMNode
  const Update = 3;    // Update a DOMNode
  const Delete = 4;    // Delete a DOMNode

  /**
   * Perform the designated action.
   * 
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   */
  public function execute(NodeRegistry $registry, FormElement $element, $value = NULL);

  /**
   * Check to see if this action should be performed.
   * 
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value 
   */
  public function shouldExecute(NodeRegistry $registry, FormElement $element, $value = NULL);

  /**
   * Converts this action to its XML repersentation.
   * 
   * @param DOMDocument $document
   * @return DOMElement
   */
  public function asNode(DOMDocument $document);
}