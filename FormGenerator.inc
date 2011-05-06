<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormDefinition');
module_load_include('inc', 'xml_form_api', 'FormPopulator');
module_load_include('inc', 'xml_form_api', 'Document');

/**
 * The purpose of this class is to generate a Drupal Form from a FormDefinition.
 */
class FormGenerator {

  /**
   *
   * @var NodeRegistry
   */
  protected $registry;
  /**
   *
   * @var Document
   */
  protected $document;

  /**
   * Create a FormGenerator
   * 
   * @param FormDefinition $definition
   * @param NodeRegistry $registry 
   */
  public function __construct(NodeRegistry $registry) {
    $this->registry = $registry;
    $this->document = $registry->document;
  }

  /**
   * Generate additional elements based on the provided document.
   * 
   * @param FormElements $elements 
   */
  public function generate(FormElements $elements) {
    $registry = $this->registry;
    foreach ($elements as $element) {
      if (!$this->registry->isElementRegistered($element)) {
        $nodes = $this->findNodes($element);
        $form_elements = $this->createDuplicates($elements, $element, count($nodes));
        $this->registerNodes($form_elements, $nodes);
      }
    }
    return $elements;
  }

  /**
   *
   * @param FormElement $element
   * @return array 
   */
  private function findNodes(FormElement $element) {
    $properties = $element->properties;
    $reader = $properties ? $properties->read : null;
    if ($reader) {
      $results = $reader->read($this->registry);
      return Utils::DOMNodelistToArray($results);
    }
    return array();
  }

  /**
   *
   * @param FormElement $element
   * @param integer $count 
   */
  private function createDuplicates(FormElements $elements, FormElement $element, $count) {
    $output = array($element);
    for ($i = 1; $i < $count; $i++) {
      $output[] = $elements->duplicateElement($element->hash);
    }
    return $output;
  }

  /**
   *
   * @param array $elements
   * @param array $nodes 
   */
  private function registerNodes(array $elements, array $nodes) {
    $iterator = new MultipleIterator();
    $iterator->attachIterator(new ArrayIterator($elements));
    $iterator->attachIterator(new ArrayIterator($nodes));
    foreach ($iterator as $set) {
      list($element, $node) = $set;
      $this->registry->register($element, $node);
    }
  }

  /**
   * Elements.
   * 
   * Duplicate elements where more than one is selected
   * Store the value for the selected element. Don't overwrite the default_value...
   * Generate the form using the selected values instead of the default values. 
   * Subsequent calls don't need to populate the form.
   * 
   */
  /**
   * For the purpose of associating nodes with form elements, we can do a first come first serve basis.
   * 
   * That is for a particular query we return all matching elements. But before we associate a node with a form element,
   * we check if it has already been associated with an existing form element. If so we try the next availible node from the list, repeating the
   * process until we found a free node which we can associate or we run out of nodes to associate.
   * 
   */
}