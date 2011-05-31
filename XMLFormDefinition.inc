<?php

// $Id$

/**
 * @file
 * @author Nigel Banks
 *
 * Defines a FormDefinition class that allows XML form defintions to be processed by the FormGenerator, and other
 * classes.
 */
module_load_include('inc', 'xml_form_api', 'XMLFormProperties');

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
      return $document->schemaValidate($path);
    }
    else if (get_class($definition) == 'DOMDocument') {
      return $definition->schemaValidate($path);
    }
    else if (get_class($definition) == 'SimpleXMLElement') {
      $form = dom_import_simplexml($definition);
      $document = new DOMDocument('1.0');
      $form = $document->importNode($form, true);
      $form = $document->appendChild($form);
      return $document->schemaValidate($path);
    }
    return FALSE;
  }

  /**
   * Get form properties.
   * 
   * @param sting $xml_definition
   * 
   * @return string 
   */
  public static function GetFormProperties(&$xml_definition) {
    if (!self::isValid($definition)) {
      throw new Exception('The given XMLFormDefinition is invalid.');
    }
    $form = simplexml_load_string($xml_definition);
    $document = $form->properties->document;
    $root = array(
      'prefix' => (string) $document->root->prefix,
      'local_name' => (string) $document->root->local_name,
      'uri' => (string) $document->root->uri,
    );
    $namespaces = array();
    if (isset($document->namespaces)) {
      $children = $document->namespaces->children();
      foreach ($children as $namespace) {
        $prefix = (string) $namespace['prefix'];
        $namespaces[$prefix] = (string) $namespace;
      }
    }
    $schema = (string) $form->properties->document->schema;
    return array(
      'document' => array(
        'root' => $root,
        'namespaces' => $namespaces,
        'schema' => $schema
    ));
  }

  /**
   * Get a drupal form definition from the given XML based one.
   * 
   * @param string $xml_definition 
   * 
   * @return array
   */
  public static function GetDrupalFormDefinition(&$xml_definition) {
    if (!self::isValid($definition)) {
      throw new Exception('The given XMLFormDefinition is invalid.');
    }
    $form = simplexml_load_string($xml_definition);
    $elements = $form->elements->children();
    return self::GetFormElements($form->elements);
  }

  /**
   * Get the values for all the child elements.
   * 
   * @param SimpleXMLElement $element
   * @return array
   */
  private static function GetFormElements(SimpleXMLElement $element) {
    $form = array();
    $elements = $element->children();
    foreach ($elements as $element) {
      $name = (string) $element['key'];
      $value = self::GetFormElementValue($element);
      if ($name) {
        $form[$name] = $value;
      }
      else {
        $form[] = $value;
      }
    }
    return $form;
  }

  /**
   * Get the value for a given form element.
   * 
   * @param SimpleXMLElement $element
   * @return array
   */
  private static function GetFormElementValue(SimpleXMLElement $element) {
    $value = array();
    $controls = $element->xpath("child::*[local-name() != 'children']");
    foreach ($controls as $control) {
      $name = $control->getName();
      $value["#$name"] = self::GetFormControlValue($name, $control);
    }
    if (isset($element->children)) {
      $children = self::GetFormElements($element->children);
      $value = array_merge($value, $children);
    }
    return $value;
  }

  /**
   * Get the value for a given form control.
   * 
   * @param string $name
   * @param SimpleXMLElement $control
   * 
   * @return mixed
   */
  private static function GetFormControlValue($name, SimpleXMLElement $control) {
    $function = self::getSimpleXMLConvertFunction($name);
    return call_user_func(array('XMLFormDefinition', $function), $control);
  }

  /**
   * Get a map of form controls as keys, and their types as values.
   * 
   * @return array
   */
  private static function getTypeMap() {
    static $type_map;
    if (empty($type_map)) {
      $filename = drupal_get_path('module', 'xml_form_api') . '/XMLFormDefinition.xsd';
      $form_definition = simplexml_load_file($filename);
      $filter_types = "@type='string' or @type = 'boolean' or @type = 'integer' or @type = 'float' or @type = 'array' or @type = 'mixed'";
      $form_controls = $form_definition->xpath("/xs:schema/xs:complexType/xs:all/xs:element[$filter_types]");
      foreach ($form_controls as $form_control) {
        $name = (string) $form_control['name'];
        $type = (string) $form_control['type'];
        $type_map[$name] = $type;
      }
    }
    return $type_map;
  }

  /**
   * Based on the given name find a conversion function to transform it from a SimpleXMLElement value
   * to a PHP value.
   * 
   * @param string $name 
   * 
   * @return string
   */
  private static function getSimpleXMLConvertFunction($name) {
    static $convert_functions;
    if (empty($convert_functions)) {
      $type_function_map = array(
        'string' => 'convertSimpleXMLToString',
        'boolean' => 'convertSimpleXMLToBoolean',
        'integer' => 'convertSimpleXMLToInteger',
        'float' => 'convertSimpleXMLToFloat',
        'array' => 'convertSimpleXMLToArray',
        'mixed' => 'convertSimpleXMLToMixed',
      );
      $type_map = self::getTypeMap();
      foreach ($type_map as $key => $type) {
        $convert_functions[$key] = $type_function_map[$type];
      }
    }
    return $convert_functions[$name];
  }

  /**
   * Convert String Form Control from XML to PHP
   * 
   * @param SimpleXMLElement $element 
   * 
   * @return string
   */
  private static function convertSimpleXMLToString(SimpleXMLElement $element) {
    return (string) $element;
  }

  /**
   * Convert Boolean Form Control from XML to PHP
   * 
   * @param SimpleXMLElement $element 
   * 
   * @return boolean
   */
  private static function convertSimpleXMLToBoolean(SimpleXMLElement $element) {
    return strcasecmp((string) $element, 'true') === 0 ? true : false;
  }

  /**
   * Convert Integer Form Control from XML to PHP
   * 
   * @param SimpleXMLElement $element 
   * 
   * @return int
   */
  private static function convertSimpleXMLToInteger(SimpleXMLElement $element) {
    return (int) $element;
  }

  /**
   * Convert Float Form Control from XML to PHP
   * 
   * @param SimpleXMLElement $element 
   * 
   * @return float
   */
  private static function convertSimpleXMLToFloat(SimpleXMLElement $element) {
    return (float) $element;
  }

  /**
   * Convert Array Form Control from XML to PHP
   * 
   * @param SimpleXMLElement $element 
   * 
   * @return array
   */
  private static function convertSimpleXMLToArray(SimpleXMLElement $element) {
    $output = array();
    $keys = $element->xpath('key');
    foreach ($keys as $key) {
      if (($name = (string) $key['name']) != null) {
        $output[$name] = self::convertSimpleXMLArrayKey($key);
      }
      else {
        $output[] = self::convertSimpleXMLArrayKey($key);
      }
    }
    return $output;
  }

  /**
   * Convert Array Key Form Control from XML to PHP
   * 
   * @param SimpleXMLElement $key
   * 
   * @return mixed
   */
  private static function convertSimpleXMLArrayKey(SimpleXMLElement $key) {
    if (isset($key->value)) {
      switch ($key->value['type']) {
        case 'string':
          return self::convertSimpleXMLToString($key->value);
        case 'integer':
          return self::convertSimpleXMLToInteger($key->value);
        case 'boolean':
          return self::convertSimpleXMLToBoolean($key->value);
        default:
          throw new Exception('Invalid Type: ' . $key->value['type']);
      }
    }
    else {
      self::convertSimpleXMLToArray($key);
    }
  }

  /**
   * Convert Mixed Form Control from XML to PHP
   * 
   * @param SimpleXMLFormElement $element
   * 
   * @return mixed
   */
  private static function convertSimpleXMLToMixed(SimpleXMLElement $element) {
    $type = $element['type'];
    switch ($type) {
      case 'string':
        return self::convertSimpleXMLToString($element);
      case 'integer':
        return self::convertSimpleXMLToInteger($element);
      case 'float':
        return self::convertSimpleXMLToFloat($element);
      case 'boolean':
        return self::convertSimpleXMLToBoolean($element);
      case 'array':
        return self::convertSimpleXMLToArray($element->array);
      default:
        throw new Exception('Invalid type: ' . $type);
    }
  }

}

