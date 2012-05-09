<?php

/**
 * @file
 */
module_load_include('inc', 'php_lib', 'ReadOnlyProtectedMembers');
module_load_include('inc', 'php_lib', 'DOMHelpers');
module_load_include('inc', 'php_lib', 'String');
module_load_include('inc', 'xml_form_api', 'SchemaTag');
module_load_include('inc', 'xml_schema_api', 'SchemaCache');

/**
 * Ignoring imported documents for the most part as they are not used much in FGDC this will be an issue we will need
 * to address in the future.
 */
class XMLSchema {
  /**
   * Constants
   */
  const prefix = 'xs';
  const uri = 'http://www.w3.org/2001/XMLSchema';

  /**
   *
   * @var ReadOnlyProtected
   */
  protected $protected;

  /**
   *
   * @var DOMDocument
   */
  protected $schema;

  /**
   *
   * @var DOMXPath
   */
  protected $xpath;

  /**
   *
   * @var Schema[]
   */
  protected $includes; // Array of include Schemas
  /**
   *
   * @var Schema[]
   */
  protected $imports; // Array of imported Schemas
  /**
   * An array of all created schemas identified by file name.
   * 
   * @var Schema[]
   */
  protected static $schemas;

  /**
   *
   * @param string $filename
   * @param string $namespace 
   */
  public function __construct($filename, $namespace = NULL) {
    $filename = trim($filename);
    self::$schemas[$filename] = $this;
    $this->protected = new ReadOnlyProtectedMembers(array(
          'filename' => $filename,
          'location' => $this->getSchemaLocation($filename),
          'namespace' => $namespace));
    $this->loadSchema($filename);
  }

  /**
   * Get a list of members to serialize.
   * 
   * @return array
   */
  public function __sleep() {
    return array('protected', 'includes', 'imports');
  }

  /**
   * Reinitialize this Object, when unserialized.
   */
  public function __wakeup() {
    $this->loadSchema($this->filename);
  }

  /**
   * Get a read only protected variable.
   * 
   * @param string $name
   * @return mixed
   */
  public function __get($name) {
    return $this->protected->$name;
  }

  /**
   * Set the value of a protected member.
   * 
   * @param string $name
   * @param mixed $value 
   */
  public function __set($name, $value) {
    $this->protected->$name = $value;
  }

  /**
   *
   * @param string $filename
   * @return string
   */
  private function getSchemaLocation($filename) {
    $index = strrpos($filename, '/');
    return substr($filename, 0, $index);
  }

  /**
   * Gets the full path to a schema maybe...
   * 
   * @param string $filename
   */
  private function getFullPathToSchema($filename) {
    $doc = new DOMDocument();
    $filename = trim($filename);
    $scheme = parse_url($filename, PHP_URL_SCHEME);
    if ($scheme == 'http') {
      return $filename;
    }
    else {
      $scheme = parse_url($this->location, PHP_URL_SCHEME);
      if ($scheme == 'http') {
        $matches = array();
        $location = $this->location;
        if (($count = preg_match_all('/\.\.\//', $filename, $matches, PREG_OFFSET_CAPTURE)) > 0) {
          for ($i = 0; $i < $count; $i++) {
            $index = strrpos($location, '/');
            $location = substr($location, 0, $index);
          }
          $filename = substr($filename, $matches[0][$count - 1][1] + 3); // +3 for "../"
        }
        $path = $location . '/' . $filename;
        return $path;
      }
      else {
        return realpath($this->location . '/' . $filename);
      }
    }
  }

  /**
   * Load the schema.
   * 
   * @param string $filename 
   */
  private function loadSchema($filename) {
    $this->schema = $this->loadDocument($filename);
    $this->configureXPath();
    $this->loadIncludedFiles();
    $this->loadImportedFiles();
  }

  /**
   * Loads the given DOMDocument.
   * 
   * @param string $filename
   * 
   * @return DOMDocument 
   */
  private function loadDocument($filename) {
    if (($doc = SchemaCache::Get($filename)) != FALSE) {
      return $doc;
    }
    else {
      $doc = new DOMDocument();
      if (!@$doc->load($filename)) { // If we can't find a document just fail, we shouldn't attempt any processing.
        throw new Exception(t('Failed to Find Document: %filename', array('%filename' => $filename)));
      }
      SchemaCache::Set($filename, $doc);
      return $doc;
    }
  }

  /**
   * Configure this schema's xpath.
   */
  private function configureXPath() {
    $this->xpath = new DOMXPath($this->schema);
    $this->xpath->registerNamespace(self::prefix, self::uri);
    $this->registerNamespaces();
  }

