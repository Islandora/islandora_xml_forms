<?php

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
    $query = db_query("SELECT count(name) FROM {%s} WHERE name = '%s'", self::table, $form_name);
    $count = db_result($query);
    return $count == 1;
  }

  /**
   * Checks to see if the given form exists in the database and is a valid definition.
   * 
   * @param string $form_name
   *   The name of the XML Form Definition.
   * 
   * @return boolean
   *   TRUE if the given form exists, FALSE otherwise.
   */
  public static function Valid($form_name) {
    if (self::Exists($form_name)) {
      return self::Get($form_name) !== FALSE;
    }
    return FALSE;
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
      $query = db_query("SELECT form FROM {%s} WHERE name = '%s'", self::table, $form_name);
      $xml = db_result($query);
      if (trim($xml) !== '') {
        $definition = new DOMDocument();
        $definition->loadXML($xml);
        $definition = self::Upgrade($form_name, $definition);
        return $definition;
      }
    }
    return FALSE;
  }

  /**
   * Upgrade to the latest version and save it back to the database.
   * 
   * @param DOMDocument $definition 
   */
  private static function Upgrade($form_name, DOMDocument $definition) {
    module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
    $version = XMLFormDefinition::getVersion($definition);
    if (!$version->isLatestVersion()) {
      $definition = XMLFormDefinition::upgradeToLatestVersion($definition);
      self::Update($form_name, $definition); // Updates to latest.
    }
    return $definition;
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
    $result = db_query('SELECT name FROM {%s}', self::table);
    while ($data = db_fetch_object($result)) {
      $names[$data->name] = $data->name;
    }
    return $names;
  }

  /**
   * Gets a list of all defined form names that have valid definitions.
   * 
   * @return array
   *   An array of defined form names, where both the key and the value are the forms name.
   *   EX. array( 'name' => 'name' );
   */
  public static function GetValidNames() {
    $form_names = self::GetNames();
    $valid_names = array();
    foreach ($form_names as $key => $form_name) {
      if (self::Valid($form_name)) {
        $valid_names[$key] = $form_name;
      }
    }
    return $valid_names;
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
      module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
      $definition = isset($definition) ? $definition : xml_form_api_get_empty_form_definition();
      $definition = XMLFormDefinition::upgradeToLatestVersion($definition);
      $object = new stdClass();
      $object->name = $form_name;
      $object->form = $definition->saveXML();
      return drupal_write_record(self::table, $object) !== FALSE;
    }
    return FALSE;
  }

  /**
   * Copies an existing form.
   * 
   * @param string $form_name_src
   *   The name of the source form to copy from.
   * @param string $form_name_dest 
   *   The name of the destination form which gets copied to.
   * 
   * @return boolean
   *   TRUE if successful FALSE otherwise.
   */
  public static function Copy($form_name_src, $form_name_dest) {
    if (self::Exists($form_name_src)) {
      $definition = self::Get($form_name_src);
      return self::Create($form_name_dest, $definition);
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
    if (self::Exists($form_name)) {
      module_load_include('inc', 'xml_form_api', 'XMLFormDefinition');
      $version = XMLFormDefinition::getVersion($definition);
      if (!$version->isLatestVersion()) {
        $definition = XMLFormDefinition::upgradeToLatestVersion($definition);
      }
      db_query("UPDATE {%s} set form = '%s' WHERE name = '%s'", self::table, $definition->saveXML(), $form_name);
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Deletes the form with the given form name.
   * 
   * @param string $form_name 
   *   The name of the XML Form Definition.
   * 
   * @return boolean
   *   TRUE if successful FALSE otherwise.
   */
  public static function Delete($form_name) {
    if (self::Exists($form_name)) {
      db_query("DELETE FROM {%s} WHERE name = '%s'", self::table, $form_name);
      return TRUE;
    }
    return FALSE;
  }

}