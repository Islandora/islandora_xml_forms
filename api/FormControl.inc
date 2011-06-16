<?php

// $Id$

/**
 * @file
 *
 */

/**
 * 
 */
class XMLFormControl {

  /**
   *
   * @var string 
   */
  protected $_name;
  /**
   *
   * @var mixed 
   */
  public $value;

  /**
   *
   * @param type $xml_definition 
   */
  public static function createFromXMLDefiniton($xml_definition) {
    return self::createFromSimpleXML(simplexml_load_string($xml_definition));
  }

  /**
   *
   * @param type $element
   * @return FormControl 
   */
  public static function createFromSimpleXML(SimpleXMLElement $form_control) {
    list($name, $value) = self::getNameAndValueFromSimpleXML($form_control);
    return new FormControl("#$name", $value);
  }

  /**
   *
   * @param SimpleXMLElement $form_control 
   */
  public static function getNameAndValueFromSimpleXML(SimpleXMLElement $form_control) {
    $name = $form_control->getName();
    $value = self::getValueFromSimpleXML($name, $form_control);
    return array($name, $value);
  }

  /**
   *
   * @param type $name
   * @param SimpleXMLElement $form_control 
   */
  public static function getValueFromSimpleXML($name, SimpleXMLElement $form_control) {
    $function = self::getSimpleXMLConvertFunction($name);
    return call_user_func(array('FormControl', $function), $form_control);
  }

