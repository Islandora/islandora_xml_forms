<?php

// $Id$

/**
 * @file
 *
 */

/**
 * A class used to store/query XPath's.
 */
class Path {
  /**
   * Constants that define the possible contexts for this path to be executed in.
   */
  const DocumentContext = 'document';
  const ParentContext = 'parent'; // Only works if this element is nested within an element that has a full path.
  const SelfContext = 'self'; // Only works if a node is registered, with the given element.

  /**
   * The FormElement this path belongs to... @todo Needs better description.
   * 
   * @var FormElement
   */
  public $element;
  /**
   * XPath to query with.
   * 
   * @var string
   */
  public $path;
  /**
   * Query Context, either document, parent or self.
   * 
   * @var string 
   */
  public $context;

  public function __construct(FormElement $element, $path, $context) {
    $this->element = $element;
    $this->path = $path;
    $this->context = $context;
  }

  private function getContext(NodeRegistry $registry) {
    switch ($this->context) {
      case self::DocumentContext:
        return NULL;
      case self::ParentContext:
        return $this->getParent($registry);
      case self::SelfContext:
        return $this->getSelf($registry);
      default:
        throw new Exception('Invalid Context.');
    }
  }

  private function getParent(NodeRegistry $registry) {
    $element = $this->element->parent;
    while ($element) {
      $node = $registry->getNode($element);
      if ($node) {
        return $node;
      }
    }
    throw new Exception('Context Parent is not defined.');
  }

  private function getSelf(NodeRegistry $registry) {
    $node = $registry->getNode($this->element);
    if ($node) {
      return $node;
    }
    throw new Exception('Context Self is not defined.');
  }

  public function query(NodeRegistry $registry) {
    $context = $this->getContext($registry);
    return $registry->document->query($this->path, $context);
  }

}