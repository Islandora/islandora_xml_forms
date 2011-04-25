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
class FCType extends SplEnum {
  const __default = 0;
  /* Common form control types */
  const FC_String = 0; // Default Form Control Type.
  const FC_Boolean = 1;
  const FC_Integer = 2;
  const FC_Array = 3;
  const FC_XMLForm = 4;
  /* Special processing (Form Control Specific)  */
  const FC_Ahah = 5;
  const FC_Redirect = 6;
}

class FormControlGeneratorFunctions {
  /**
   *
   * @var array 
   */
  static private $formControlToTypeMap = array(
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
    /* XML Form API specific Form Controls */
    'xml_form' => FCType::FC_XMLForm
  );
  
  /**
   *
   * @var type 
   */
  static private $typeToGenerateFunctionMap = array(
    FCType::FC_String => self::defaultGenerateFunction,
    FCType::FC_Boolean => self::defaultGenerateFunction,
    FCType::FC_Integer => self::defaultGenerateFunction,
    FCType::FC_Array => self::arrayGenerateFunction,
    FCType::FC_XMLForm => self::XMLFormGenerateFunction,
    FCType::FC_Ahah => self::ahahGenerateFunction,
    FCType::FC_Redirect => self::redirectGenerateFunction,
  );

  /**
   *
   * @param type $name 
   * @return FormControlType
   */
  public static function getFormControlType(&$name) {
    if (array_key_exists($name, self::$formControlToTypeMap)) {
      return self::$formControlToTypeMap[$name];
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
  public static function getGeneratorFunctionFromName(&$name) {
    $type = self::getFormControlType($name);
    return self::getFormControlGeneratorFunction($name);
  }

  /**
   *
   * @param FCType $type 
   * @return string
   *   Function name of the generator function for the requested type.
   */
  public static function getGeneratorFunctionFromType(FCType $type) {
    return self::$typeToGenerateFunctionMap[$type];
  }

  /**
   * @param DOMElement $form_control
   * @return array
   */
  public static function defaultGenerateFunction(DOMElement $form_control) {
    return $form_control->textContent;
  }

}