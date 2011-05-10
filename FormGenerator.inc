<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'xml_form_api', 'FormDefinition');
module_load_include('inc', 'xml_form_api', 'FormPopulator');

/**
 * The purpose of this class is to register DOMNodes to FormElements and to generate additional FormElements for
 * FormElements that repersent several DOMNodes.
 */
class FormGenerator {

  /**
   * Reference to the form were, generating.
   * 
   * @var Form
   */
  protected $form;
  /**
   *
   * @var NodeRegistry
   */
  protected $registry;

  /**
   * Create a FormGenerator
   * 
   * @param FormDefinition $definition
   * @param NodeRegistry $registry 
   */
  public function __construct(Form $form, NodeRegistry $registry) {
    $this->form = $form;
    $this->registry = $registry;
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
  }

  /**
   *
   * @param FormElement $element
   * @return array 
   */
  private function findNodes(FormElement $element) {
    $actions = $element->actions;
    $reader = $actions ? $actions->read : NULL;
    if ($reader) {
      $results = $reader->execute($this->registry, $element);
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
      $output[] = $this->form->duplicate($element);
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

}