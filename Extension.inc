<?php

// $Id$

/**
 * @file
 *
 * Models a <xs:extension> element.
 */
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'schema_api', 'All');
module_load_include('inc', 'schema_api', 'Choice');
module_load_include('inc', 'schema_api', 'Sequence');
module_load_include('inc', 'schema_api', 'Group');

/**
 * Models a <xs:extension> element.
 */
class XMLSchemaExtension extends XMLSchemaNode {
  /**
   * Constants.
   */
  const LocalName = 'extension';

  /**
   * 
   * @param DOMElement $node 
   */
  public function __construct(DOMElement $node) {
    parent::__construct($node);
    $this->protected->add(
        array(
          'all' => $this->createChild(XMLSchemaAll::LocalName),
          'choice' => $this->createChild(XMLSchemaChoice::LocalName),
          'sequence' => $this->createChild(XMLSchemaSequence::LocalName),
          'group' => $this->createChild(XMLSchemaGroup::LocalName),
        )
    );
  }

  /**
   * Returns the local name of this object.
   * 
   * @return string
   */
  protected function getLocalName() {
    return self::LocalName;
  }

}