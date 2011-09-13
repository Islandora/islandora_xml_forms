<?php

// $Id$

/**
 * @file
 *
 * Provides a wrapper class around the database table were forms are stored.
 */
class XMLFormDatabase {
  /**
   * Constants
   */
  const table = 'xml_forms';

  /**
   * Checks to see if the given form exists in the database.
   * 
   * @param string $form_name
   *   The name of the XML Form Definition.
   * 
   * @return boolean
   *   TRUE if the given form exists, FALSE otherwise.
   */
  public static function Exists($form_name) {
    $query = db_query('SELECT count(name) FROM {%s} WHERE name = "%s"', self::table, $form_name);
    $count = db_result($query);
    return $count == 1;
  }

  /**
   * Gets the XML Form Definition identified by name.
   * 
   * @param string $form_name
   *   The name of the XML Form Definition.
   * 
   * @return DOMDocument 
   *   The XML Form Definition if found, FALSE otherwise.
   */
  public static function Get($form_name) {
    if (self::Exists($form_name)) {
      $query = db_query('SELECT form FROM {%s} WHERE name = "%s"', self::table, $form_name);
      $xml = db_result($query);
      $definition = new DOMDocument();
      $definition->loadXML($xml);
      return $definition;
    }
    return FALSE;
  }

  /**
   * Gets a list of all defined form names.
   * 
   * @return array
   *   An array of defined form names, where both the key and the value are the forms name.
   *   EX. array( 'name' => 'name' );
   */
  public static function GetNames() {
    $names = array();
    $result = db_query('SELECT name FROM {xml_forms}');
    while ($data = db_fetch_object($result)) {
      $names[$data->name] = $data->name;
    }
    return $names;
  }

  /**
   * Creates a form with the given form name and definition.
   * 
   * If the form already exists it will fail.
   * 
   * @param string $form_name
   *   The name of the XML Form Definition.
   * @param DOMDocument $definition 
   *   A XML Form Definition.
   * 
   * @return boolean
   *  TRUE if successful, otherwise FALSE.
   */
  public static function Create($form_name, DOMDocument $definition = NULL) {
    if (!self::Exists($form_name)) {
      $definition = isset($definition) ? $definition : xml_form_api_get_empty_form();
      $object = new stdClass();
      $object->name = $form_name;
      $object->form = $definition;
      return drupal_write_record(self::table, $object);
    }
    return FALSE;
  }

  /**
   * Updates the form with the given form name, the previous definition is lost.
   * 
   * If the form does not exist this function will fail.
   * 
   * @param string $form_name
   *   The name of the XML Form Definition.
   * @param DOMDocument $definition 
   *   A XML Form Definition.
   */
  public static function Update($form_name, DOMDocument $definition) {
    
  }

  /**
   * Deletes the form with the given form name.
   * 
   * @param string $form_name 
   *   The name of the XML Form Definition.
   */
  public static function Delete($form_name) {
    
  }

}