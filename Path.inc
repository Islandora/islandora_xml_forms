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

  public function __construct($path, $context) {
    $this->path = $path;
    $this->context = $context;
  }

  private function getContext(NodeRegistry $registry, FormElement $element) {
    switch ($this->context) {
      case self::DocumentContext:
        return NULL;
      case self::ParentContext:
        return $this->getParent($registry, $element);
      case self::SelfContext:
        return $this->getSelf($registry, $element);
      default:
        throw new Exception('Invalid Context.');
    }
  }

  private function getParent(NodeRegistry $registry, FormElement $element) {
    $parent = $element->parent;
    while ($parent) {
      // Check to see if it defines a read property.
      if (isset($parent->actions->read)) {
        $node = $registry->getNode($parent->hash);
        return isset($node) ? $node : FALSE; // Parent was not read from the document.
      }
      $parent = $parent->parent;
    }
    $type = $element->type;
    $title = $element->title;
    throw new Exception("The Form Element '{$title}' of type '{$type}' specifies a xpath context of parent but none is defined.");
  }

  private function getSelf(NodeRegistry $registry, FormElement $element) {
    $node = $registry->getNode($element);
    if ($node) {
      return $node;
    }
    throw new Exception('Context Self is not defined.');
  }

  public function query(NodeRegistry $registry, FormElement $element) {
    $context = $this->getContext($registry, $element);
    if ($context !== FALSE) {
      return $registry->document->query($this->path, $context);
    }
    return FALSE;
  }

  /**
   * 
   * @return DOMElement
   */
  public function asNode() {
    $path = new DOMElement('path');
    $path->appendChild(new DOMText($this->path));
    $path->setAttribute('context', $this->context);
    return $path;
  }

}