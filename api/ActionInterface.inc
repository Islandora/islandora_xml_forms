<?php



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
  const Create = 1; // Create a DOMNode
  const Read = 2; // Read from a DOMNode
  const Update = 3; // Update a DOMNode
  const Delete = 4; // Delete a DOMNode

  /**
   * Perform the designated action.
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   */
  public function execute(XMLDocument $document, FormElement $element, $value = NULL);

  /**
   * Check to see if this action should be performed.
   *
   * @param NodeRegistry $registry
   * @param FormElement $element
   * @param mixed $value
   */
  public function shouldExecute(XMLDocument $document, FormElement $element, $value = NULL);

  /**
   * Converts this object to a Drupal Form Property value.
   *
   * @return mixed
   *   Returns an array repersentation of this class, which can be used within a Drupal Form Property '#actions'.
   */
  public function toDrupalForm();
}
