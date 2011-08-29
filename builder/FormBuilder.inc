<?php

// $Id$

/**
 * @file
 *
 * 
 */
module_load_include('inc', 'php_lib', 'Array');

class FormBuilder {
  /**
   * Permissions.
   */
  const PERM_LIST_FORMS = 'List Forms';
  const PERM_CREATE_FORMS = 'Create Forms';
  const PERM_EDIT_FORMS = 'Edit Forms';
  const PERM_DELETE_FORMS = 'Delete Forms';
  const BASE_PATH = 'admin/content/xml/form';

  /**
   * Get the path to the create form.
   * 
   * @return string
   */
  public static function CreatePath() {
    return self::BASE_PATH . '/create';
  }

  /**
   * Get the path to the create form.
   * 
   * @return string
   */
  public static function CopyPath($form_name) {
    return self::BASE_PATH . "/$form_name/copy";
  }

  /**
   * Get the path to the edit form.
   * 
   * @param string $form_name 
   * @return string
   */
  public static function EditPath($form_name) {
    return self::BASE_PATH . "/$form_name/edit";
  }

  /**
   * Get the path to view the form.
   *
   * @param string $form_name
   * @return string
   */
  public static function ViewPath($form_name) {
    return self::BASE_PATH . "/$form_name/view";
  }

  /**
   * Get the path to the edit form.
   * 
   * @param string $form_name 
   * @return string
   */
  public static function DeletePath($form_name) {
    return self::BASE_PATH . "/$form_name/delete";
  }

  /**
   * Get all existing form names.
   * 
   * @return array
   */
  public static function GetFormNames() {
    $output = array();
    $result = db_query('SELECT name FROM {xml_forms}');
    while ($data = db_fetch_object($result)) {
      $output[$data->name] = $data->name;
    }
    return $output;
  }

  /**
   * 
   * 
   * @param string $form_name
   * @return boolean
   */
  public static function HasFormDefinition($form_name) {
    if (self::FormExists($form_name)) {
      $xml_definition = db_result(db_query("SELECT form FROM {xml_forms} WHERE name = '%s'", $form_name));
      return $xml_definition != NULL;
    }
    return FALSE;
  }

  /**
   * Get the definition of the given form.
   * 
   * @param string $form_name 
   * @return mixed
   *   If the form exists its definition will be returned, this will be a xml string or NULL. FALSE is 
   *   returned if the form does not exist.
   */
  public static function GetFormDefinition($form_name) {
    if (self::FormExists($form_name)) {
      return db_result(db_query("SELECT form FROM {xml_forms} WHERE name = '%s'", $form_name));
    }
    return FALSE;
  }

  /**
   *
   * @param string $form_name
   * @param DOMDocument $definition 
   */
  public static function SetFormDefinition($form_name, DOMDocument $definition) {
    if (self::FormExists($form_name)) {
      return db_query("UPDATE {xml_forms} set form = '%s' WHERE name = '%s'", $definition->saveXML(), $form_name);
    }
    return FALSE;
  }

  /**
   *
   * @param string $form_name
   * @return boolean
   */
  public static function CreateForm($form_name) {
    if (!self::FormExists($form_name) && self::IsValidFormName($form_name)) {
      $object = new stdClass();
      $object->name = $form_name;
      return drupal_write_record('xml_forms', $object);
    }
    return FALSE;
  }

  /**
   *
   * @param string $original
   * @param string $form_name
   * @return boolean
   */
  public static function CopyForm($original, $form_name) {
    if (self::FormExists($original) && !self::FormExists($form_name) && self::IsValidFormName($form_name)) {
      $object = new stdClass();
      $object->name = $form_name;
      $object->form = self::GetFormDefinition($original);
      return drupal_write_record('xml_forms', $object);
    }
    return FALSE;
  }

  /**
   *
   * @param string $form_name
   * @return boolean
   */
  public static function DeleteForm($form_name) {
    if (self::FormExists($form_name)) {
      db_query("DELETE FROM {xml_forms} WHERE name = '%s'", $form_name);
      return TRUE;
    }
    return FALSE;
  }

  /**
   *
   * @param string $form_name
   * @return boolean
   */
  public static function FormExists($form_name) {
    $count = db_result(db_query("SELECT COUNT(name) FROM {xml_forms} WHERE name = '%s'", $form_name));
    return $count == 1;
  }

