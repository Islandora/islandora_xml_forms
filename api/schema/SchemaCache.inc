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
class SchemaCache {
  /**
   * The Drupal cache id where the mappings of urls to actual files is stored.
   */
  const ID = 'xml-schema-table';
  const Type = 'cache';

  /**
   * Checks if the Cache exists.
   * 
   * @return boolean
   *   TRUE if the cache exists FALSE otherwise.
   */
  public static function Exists() {
    return cache_get(self::ID, self::Type) && self::LocationExists();
  }

  /**
   * Creates the cache, if it does not exist.
   * 
   * @return boolean
   *   TRUE if the cache was created or already exists, FALSE otherwise.
   */
  public static function Create() {
    if (!self::Exists()) {
      return self::Clear(); // Clear will initalize the table and files location.
    }
    return TRUE;
  }

  /**
   * Clears the Cache table and removes the Cached files.
   * 
   * @return boolean
   *   TRUE if the cache was successfully cleared or initialized.
   */
  public static function Clear() {
    self::ClearCacheTable(); // Assumed to always work.
    return self::ClearCachedFiles();
  }

  /**
   * Clears the Cache table.
   */
  private static function ClearCacheTable() {
    cache_set(self::ID, array(), self::Type, CACHE_PERMANENT); // Empty to start with.
  }

  /**
   * Removes the Cached files if they exists. Creates the cached file location if it doesn't exist.
   * 
   * @return boolean
   *   TRUE if the cached files were successfully cleared, or if the cached file location
   *   was successfully created.
   */
  private static function ClearCachedFiles() {
    if (self::LocationExists()) {
      $files = scandir(XML_SCHEMA_API_CACHE_LOCATION);
      foreach ($files as $file) {
        if ($file != "." && $file != "..") {
          $full_filename = XML_SCHEMA_API_CACHE_LOCATION . "/" . $file;
          if (filetype($full_filename) != "dir") {
            unlink($full_filename);
          }
        }
      }
      return TRUE;
    }
    return @mkdir(XML_SCHEMA_API_CACHE_LOCATION, 0775, FALSE);
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
    $filename = empty($filename) ? XML_SCHEMA_API_CACHE_LOCATION : $filename;
    return file_exists($filename);
  }

  /**
   * Get the cached document
   * 
   * @param string $filename
   *   The filename used by schemas internally to refer to specific files. May also be a url.
   * 
   * @return DOMDocument 
   *   The cached DOMDocument if found FALSE otherwise.
   */
  public static function Get($filename) {
    $cached_filename = self::GetCachedFilename($filename);
    if ($cached_filename !== FALSE) {
      $doc = new DOMDocument();
      if (!@$doc->load($cached_filename)) {
        // Failed to load clear instance from the cache.
        $cache = cache_get(self::ID, self::Type)->data;
        unset($cache[$filename]);
        cache_set(self::ID, $cache, self::Type);
        return FALSE;
      }
      return $doc; // Success.
    }
    return FALSE;
  }

  /**
   * Gets the cached document's filename
   * 
   * @param string $filename
   *   The filename used by schemas internally to refer to specific files. May also be a url.
   * 
   * @return string
   *   The location of the cached if found FALSE otherwise.
   */
  public static function GetCachedFilename($filename) {
    if (self::Create()) { // Only Creates if none existed.
      $cache = cache_get(self::ID, self::Type)->data;
      if (isset($filename) && isset($cache[$filename])) {
        if (self::LocationExists($cache[$filename])) {
          return $cache[$filename];
        }
        // Failed to find the file clear instance from the cache.
        unset($cache[$filename]);
        cache_set(self::ID, $cache, self::Type);
      }
    }
    return FALSE;
  }

  /**
   * Caches the given document with the given filename.
   *
   * @param string $filename
   *   The name of the schema file.
   * @param DOMDocument $doc 
   *   The schema file.
   */
  public static

  function Set($filename, DOMDocument $doc) {
    if (self::Create()) { // Only Creates if none existed.
      $cached_filename = file_create_filename('schema.xsd', XML_SCHEMA_API_CACHE_LOCATION);
      if (@$doc->save($cached_filename) != FALSE) {
        $cache = cache_get(self::ID, self::Type)->data;
        $cache[$filename] = $cached_filename;
        cache_set(self::ID, $cache, self::Type);
      }
    }
  }

}