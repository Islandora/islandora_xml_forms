<?php

// $Id$

/**
 * @file
 *
 */

/**
 * A class of static helper methods.
 */
class Utils {

  public static function convertNodeToPath(DOMNode &$node) {
    $node = self::getXPathToNode($node);
  }

  public static function getXPathToNode(DOMNode $node) {
    $path = self::getXPathDOMNodeFragment($node);
    while (($node = $node->parentNode) && get_class($node) == 'DOMElement') {
      $name = self::getXPathDOMNodeFragment($node);
      $path = "$name/$path";
    }
    return "/$path";
  }

  public static function getChildNodeXPathIndex(DOMNode $parent, DOMNode $child) {
    $xpath = new DOMXPath($child->ownerDocument);
    $results = $xpath->query('child::*', $parent);
    $count = $results->length;
    $index = 1;
    for ($i = 0; $i < $count; $i++) {
      $node = $results->item($i);
      if ($child->isSameNode($node)) {
        break;
      }
      $index++;
    }
    return $index;
  }

  public static function getXPathDOMNodeFragment(DOMNode $node) {
    if (get_class($node) == 'DOMAttr') {
      return "@{$node->nodeName}";
    }
    else {
      $index = self::getChildNodeXPathIndex($node->parentNode, $node);
      return "{$node->nodeName}[$index]";
    }
  }

}