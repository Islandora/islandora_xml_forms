<?php

// $Id$

/**
 * @file
 *
 */

/**
 * 
 */
class FormControl {

  /**
   * 
   * @var array
   */
  protected static $convertSimpleXMLFunctions;
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
   *
   * @param type $name 
   */
  public static function getSimpleXMLConvertFunction($name) {
    if (empty(self::$convertSimpleXMLFunctions)) {
      $type_function_map = array(
        'string' => 'convertSimpleXMLToString',
        'boolean' => 'convertSimpleXMLToBoolean',
        'integer' => 'convertSimpleXMLToInteger',
        'float' => 'convertSimpleXMLToFloat',
        'array' => 'convertSimpleXMLToArray',
        'mixed' => 'convertSimpleXMLToMixed',
      );
      $filename = drupal_get_path('module', 'xml_form_api') . '/FormDefinition.xsd';
      $form_definition = simplexml_load_file($filename);
      $filter_types = "@type='string' or @type = 'boolean' or @type = 'integer' or @type = 'float' or @type = 'array' or @type = 'mixed'";
      $form_controls = $form_definition->xpath("/xs:schema/xs:complexType/xs:all/xs:element[$filter_types]");
      foreach ($form_controls as $form_control) {
        self::$convertSimpleXMLFunctions[(string) $form_control['name']] = $type_function_map[(string) $form_control['type']];
      }
    }
    return self::$convertSimpleXMLFunctions[$name];
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
    $keys = $element->xpath('key');
    foreach ($keys as $key) {
      if (($name = $key['name']) != null) {
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
          break;
        case 'integer':
          return self::convertSimpleXMLToInteger($key->value);
          break;
        case 'boolean':
          return self::convertSimpleXMLToBoolean($key->value);
          break;
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
        break;
      case 'integer':
        return self::convertSimpleXMLToInteger($element);
        break;
      case 'float':
        return self::convertSimpleXMLToFloat($element);
        break;
      case 'boolean':
        return self::convertSimpleXMLToBoolean($element);
        break;
      case 'array':
        return self::convertSimpleXMLToArray($element->array);
        break;
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
   */
  public function asXML() {
    $xml = '';
    foreach ($this as $control) {
      $xml .= $control->asXML() . "/n";
    }
    return $xml;
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