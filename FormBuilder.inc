<?php

// $Id$

/**
 * @file
 *
 * 
 */
class FormBuilder {
  /**
   * Permissions.
   */
  const PERM_LIST_FORMS = 'List Forms';
  const PERM_CREATE_FORMS = 'Create Forms';
  const PERM_EDIT_FORMS = 'Edit Forms';
  const PERM_DELETE_FORMS = 'Delete Forms';

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

}