  /**
   *
   * @param string $form_name
   * @return boolean
   */
  public static function IsValidFormName($form_name) {
    $result = preg_match('/^[a-zA-Z][a-zA-Z0-9_]*$/', $form_name);
    return $result != FALSE && $result > 0;
  }

  /**
   * Gets a list of all allowed form elements.
   * 
   * @staticvar array $cache
   * @return array
   */
  public static function GetElementTypes() {
    static $cache;
    if (!isset($cache)) {
      $cache = array();
      foreach (module_implements('elements') as $module) {
        $elements = module_invoke($module, 'elements');
        if (isset($elements) && is_array($elements)) {
          $cache = array_merge_recursive($cache, $elements);
        }
      }
    }
    $types = array_keys($cache);
    $output = array();
    foreach ($types as $type) {
      $output[$type] = $type;
    }
    return $output;
  }

  public static function Export($form_name) {
    if (self::FormExists($form_name)) {
      header('Content-Type: text/xml', true);
      header('Content-Disposition: attachment; filename="' . $form_name . '.xml"');
      echo self::GetFormDefinition($form_name);
      exit();
    }
    else {
      drupal_not_found();
    }
  }

  public static function TransformPropertiesToJSON($form_name) {
    if (self::HasFormDefinition($form_name)) {
      module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
      $definition = self::GetFormDefinition($form_name);
      $properties = XMLFormDefinition::GetFormProperties($definition);
      $namespaces = isset($properties['namespaces']) ? $properties['namespaces'] : array();
      $properties = array(
        'localName' => $properties['root_name'],
        'uri' => $properties['default_uri'],
        'namespaces' => $namespaces,
        'schema' => $properties['schema_uri']
      );
      return json_encode(array($properties));
    }
    else {
      return json_encode(array('properties' => NULL));
    }
  }

  public static function TransformElementsToJSON($form_name) {
    if (self::HasFormDefinition($form_name)) {
      module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
      $root = array();
      if (self::HasFormDefinition($form_name)) {
        $xml = self::GetFormDefinition($form_name);
        if (isset($xml)) {
          $root = XMLFormDefinition::GetDrupalForm($xml);
        }
      }
      $elements = array(
        'key' => 'Root',
        'text' => 'Elements',
        'children' => array()
      );
      $form_element = new FormElement($root);
      $form_element['#type'] = 'form';
      //$elements['children'][] = self::TransformElement($form_element);
      $elements = self::TransformElement($form_element);
      $elements['key'] = 'Root';
      $elements['text'] = "Form ({$elements['type']})";
      return json_encode(array('expanded' => true, 'children' => $elements));
    }
    else {
      return json_encode(array());
    }
  }

  private static function TransformElement(FormElement $element) {
    $output = array();
    // Form Controls.
    foreach ($element->controls as $name => $control) {
      $name = str_replace('#', '', $name); // Remove the Hash.
      $value = $control;
      if ($value instanceof FormPropertyInterface) {
        $value = $value->toDrupalForm();
      }
      if (is_array($value)) {
        $value = self::TransformArray($value);
      }
      $output[$name] = $value;
    }
    // Children
    $output['children'] = array();
    foreach ($element->children as $child) {
      $output['children'][] = self::TransformElement($child);
    }
    $key = $element->getIndex();
    $type = $element['#type'];
    $output['key'] = $key;
    $output['text'] = "{$key} ({$type})";
    return $output;
  }

  private static function TransformArray(array $original) {
    $keys = array_keys($original);
    $numeric_keys = array_filter($keys, 'is_numeric');
    $array_is_numerically_keyed = (count($keys) == count($numeric_keys));
    if ($array_is_numerically_keyed) {
      return self::TransformArrayExtJSFriendlyArray($original);
    }
    else {
      return self::TransformArrayExtJSFriendlyObject($original);
    }
  }

  private static function TransformArrayExtJSFriendlyArray(array $original) {
    $output = array();
    foreach ($original as $key => $value) {
      $output[$key] = array($value);
    }
    return $output;
  }

  private static function TransformArrayExtJSFriendlyObject(array $original) {
    $output = array();
    foreach ($original as $key => $value) {
      $output[$key] = $value;
    }
    return $output;
  }

}
