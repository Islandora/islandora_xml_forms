<?php

// $Id$

/**
 * @file
 *
 */

/**
 * Builds a form from a JSON object posted to the server by the form builder.
 */
class JSONForm {

  /**
   *
   * @var stdClass
   */
  protected $json;
  protected $documentProperties;
  protected $properties;
  protected $elements;
  protected $definition;

  /**
   * Create a FormDefintion from Posted JSON data.
   */
  public function __construct($form_name) {
    $this->json = json_decode($_POST['data']);
    $this->properties = $this->createFormProperties();
    $this->elements = $this->createFormElements();
    $this->definition = $this->createFormDefinition($this->properties, $this->elements);
  }

  /**
   *
   * @return FormProperties 
   */
  private function createFormProperties() {
    module_load_include('inc', 'xml_form_api', 'FormProperties');
    $this->documentProperties = $this->createDocumentProperties();
    return new FormProperties($this->documentProperties);
  }

  /**
   *
   * @return DocumentProperties 
   */
  private function createDocumentProperties() {
    module_load_include('inc', 'xml_form_api', 'DocumentProperties');
    $properties = $this->json->properties;
    $prefix = null; //$properties->prefix;
    $name = "mods"; //$properties->name;
    $uri = null; //$properties->uri;
    $schema = null; //$properties->schema;
    $namespaces = array(); //$properties->namespaces;
    return new DocumentProperties($prefix, $name, $uri, $schema, $namespaces);
  }

  /**
   *
   * @return FormElements
   */
  private function createFormElements() {
    $elements = $this->createElements($this->json->elements);
    return new FormElements(Form::ROOT, $elements);
  }

  /**
   * 
   * @return array
   */
  private function createElements(array $elements) {
    $output = array();
    foreach ($elements as $element) {
      $element = $this->createElement($element);
      $output[$element->name] = $element;
    }
    return $output;
  }

  /**
   *
   * @param stdClass $element
   * @return FormElement
   */
  private function createElement($element) {
    $key = isset($element->key) ? $element->key : NULL;
    $controls = $this->createFormControls($element);
    $actions = $this->createActions($element);
    $children = $this->createElements($element->elements);
    $element = new FormElement();
    $element->name = (isset($key) && $key != "") ? $key : NULL;
    $element->controls = $controls;
    $element->actions = $actions;
    foreach ($children as $child) {
      $element[$child->name] = $child;
    }
    return $element;
  }

  /**
   *
   * @param stdClass $element 
   */
  private function createFormControls($element) {
    $controls = array(
      'type' => array(),
      'access' => array(),
      'after_build' => array('convert' => 'convertFormControlToArray', 'validate' => 'arrayIsNotEmpty'),
      'ahah' => array('convert' => 'convertAhah', 'validate' => 'arrayIsNotEmpty'),
      'attributes' => array('convert' => 'convertFormControlToMap', 'validate' => 'arrayIsNotEmpty'),
      'autocomplete_path' => array('validate' => 'stringIsNotEmpty'),
      'button_type' => array('validate' => 'stringIsNotEmpty'),
      'collapsed' => array(),
      'collapsible' => array(),
      'cols' => array(),
      'default_value' => array('validate' => 'stringIsNotEmpty'),
      'delta' => array(),
      'description' => array('validate' => 'stringIsNotEmpty'),
      'disabled' => array(),
      'element_validate' => array('convert' => 'convertFormControlToArray', 'validate' => 'arrayIsNotEmpty'),
      'executes_submit_callback' => array(),
      'field_prefix' => array('validate' => 'stringIsNotEmpty'),
      'field_suffix' => array('validate' => 'stringIsNotEmpty'),
      'maxlength' => array(),
      'method' => array('validate' => 'stringIsNotEmpty'),
      'multiple' => array(),
      'name' => array('validate' => 'stringIsNotEmpty'),
      'options' => array('convert' => 'convertFormControlToMap', 'validate' => 'arrayIsNotEmpty'),
      'post_render' => array('convert' => 'convertFormControlToArray', 'validate' => 'arrayIsNotEmpty'),
      'prefix' => array('validate' => 'stringIsNotEmpty'),
      'pre_render' => array('convert' => 'convertFormControlToArray', 'validate' => 'arrayIsNotEmpty'),
      'process' => array('convert' => 'convertFormControlToArray', 'validate' => 'arrayIsNotEmpty'),
      'required' => array(),
      'resizable' => array(),
      'return_value' => array('validate' => 'stringIsNotEmpty'),
      'rows' => array(),
      'size' => array(),
      'src' => array('validate' => 'stringIsNotEmpty'),
      'submit' => array('convert' => 'convertFormControlToArray', 'validate' => 'arrayIsNotEmpty'),
      'suffix' => array('validate' => 'stringIsNotEmpty'),
      'theme' => array('validate' => 'stringIsNotEmpty'),
      'title' => array('validate' => 'stringIsNotEmpty'),
      'tree' => array(),
      'validate' => array('convert' => 'convertFormControlToArray', 'validate' => 'arrayIsNotEmpty'),
      'value' => array('validate' => 'stringIsNotEmpty'),
      'weight' => array(),
      'user_data' => array('convert' => 'convertFormControlToMap', 'validate' => 'arrayIsNotEmpty'),
    );
    module_load_include('inc', 'xml_form_api', 'FormControl');
    $form_controls = new FormControls();
    foreach ($controls as $control => $action) {
      if (isset($element->$control)) {
        $name = "#$control";
        $value = isset($action['convert']) ? call_user_func(array($this, $action['convert']), $element->$control) : $element->$control;
        if (empty($action['validate']) || call_user_func(array($this, $action['validate']), $value)) {
          $form_controls[$name] = new FormControl($name, $value);
        }
      }
    }
    return $form_controls;
  }

