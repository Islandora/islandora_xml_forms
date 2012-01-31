<?php

/**
 * @file
 * Handles the caching of schemas so they don't need to be downloaded each request.
 */
define('XML_SCHEMA_API_CACHE_LOCATION', file_directory_path() . '/schemas');

/**
 * A class used to store/retrieve schema documents from the files location on disk.
 * 
 * The mappings of url's to actual files is stored in the Drupal cache.
 */
class Cache {
  /**
   * The drupal cache id where the mappings of urls to actual files is stored.
   */
  const ID = 'xml-schema-table';

  /**
   * Creates the cache, if it does not exist.
   * 
   * @return boolean
   *   TRUE if the cache was created or already exists, FALSE otherwise.
   */
  public static function Create() {
    if (cache_get(self::ID, 'cache') == FALSE) {
      cache_set(self::ID, array(), 'cache', CACHE_PERMANENT); // Empty to start with.
    }
    if (!self::LocationExists()) { // Cache location.
      return @mkdir(XML_SCHEMA_API_CACHE_LOCATION, 775, TRUE);
    }
    return TRUE;
  }

  /**
   * Checks to see if the cache location exists.
   * 
   * @param string $filename
   *   The name of the cached file to retrieve.
   * 
   * @return boolean
   *   TRUE if the cach
   */
  public static function LocationExists($filename = NULL) {
    $filename = empty($filename) ? '' : '/' . $filename;
    return file_exists(XML_SCHEMA_API_CACHE_LOCATION . $filename);
  }

  /**
   *
   * @param string $url
   *   The filename/url used by schemas internally to refer to specific files.
   * @return DOMDocument 
   */
  public static function Get($url) {
    // @todo Get the filename from the cache.
    $filename = '';
    if (isset($filename) && self::LocationExists($filename)) {
      $doc = new DOMDocument();
      
      return $doc;
    }
    return FALSE;
  }

}