  /**
   * Get a map of form controls as keys, and thier types as values.
   * 
   * @return array
   */
  public static function getTypeMap() {
    static $type_map;
    if (empty($type_map)) {
      $filename = drupal_get_path('module', 'xml_form_api') . '/FormDefinition.xsd';
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
   * Get the type of value this form control has.
   * 
   * @param string $name
   * @return string
   */
  public static function getType($name) {
    $type_map = self::getTypeMap();
    return $type_map[$name];
  }

  /**
   *
   * @param type $name 
   */
  public static function getSimpleXMLConvertFunction($name) {
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
   *
   * @param SimpleXMLElement $element 
   */
  public static function convertSimpleXMLToString(SimpleXMLElement $element) {
    return (string) $element;
  }

  /**
   *
   * @param SimpleXMLElement $element 
   */
  public static function convertSimpleXMLToBoolean(SimpleXMLElement $element) {
    return strcasecmp((string) $element, 'true') === 0 ? true : false;
  }

  /**
   *
   * @param SimpleXMLElement $element 
   */
  public static function convertSimpleXMLToInteger(SimpleXMLElement $element) {
    return (int) $element;
  }

  /**
   *
   * @param SimpleXMLElement $element 
   */
  public static function convertSimpleXMLToFloat(SimpleXMLElement $element) {
    return (float) $element;
  }

  /**
   *
   * @param SimpleXMLElement $element 
   */
  public static function convertSimpleXMLToArray(SimpleXMLElement $element) {
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
   *
   * @param SimpleXMLElement $key
   * @return type 
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
   *
   * @param type $element 
   */
  public static function convertSimpleXMLToMixed(SimpleXMLElement $element) {
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

  /**
   *
   * @param SimpleXMLElement $element 
   */
  public static function convertSimpleXMLToUserData(SimpleXMLElement $element) {
    return module_invoke_all('convert_form_control_user_data');
  }

  /**
   *
   * @param string $name 
   * @return boolean
   */
  public static function validName($name) {
    return is_string($name) && strpos($name, '#') === 0;
  }

  /**
   * Returns a string description of the type of the parameter.
   * 
   * @param mixed $var
   * @return string
   */
  public static function determineMixedType($var) {
    if (is_string($var)) {
      return 'string';
    }
    if (is_int($var)) {
      return 'integer';
    }
    if (is_float($var)) {
      return 'float';
    }
    if (is_bool($var)) {
      return 'boolean';
    }
    if (is_array($var)) {
      return 'array';
    }
    throw new Exception('Invalid type.');
  }

  /**
   *
   * @param string $name
   * @param mixed $value 
   */
  public function __construct($name, $value) {
    if (self::validName($name)) {
      $this->_name = $name;
      $this->value = $value;
    }
    else {
      throw new Exception("Invalid Name: $name for FormControl.");
    }
  }

  /**
   * 
   */
  public function asNode(DOMDocument $document) {
    $name = str_replace('#', '', $this->_name); // Remove the Hash.
    $control = $document->createElement($name);
    switch (self::getType($name)) {
      case 'string':
      case 'integer':
      case 'float':
        $control->appendChild(new DOMText((string) $this->value));
        break;
      case 'boolean':
        $control->appendChild(new DOMText(($this->value) ? 'true' : 'false'));
        break;
      case 'array':
        $this->appendArray($document, $control, $this->value);
        break;
      case 'mixed':
        $this->appendMixed($document, $control, $this->value);
        break;
      default:
        throw new Exception('Invalid type: ' . $type);
    }
    return $control;
  }

  /**
   *
   * @param DOMElement $control
   * @param array $values 
   */
  private function appendArray(DOMDocument $document, DOMElement $control, array $values) {
    foreach ($values as $name => $item) {
      $key = $document->createElement('key');
      if (!is_numeric($name)) {
        $key->setAttribute('name', $name);
      }
      if (is_array($value)) {
        $this->appendArray($key, $item);
      }
      else if (isset($item)) {
        $value = $document->createElement('value', (string) $item);
        $value->setAttribute('type', self::determineMixedType($item));
        $key->appendChild($value);
      }
      else {
        continue;
      }
      $control->appendChild($key);
    }
  }

  /**
   *
   * @param DOMElement $control
   * @param mixed $value 
   */
  private function appendMixed(DOMDocument $document, DOMElement $control, $value) {
    $type = self::determineMixedType($value);
    $control->setAttribute('type', $type);
    switch ($type) {
      case 'string':
      case 'integer':
      case 'float':
        $control->appendChild(new DOMText((string) $value));
        break;
      case 'boolean':
        $control->appendChild(new DOMText(($value) ? 'true' : 'false'));
        break;
      case 'array':
        $array = $document->createElement('array');
        $this->appendArray($array, $value);
        $control->appendChild($array);
        break;
    }
  }

}

/**
 * 
 */
class FormControls extends ArrayObject {

  /**
   *
   * @param type $xml_definition 
   */
  public static function createFromXMLDefiniton($xml_definition) {
    return self::createFromSimpleXML(simplexml_load_string($xml_definition));
  }

  /**
   *
   * @param type $element
   * @return FormControl 
   */
  public static function createFromSimpleXML($element) {
    $form_controls = new FormControls();
    $controls = $element->xpath("child::*[local-name() != 'children' and local-name() != 'actions']");
    foreach ($controls as $name => $control) {
      $form_controls['#' . $control->getName()] = FormControl::createFromSimpleXML($control);
    }
    return $form_controls;
  }

  /**
   * 
   */
  public function toDrupalForm() {
    $output = null;
    foreach ($this as $key => $control) {
      $output[$key] = $control->value;
    }
    return $output;
  }

  /**
   *
   * @param mixed $offset
   * @return mixed 
   */
  public function offsetGet($offset) {
    if (parent::offsetExists($offset)) {
      $value = parent::offsetGet($offset);
    }
    return isset($value) ? $value->value : NULL;
  }

  /**
   *
   * @param mixed $offset
   * @param mixed $value 
   */
  public function offsetSet($offset, $value) {
    $value_is_form_control = isset($value) && is_object($value) && get_class($value) == 'FormControl';
    $value = $value_is_form_control ? $value : new FormControl($offset, $value);
    parent::offsetSet($offset, $value);
  }

  /**
   *
   * @param type $name
   * @param type $value 
   */
  public function __set($name, $value) {
    $this->offsetSet("#$name", $value);
  }

  /**
   *
   * @param type $name
   * @return type 
   */
  public function __get($name) {
    $name = "#$name";
    if ($this->offsetExists($name)) {
      return $this->offsetGet($name);
    }
    return NULL;
  }

}