  /**
   * Extjs treats arrays kinda funky so we need to normalize them.
   * 
   * @param array $control 
   */
  private function convertFormControlToArray(array $control) {
    $output = array();
    foreach ($control as $item) {
      $output[] = $item[0];
    }
    return $output;
  }

  /**
   * Extjs treats arrays kinda funky so we need to normalize them.
   * 
   * @param array $control 
   */
  private function convertFormControlToMap(array $control) {
    $output = array();
    foreach ($control as $item) {
      $output[] = $item[0];
    }
    return $output;
  }

  /**
   *
   * @param mixed $ahah 
   * @return array
   */
  private function convertAhah($ahah) {
    if (is_string($ahah)) {
      return array();
    }
    else {
      $output = array(
        'wrapper' => isset($ahah->wrapper) ? $ahah->wrapper : NULL,
        'effect' => isset($ahah->effect) ? $ahah->effect : NULL,
        'method' => isset($ahah->method) ? $ahah->method : NULL,
        'path' => isset($ahah->path) ? $ahah->path : NULL,
        'keypress' => isset($ahah->keypress) ? $ahah->keypress : NULL,
        'progress' => empty($ahah->progress) ? NULL : array(
          'type' => isset($ahah->progress->type) ? $ahah->progress->type : NULL,
          'message' => isset($ahah->progress->message) ? $ahah->progress->message : NULL,
          'url' => isset($ahah->progress->url) ? $ahah->progress->url : NULL,
          'interval' => isset($ahah->progress->interval) ? $ahah->progress->interval : NULL)
      );
      return $output;
    }
  }

  /**
   *
   * @param array $value
   * @return type 
   */
  private function arrayIsNotEmpty(array $value) {
    return count($value) > 0;
  }

  /**
   *
   * @param type $value
   * @return type 
   */
  private function stringIsNotEmpty($value) {
    return strlen($value) > 0;
  }

  /**
   *
   * @param stdClass $element 
   * @return Actions
   */
  private function createActions($element) {
    return NULL;
  }

  /**
   *
   * @param FormProperties $properties
   * @param FormElements $elements 
   */
  private function createFormDefinition(FormProperties $properties, FormElements $elements) {
    return new FormDefinition($properties, $elements);
  }

  /**
   * Get the form definition ready for storage.
   * 
   * @return DOMDocument 
   */
  public function getFormDefinition() {
    $doc = new DOMDocument();
    $doc->loadXML($this->definition->asXML());
    return $doc;
  }

}