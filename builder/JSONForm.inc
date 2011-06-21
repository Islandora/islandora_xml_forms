<?php

// $Id$

/**
 * @file
 *
 */
module_load_include('inc', 'php_lib', 'String');
module_load_include('inc', 'php_lib', 'Array');
module_load_include('inc', 'php_lib', 'Misc');
module_load_include('inc', 'xml_form_api', 'Actions');

/**
 * Builds a form from a JSON object posted to the server by the form builder.
 */
class JSONForm {

  /**
   *
   * @var stdClass
   */
  protected $json;
  protected $definition;

  /**
   * Create a FormDefintion from Posted JSON data.
   */
  public function __construct($form_name) {
    module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
    $this->json = json_decode($_POST['data']);
    $properties = $this->createProperties();
    $form = $this->createDrupalForm();
    $definition = XMLFormDefinition::Create($properties, $form['Root']);
    $this->definition = new DOMDocument();
    $this->definition->loadXML($definition->asXML());
    echo  $this->definition->saveXML();
  }

  /**
   *
   * @return FormProperties 
   */
  private function createProperties() {
    $properties = $this->json->properties;
    $namespaces = $this->convertStdClassToArray($properties->namespaces);
    return array(
      'document' => array(
        'root' => array(
          'prefix' => $properties->prefix,
          'local_name' => $properties->name,
          'uri' => $properties->uri
        ),
        'namespaces' => $namespaces,
        'schema' => $properties->schema
    ));
  }

  /**
   *
   * @return FormElements
   */
  private function createDrupalForm() {
    $elements = $this->json->elements;
    $output = array();
    foreach ($elements as $element) {
      list($key, $value) = $this->createDrupalFormElement($element);
      array_add($output, $key, $value);
    }
    return $output;
  }

  /**
   * 
   */
  private function createDrupalFormElement($element) {
    $key = isset($element->key) ? $element->key : NULL;
    $controls = $this->getDrupalFormControls($element);
    $children = $this->getDrupalFormChildElements($element);
    return array($key, array_merge($controls, $children));
  }

  /**
   *
   * @param array $element
   * @return array
   */
  private function getDrupalFormControls($element) {
    $ignore = array('key', 'text', 'root', 'leaf', 'id', 'parentId', 'icon', 'cls', 'qtip', 'qtitle',
      'iconCls', 'href', 'hrefTarget', 'elements', 'index', 'depth', 'expanded', 'checked',
      'isLast', 'isFirst', 'allowDrop', 'allowDrag', 'loaded', 'loading');
    $controls = array();
    foreach ($element as $key => $value) {
      $is_control = array_search($key, $ignore) === FALSE;
      $is_valid = $this->validateFormControl($key, $value);
      $value = is_object($value) ? object_as_array($value) : $value;
      if ($is_control && $is_valid) {
        $controls["#$key"] = $value;
      }
    }
    return $controls;
  }

  /**
   *
   * @param array $element
   * @return array 
   */
  private function getDrupalFormChildElements($element) {
    $children = array();
    if (isset($element->elements)) {
      foreach ($element->elements as $child) {
        list($key, $value) = $this->createDrupalFormElement($child);
        array_add($children, $key, $value);
      }
    }
    return $children;
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

    exit();
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
   * @param string $name
   * @param mixed $value
   * @return boolean 
   */
  private function validateFormControl($name, $value) {
    $controls = array(
      'type' => 'is_non_empty_string',
      'access' => 'is_bool',
      'after_build' => 'is_non_empty_array',
      'ahah' => 'is_non_empty_array',
      'attributes' => 'is_non_empty_array',
      'autocomplete_path' => 'is_non_empty_string',
      'button_type' => 'is_non_empty_string',
      'collapsed' => 'is_bool',
      'collapsible' => 'is_bool',
      'cols' => 'is_numeric',
      'default_value' => 'is_non_empty_string',
      'delta' => 'is_numeric',
      'description' => 'is_non_empty_string',
      'disabled' => 'is_bool',
      'element_validate' => 'is_non_empty_array',
      'executes_submit_callback' => 'is_bool',
      'field_prefix' => 'is_non_empty_string',
      'field_suffix' => 'is_non_empty_string',
      'maxlength' => 'is_numeric',
      'method' => 'is_non_empty_string',
      'multiple' => 'is_bool',
      'name' => 'is_non_empty_string',
      'options' => 'is_non_empty_array',
      'post_render' => 'is_non_empty_array',
      'prefix' => 'is_non_empty_string',
      'pre_render' => 'is_non_empty_array',
      'process' => 'is_non_empty_array',
      'required' => 'is_bool',
      'resizable' => 'is_bool',
      'return_value' => 'is_non_empty_string',
      'rows' => 'is_numeric',
      'size' => 'is_numeric',
      'src' => 'is_non_empty_string',
      'submit' => 'is_non_empty_array',
      'suffix' => 'is_non_empty_string',
      'theme' => 'is_non_empty_string',
      'title' => 'is_non_empty_string',
      'tree' => 'is_bool',
      'validate' => 'is_non_empty_array',
      'value' => 'is_non_empty_string',
      'weight' => 'is_numeric',
      'user_data' => 'is_non_empty_array',
      'actions' => 'is_non_empty_array',
    );
    $value = is_object($value) ? object_as_array($value) : $value;
    if (isset($controls[$name])) {
      return call_user_func($controls[$name], $value);
    }
    return TRUE; // Assume valid for non specified controls.
  }

  function convertStdClassToArray($object) {
    $array = array();
    if (is_object($object) || is_array($object)) {
      foreach ($object as $key => $value) {
        $array[$key] = is_object($value) ? $this->convertStdClassToArray($value) : $value;
      }
      return $array;
    }
    return $object;
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
   * Get the form definition ready for storage.
   * 
   * @return DOMDocument 
   */
  public function getFormDefinition() {
    return $this->definition;
  }

}