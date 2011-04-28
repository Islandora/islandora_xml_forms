<?php

// $Id$

/**
 * @file
 *
 * This file contains a collection of functions that map xml form control definitions to thier array based
 * Drupal Form API equivalents.
 * 
 */

/**
 * Form Control Type
 */
class FCType {
  const __default = 0; // Enum supported with pecl... Need to extend SplEnum
  /* Common form control types */
  const FC_String = 0; // Default Form Control Type.
  const FC_Boolean = 1;
  const FC_Integer = 2;
  const FC_Array = 3;
  /* Special processing (Form Control Specific)  */
  const FC_Ahah = 4;
  const FC_Redirect = 5;
}

class FormControlGeneratorFunctions {

  /**
   *
   * @var array 
   */
  public static $form_control_type_map = array(
    /* Drupal API Form Controls */
    'access' => FCType::FC_Boolean,
    'action' => FCType::FC_String,
    'after_build' => FCType::FC_Array,
    'ahah' => FCType::FC_Ahah,
    'attributes' => FCType::FC_Array,
    'autocomplete_path' => FCType::FC_String,
    'button_type' => FCType::FC_String,
    'collapsed' => FCType::FC_Boolean,
    'collapsible' => FCType::FC_Boolean,
    'cols' => FCType::FC_Integer,
    'default_value' => FCType::FC_String,
    'delta' => FCType::FC_Integer,
    'description' => FCType::FC_String,
    'disabled' => FCType::FC_Boolean,
    'element_validate' => FCType::FC_Array,
    'executes_submit_callback' => FCType::FC_Boolean,
    'field_prefix' => FCType::FC_String,
    'field_suffix' => FCType::FC_String,
    'maxlength' => FCType::FC_Integer,
    'method' => FCType::FC_String,
    'multiple' => FCType::FC_Boolean,
    'name' => FCType::FC_String,
    'options' => FCType::FC_Array,
    'post_render' => FCType::FC_Array,
    'prefix' => FCType::FC_String,
    'pre_render' => FCType::FC_Array,
    'process' => FCType::FC_Array,
    'redirect' => FCType::FC_Redirect,
    'required' => FCType::FC_Boolean,
    'resizable' => FCType::FC_Boolean,
    'return_value' => FCType::FC_String,
    'rows' => FCType::FC_Integer,
    'size' => FCType::FC_Integer,
    'src' => FCType::FC_String,
    'submit' => FCType::FC_Array,
    'suffix' => FCType::FC_String,
    'theme' => FCType::FC_String,
    'title' => FCType::FC_String,
    'tree' => FCType::FC_Boolean,
    'type' => FCType::FC_String,
    'validate' => FCType::FC_Array,
    'value' => FCType::FC_String,
    'weight' => FCType::FC_Integer,
  );

  /**
   * @param DOMElement $form_control
   * @return string
   */
  public static function stringGenerateFunction(DOMElement $form_control) {
    return $form_control->textContent;
  }

  /**
   *
   * @param DOMElement $form_control
   * @return boolean
   */
  public static function booleanGenerateFunction(DOMElement $form_control) {
    $value = $form_control->textContent;
    if (strcasecmp($value, 'true') === 0) {
      return true;
    }
    else {
      return false;
    }
  }

  public static function integerGenerateFunction(DOMElement $form_control) {
    return (int) $form_control->textContent;
  }

  public static function arrayGenerateFunction(DOMElement $form_control) {
    $keys = $form_control->getElementsByTagName('key');
    return self::generateArrayFromKeys($keys);
  }

  private static function generateArrayFromKeys(DOMNodeList $keys) {
    $output = array();
    $count = $keys->length;
    for ($i = 0; $i < $count; $i++) {
      $key = $keys->item($i);
      list($name, $value) = self::processKey($key);
      if ($name != '') {
        $output[$name] = $value;
      }
      else {
        $output[] = $value;
      }
    }
    return $output;
  }

  private static function processKey(DOMElement $key) {
    $keys = $key->getElementsByTagName('key');
    if ($keys->length > 0) {
      $value = self::generateArrayFromKeys($keys);
    }
    else {
      $value = $key->getElementsByTagName('value')->item(0)->textContent;
    }
    $name = $key->getAttribute('name');
    return array($name, $value);
  }

  public static function ahahGenerateFunction(DOMElement $form_control) {
    $output = array();
    $element = simplexml_import_dom($form_control);
    if (isset($element->effect)) {
      $output['effect'] = (string) $element->effect;
    }
    if (isset($element->event)) {
      $output['event'] = (string) $element->event;
    }
    if (isset($element->keypress)) {
      $output['keypress'] = strcasecmp((string) $element->keypress, 'true') == 0;
    }
    if (isset($element->method)) {
      $output['method'] = (string) $element->method;
    }
    if (isset($element->path)) {
      $output['path'] = (string) $element->path;
    }
    if (isset($element->wrapper)) {
      $output['wrapper'] = (string) $element->wrapper;
    }
    if (isset($element->progress)) {
      $progress = $element->progress;
      $output['progress'] = array();
      $output['progress']['type'] = isset($progress->type) ? (string) $progress->type : null;
      $output['progress']['message'] = isset($progress->message) ? (string) $progress->message : null;
      $output['progress']['url'] = isset($progress->url) ? (string) $progress->url : null;
      $output['progress']['interval'] = isset($progress->interval) ? (string) $progress->interval : null;
    }
    return $output;
  }

  public static function redirectGenerateFunction(DOMElement $form_control) {
    $element = simplexml_import_dom($form_control);
    if (isset($element->false)) {
      return FALSE;
    }
    else if (isset($element->internal_path)) {
      return (string) $element->internal_path;
    }
    else {
      $xpath = new DOMXPath($form_control->ownerDocument);
      $keys = $xpath->query('array/child::key');
      return self::generateArrayFromKeys($keys);
    }
  }

  /**
   *
   * @var type 
   */
  public static $type_generate_function_map = array(
    FCType::FC_String => 'stringGenerateFunction',
    FCType::FC_Boolean => 'booleanGenerateFunction',
    FCType::FC_Integer => 'integerGenerateFunction',
    FCType::FC_Array => 'arrayGenerateFunction',
    FCType::FC_Ahah => 'ahahGenerateFunction',
    FCType::FC_Redirect => 'redirectGenerateFunction'
  );

  /**
   *
   * @param string $name 
   * @return FormControlType
   */
  public static function getFormControlType($name) {
    if (array_key_exists($name, self::$form_control_type_map)) {
      return self::$form_control_type_map[$name];
    }
    else {
      throw new Exception('Form Control ' . $name . ' does not exist.');
    }
  }

  /**
   *
   * @param string $name 
   *   The name of the form control.
   * @return string
   *   Function name of the generator function associated with the form control defined by $name
   */
  public static function getFunctionFromName($name) {
    $type = self::getFormControlType($name);
    return self::getFunctionFromType($type);
  }

  /**
   *
   * @param FCType $type 
   * @return string
   *   Function name of the generator function for the requested type.
   */
  public static function getFunctionFromType($type) {
    return self::$type_generate_function_map[$type];
  }

}