<?php

/**
 * @file
 *
 */
module_load_include('inc', 'php_lib', 'DOMHelpers.inc');

/**
 * The purpose of this class is to register DOMNodes to FormElements and to generate additional FormElements for
 * FormElements that repersent several DOMNodes.
 */
class XMLFormGenerator {

  /**
   * Reference to the form were, generating.
   *
   * @var Form
   */
  protected $form;

  /**
   *
   * @var XMLDocument
   */
  protected $document;

  /**
   *
   * @var NodeRegistry
   */
  protected $registry;

  /**
   * Create a XMLFormGenerator
   *
   * @param XMLForm $form
   * @param XMLDocument $document
   */
  public function __construct(XMLForm $form, XMLDocument $document) {
    $this->form = $form;
    $this->document = $document;
    $this->registry = $document->registry;
  }

  /**
   * Generate additional elements based on the provided document.
   *
   * @param FormElements $elements
   */
  public function generate(FormElement $root) {
    $this->processElement($root);
    $root->eachDecendant(array($this, 'processElement'));
  }

  /**
   * Registers the given element generates stuff.
   *
   * @param FormElement $element
   */
  public function processElement(FormElement $element) {
    if (!$this->registry->isRegistered($element->hash)) {
      $nodes = $this->findNodes($element);
      $node = array_shift($nodes);
      if (isset($node)) {
        $this->registry->register($element->hash, $node);
        $duplicates = $this->createDuplicates($element, count($nodes));
        $this->registerDuplicates($duplicates, $nodes);
        foreach ($duplicates as $duplicate) {
          $duplicate->eachDecendant(array($this, 'processElement'));
        }
      }
      return TRUE;
    }
    return FALSE;
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
      $results = $reader->execute($this->document, $element);
      if ($results) {
        return dom_node_list_to_array($results);
      }
    }
    return array();
  }

  /**
   *
   * @param FormElement $element
   * @param integer $count
   */
  private function createDuplicates(FormElement $element, $count) {
    $output = array();
    for ($i = 0; $i < $count; $i++) {
      $clone = $this->form->duplicateOriginal($element->hash);
      $element->parent[] = $clone;
      $output[] = $clone;
    }
    return $output;
  }

  /**
   *
   * @param array $elements
   * @param array $nodes
   */
  private function registerDuplicates(array $elements, array $nodes) {
    $count = count($elements);
    for ($i = 0; $i < $count; $i++) {
      $element = $elements[$i];
      $node = $nodes[$i];
      $this->registry->register($element->hash, $node);
    }
  }

}
