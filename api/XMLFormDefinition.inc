<?php

// $Id$

/**
 * @file
 * @author Nigel Banks
 *
 * Defines a FormDefinition class that allows XML form defintions to be processed by the FormGenerator, and other
 * classes.
 */
module_load_include('inc', 'php_lib', 'String');
module_load_include('inc', 'php_lib', 'Array');

/**
 * Describes a form
 */
class XMLFormDefinition {

  /**
   * Checks to see if a xml form definition is valid.
   * 
   * @param mixed $definition
   *   The xml form definition to validated. Can be a string a DOMDocument or a SimpleXMLElement
   * @return boolean
   *   TRUE if $definition is a valid form definition, FALSE otherwise.
   */
  public static function isValid(&$definition) {
    $path = drupal_get_path('module', 'xml_form_api') . '/XMLFormDefinition.xsd';
    if (is_string($definition)) {
      $document = new DOMDocument();
      $document->loadXML($definition);
      return @$document->schemaValidate($path);
    }
    else if (get_class($definition) == 'DOMDocument') {
      return @$definition->schemaValidate($path);
    }
    else if (get_class($definition) == 'SimpleXMLElement') {
      $form = dom_import_simplexml($definition);
      $document = new DOMDocument('1.0');
      $form = $document->importNode($form, true);
      $form = $document->appendChild($form);
      return @$document->schemaValidate($path);
    }
    return FALSE;
  }

  /**
   * Create
   */
  public static function Create(array $properties, array $form) {
    $definition = new SimpleXMLElement('<definition xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"></definition>');
    self::AddPropertiesToDefinition($definition, $properties);
    self::AddFormToDefinition($definition, new FormElement($form));
    return $definition;
  }

  /**
   * 
   * @param SimpleXMLElement $definition 
   * @param array $properties
   */
  private static function AddPropertiesToDefinition(SimpleXMLElement $definition, array &$properties) {
    $form_properties = $definition->addChild('properties');
    if (isset($properties['root_name'])) {
      $form_properties->addChild('root_name', $properties['root_name']);
    }
    if (isset($properties['schema_uri'])) {
      $form_properties->addChild('schema_uri', $properties['schema_uri']);
    }
    $namespaces = $form_properties->addChild('namespaces');
    if ($properties['default_uri']) {
      $namespaces->addAttribute('default', $properties['default_uri']);
    }
    if (isset($properties['namespaces'])) {
      foreach ($properties['namespaces'] as $prefix => $uri) {
        $namespace = $namespaces->addChild('namespace', $uri);
        $namespace->addAttribute('prefix', $prefix);
      }
    }
  }

  /**
   *
   * @param SimpleXMLElement $definition
   * @param FormElement $form
   */
  private static function AddFormToDefinition(SimpleXMLElement $definition, FormElement $root) {
    $form = $definition->addChild('form');
    $properties = $form->addChild('properties');
    foreach ($root->controls as $key => $value) {
      self::AddFormControlToProperties($properties, $key, $value);
    }
    $children = $form->addChild('children');
    foreach ($root->children as $key => $child) {
      self::AddFormElementToChildren($children, $key, $child);
    }
  }

  private static function AddFormControlToProperties(SimpleXMLElement $properties, $key, $value) {
    if ($value instanceof FormPropertyInterface) {
      $value = $value->getValue();
    }
    if (is_bool($value)) {
      $value = $value ? 'true' : 'false';
    }
    $key = trim($key, '#');
    if (!is_array($value)) {
      $control = $properties->addChild($key, $value);
      if ($key == 'option') { // Hacktastic...
        $control->addAttribute('key', $value);
      }
    }
    else {
      $control = $properties->addChild($key);
      foreach ($value as $index => $item) {
        $index = trim($index, '#');
        $name = ($key == 'options') ? 'option' : $index; // Hacktastic...
        self::AddFormControlToProperties($control, $name, $item);
      }
    }
  }

  private static function AddFormElementToChildren(SimpleXMLElement $children, $key, FormElement $child) {
    $element = $children->addChild('element');
    if (isset($key)) {
      $element->addAttribute('name', $key);
    }
    $properties = $element->addChild('properties');
    foreach ($child->controls as $key => $value) {
      self::AddFormControlToProperties($properties, $key, $value);
    }
    $children = $element->addChild('children');
    foreach ($child->children as $key => $grandchild) {
      self::AddFormElementToChildren($children, $key, $grandchild);
    }
  }

  /**
   * Get form properties.
   * 
   * @param sting $xml_definition
   * 
   * @return string 
   */
  public static function GetFormProperties(&$xml_definition) {
    if (!self::isValid($xml_definition)) {
      return array(
        'root_name' => NULL,
        'schema_uri' => NULL,
        'namespaces' => array(),
        'default_uri' => NULL
      );
    }
    $definition = simplexml_load_string($xml_definition);
    $properties = $definition->properties;
    $root_name = (string) $properties->root_name;
    $schema_uri = isset($properties->schema_uri) ? (string) $properties->schema_uri : NULL;
    $namespaces = array();
    $default_uri = NULL;
    if (isset($properties->namespaces)) {
      $default_uri = isset($properties->namespaces['default']) ? (string) $properties->namespaces['default'] : NULL;
      $children = $properties->namespaces->children();
      foreach ($children as $namespace) {
        $prefix = (string) $namespace['prefix'];
        $namespaces[$prefix] = (string) $namespace;
      }
    }
    return array(
      'root_name' => $root_name,
      'schema_uri' => $schema_uri,
      'namespaces' => $namespaces,
      'default_uri' => $default_uri,
    );
  }

  /**
   * Get a drupal form definition from the given XML based one.
   * 
   * @param string $xml_definition 
   * 
   * @return array
   */
  public static function GetDrupalForm(&$xml_definition) {
    if (!self::isValid($xml_definition)) {
      return array(); // Empty form.
    }
    $definition = simplexml_load_string($xml_definition);
    return self::GetElement($definition->form);
  }

  /**
   *
   * @param SimpleXMLElement $element
   * @return type 
   */
  private static function GetElementProperties(SimpleXMLElement $element) {
    $output = array();
    if (isset($element->properties)) {
      $properties = $element->properties->children();
      foreach ($properties as $property) {
        $name = $property->getName();
        $output["#$name"] = self::GetElementProperty($property);
      }
    }
    return $output;
  }

  /**
   *
   * @param SimpleXMLElement $property
   * @return type 
   */
  private static function GetElementProperty(SimpleXMLElement $property) {
    $children = $property->children();
    if (count($children) == 0) {
      return cast_string_to_boolean((string) $property); // If cast fails the string is returned.
    }
    $output = array();
    foreach ($children as $child) {
      $name = self::GetElementPropertyName($child);
      $output[$name] = self::GetElementProperty($child);
    }
    return $output;
  }

  private static function GetElementPropertyName(SimpleXMLElement $property) {
    if (isset($property['key'])) {
      $key = (string) $property['key'];
      return strcasecmp($key, 'NULL') == 0 ? NULL : $key;
    }
    return $property->getName();
  }

  private static function GetElementChildren(SimpleXMLElement $element) {
    $output = array();
    if (isset($element->children)) {
      $elements = $element->children->children();
      foreach ($elements as $element) {
        $key = isset($element['name']) ? (string) $element['name'] : NULL;
        array_add($output, $key, self::GetElement($element));
      }
    }
    return $output;
  }

  private static function GetElement(SimpleXMLElement $element) {
    $output = self::GetElementProperties($element);
    return array_merge($output, self::GetElementChildren($element));
  }

}