  /**
   * Register namepaces for use in xpath queries.
   */
  private function registerNamespaces() {
    $simple = simplexml_import_dom($this->schema->documentElement);
    $namespaces = $simple->getNamespaces(TRUE);
    foreach ($namespaces as $prefix => $uri) {
      $this->xpath->registerNamespace($prefix, $uri);
    }
  }

  /**
   * Load included schema's.
   */
  private function loadIncludedFiles() {
    $this->includes = array();
    $included_files = $this->getIncludedFiles();
    foreach ($included_files as $include_file) {
      $filename = $this->getFullPathToSchema($include_file);
      if (empty(self::$schemas[$filename])) {
        $this->includes[$filename] = new XMLSchema($filename, $this->namespace); // Included files share the same namespace.
      }
      else {
        $this->includes[$filename] = self::$schemas[$filename];
      }
    }
  }

  /**
   * Gets a list of schema files that this schema includes.
   * 
   * @return array
   */
  private function getIncludedFiles() {
    $included_files = array();
    $result = $this->xpath->query('//xs:include');
    if ($result && $result->length > 0) {
      for ($i = 0; $i < $result->length; $i++) {
        $filename = $result->item($i)->getAttribute('schemaLocation');
        $included_files[] = $filename;
      }
    }
    return $included_files;
  }

  /**
   * Load imported schema's.
   */
  private function loadImportedFiles() {
    $this->imports = array();
    $imported_files = $this->getImportedFiles();
    foreach ($imported_files as $import_file) {
      list($filename, $namespace) = $import_file;
      $filename = $this->getFullPathToSchema($filename);
      if (empty(self::$schemas[$filename])) {
        $this->imports[$filename] = new XMLSchema($filename, $namespace);
      }
      else {
        $this->imports[$filename] = self::$schemas[$filename];
      }
    }
  }

  /**
   * Gets a list of schema files that this schema imports.
   * 
   * @return array
   */
  private function getImportedFiles() {
    $imported_files = array();
    $result = $this->xpath->query('//xs:import');
    if ($result && $result->length > 0) {
      for ($i = 0; $i < $result->length; $i++) {
        $location = $result->item($i)->getAttribute('schemaLocation');
        $namespace = $result->item($i)->getAttribute('namespace');
        $imported_files[] = array($location, $namespace);
      }
    }
    return $imported_files;
  }

  /**
   * Checks to see if the given doc is valid.
   * 
   * @param DOMDocument $doc 
   */
  public function validate(DOMDocument $doc) {
    $filename = SchemaCache::GetCachedFilename($this->filename);
    if ($filename != FALSE) {
      return $doc->schemaValidate($filename);
    }
  }

  /**
   * Query the given path in this schema, and all schema's that are included or imported.
   * 
   * @param string $path
   * 
   * @return array
   */
  public function query($path) {
    if ($this->schema) {
      $results = dom_node_list_to_array($this->xpath->query($path));
      foreach ($this->includes as $schema) {
        $results = array_merge($results, $schema->query($path));
      }
      return $results;
    }
    return array();
  }

  /**
   * Gets a unique node at the given path in this schema.
   * 
   * If the node does not exist or is not unique an exception is thrown.
   * 
   * @param string $path
   * @return DOMNode
   */
  public function getUniqueNode($path) {
    $results = $this->query($path);
    if (count($results) != 1) { // Not unique
      throw new Exception('Node at path was not unique. Path: ' . $path);
    }
    return array_shift($results);
  }

  /**
   * Gets all element tags within this schema.
   * 
   * @return array
   */
  public function getAllElements() {
    return $this->query('//xs:element');
  }

  /**
   * Looks for a given global type.
   * 
   * Type is defined as a complexType or simpleType.
   * 
   * @param string $type_name
   * 
   * @return DOMElement 
   */
  public function findGlobalType($name) {
    if (isset($name) && $name != '') {
      $path = "(/xs:schema/xs:complexType[@name='$name']) | (/xs:schema/xs:simpleType[@name='$name'])";
      $results = $this->query($path);
      return count($results) == 1 ? array_shift($results) : NULL;
    }
    return NULL;
  }

  public function findGlobalElement($name) {
    if (isset($name) && $name != '') {
      $path = "/xs:schema/xs:element[@name='$name']";
      $results = $this->query($path);
      return count($results) == 1 ? array_shift($results) : NULL;
    }
    return NULL;
  }

  public function findGroup($name) {
    if (isset($name) && $name != '') {
      $path = "/xs:schema/xs:group[@name='$name']";
      $results = $this->query($path);
      return count($results) == 1 ? array_shift($results) : NULL;
    }
    return NULL;
  }

}