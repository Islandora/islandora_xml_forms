<?php

// $Id$

/**
 * @file
 * @author Nigel Banks
 *
 * Defines a FormDefinition class that allows XML form defintions to be processed by the FormGenerator, and other
 * classes.
 */
module_load_include('inc', 'php_lib', 'String');
module_load_include('inc', 'php_lib', 'Array');

/**
 * Encapulates functions 
 */
class XMLFormDefinitionVersion {

  /**
   * The version which this class repersents.
   * 
   * @var int
   */
  private $version;

  /**
   * The path to the directory where all the schema definitions are stored.
   * 
   * @return string
   */
  public static function getPathToSchemaDefinitionDirectory() {
    return drupal_get_path('module', 'xml_form_api') . '/definitions';
  }

  /**
   * The path to the directory where all the XSL Transformations are stored.
   * 
   * @return string
   */
  public static function getPathToXSLTransformDirectory() {
    return drupal_get_path('module', 'xml_form_api') . '/transforms';
  }

  /**
   * Gets a list of all the known versions of a XML Form Definition.
   * 
   * @return Array
   *   An array where all the values correspond to the defined versions of an XML Form Definition. 
   *   In order from oldest to most recent.
   */
  public static function getAllVersions() {
    static $versions = NULL;
    if (empty($versions)) {
      $path = self::getPathToSchemaDefinitionDirectory();
      $files = scandir($path);
      foreach ($files as $filename) {
        $version = str_replace('.xsd', '', $filename);
        if ($version != 'latest') { // Maybe remove
          $versions[] = (int) $version;
        }
      }
      asort($version);
    }
    return $versions;
  }

  /**
   * Checks to see if the given version number is valid.
   * 
   * Valid schema numbers are defined by having a corresponding Schema file with the same version.
   * 
   * @param int $version 
   *   A XML Form Definition version number.
   * 
   * @return bool
   */
  public static function isValid($version) {
    $versions = self::getAllVersions();
    return array_search($version, $versions) !== FALSE;
  }

  /**
   * Gets the latest schema version number.
   * 
   * @return int
   *   The latest version number.
   */
  public static function getLatestVersion() {
    $versions = self::getAllVersions();
    $latest_version = array_pop($versions);
    return new XMLFormDefinitionVersion($latest_version);
  }

  /**
   * Gets the version number of the XML Form Definition.
   * 
   * Also validates the document against that version number.s
   * 
   * @param DOMDocument $definiton
   *   The XML Form Definition.
   * 
   * @return float
   *   The version of the XML Form Definition, if the XML Form Definition is not valid FALSE is returned.
   */
  public static function getVersion(DOMDocument $definiton) {
    $declares_version = $definiton->documentElement->hasAttribute('version');
    if ($declares_version) {
      $version = (int) $definiton->documentElement->getAttribute('version');
      return self::isValid($definition, $version);
    }
    else { // Files with a version of 0 or 1 may not have their version declared. Check to see manually.
      $undeclared_versions = array(
        new XMLFormDefinitionVersion(0),
        new XMLFormDefinitionVersion(1)
      );
      foreach ($undeclared_versions as $version) {
        if (self::isValid($definition, $version)) {
          return $version;
        }
      }
    }
    return FALSE; // Could not find the version.
  }

  /**
   * Creates an instance of the XMLFormDefinitionVersion.
   * 
   * @param int $version
   *   The XML Form Definition Schema number.
   */
  public function __construct($version) {
    if (self::isValid($version)) {
      $this->current = $version;
    }
  }

  /**
   * Gets the next most recent version number from the version number given.
   * 
   * @return XMLFormDefinitionVersion
   *   The next version number if the given version is not the latest, otherwise FALSE is returned.
   */
  public function getNextVersion() {
    $versions = self::getAllVersions();
    $position = array_search($current_version, $versions);
    $next_position = $position++;
    if (isset($versions[$next_position])) {
      $next_version = $versions[$next_position];
      return new XMLFormDefinitionVersion($next_version);
    }
    return FALSE;
  }

  /**
   * Checks to see if the XML Form Definition is at the most recent version.
   * 
   * @return bool
   *   TRUE if it is the latest, FALSE otherwise.
   */
  public function isLatestVersion() {
    return $version == self::getLatestVersion();
  }

  /**
   * Gets the filename of the Schema that repersents this version.
   * 
   * @return string
   */
  public function getSchemaFileName() {
    $path = self::getPathToSchemaDefinitionDirectory();
    return $path . '/' . $version . '.xsd';
  }

  /**
   * Gets the filename of the XSL Transform that repersents this version.
   * 
   * @return string
   */
  public function getTansformFileName() {
    $path = self::getPathToSchemaDefinitionDirectory();
    return $path . '/' . $version . '.xsl';
  }

  /**
   * Gets an XSLTProcessor that has been initialized with the transform specified by $version.
   * 
   * @param int $version 
   */
  public function getTransform() {
    $filename = $this->getTansformFileName();
    $xsl = new DOMDocument();
    $xsl->load($filename);
    $xslt = new XSLTProcessor();
    $xslt->importStyleSheet($xsl);
    return $xslt;
  }

}

/**
 * This class is used to validate and transform XML Form Definitions into 
 */
class XMLFormDefinition {

  /**
   * Gets the version number of the XML Form Definition.
   * 
   * Also validates the document against that version number.s
   * 
   * @param DOMDocument $definiton
   *   The XML Form Definition.
   * 
   * @return float
   *   The version of the XML Form Definition, if the XML Form Definition is not valid FALSE is returned.
   */
  public static function getVersion(DOMDocument $definiton) {
    $declares_version = $definiton->documentElement->hasAttribute('version');
    if ($declares_version) {
      $version = (int) $definiton->documentElement->getAttribute('version');
      $version = new XMLFormDefinitionVersion($version);
      if (self::isValid($definition, $version)) {
        return $version;
      }
      return FALSE; // The version defined didn't match up with the Schema.
    }
    else { // Files with a version of 0 or 1 may not have their version declared. Check to see manually.
      $undeclared_versions = array(
        new XMLFormDefinitionVersion(0),
        new XMLFormDefinitionVersion(1)
      );
      foreach ($undeclared_versions as $version) {
        if (self::isValid($definition, $version)) {
          return $version;
        }
      }
    }
    return FALSE; // Could not find the version.
  }

  /**
   * Uses the XML Schema associated with the XML Form Definitions version to validate the definition
   * 
   * @param DOMDocument $definition
   *   The XML Form Definition.
   * @param XMLFormDefinitionVersion $version 
   *   The schema version, if none is give the latest is used. 
   * 
   * @return boolean
   *   TRUE if the XML Form Definition is valid, FALSE otherwise.
   */
  public static function isValid(DOMDocument $definition, XMLFormDefinitionVersion $version = NULL) {
    $version = isset($version) ? $version : XMLFormDefinitionVersion::getLatestVersion();
    $file_name = $version->getSchemaFileName();
    return @$definiton->schemaValidate($file_name);
  }

  /**
   * Incrementally upgrades the XML Form Definition, to its next version after its own.
   * 
   * If the document is already at its latest version nothing occurs.
   *
   * @param DOMDocument $definition
   *   The XML Form Definition to upgrade. This may be modified by this function.
   * 
   * @return XMLFormDefinitionVersion
   *   The upgraded version of the XML Form Definition.
   */
  public static function upgradeVersion(DOMDocument &$definition) {
    $current = self::getVersion($definiton);
    $next = $current->getNextVersion();
    if ($next) { // There exists a version to upgrade to.
      $xslt = $next->getTransform();
      $definition = $xslt->transformToDoc($definition->documentElement);
      return $next;
    }
    return $current; // Do nothing
  }

  /**
   * Repeatedly upgrades the XML Form Definition, till it reaches the latest version.
   * 
   * @param DOMDocument $definition 
   *   The XML Form Definition to upgrade. This may be modified by this function.
   * 
   * @return XMLFormDefinitionVersion
   *   The upgraded version of the XML Form Definition.
   */
  public static function upgradeToLatestVersion(DOMDocument &$definition) {
    do {
      $version = self::upgradeVersion($definition);
    } while (!$version->isLatestVersion());
    return $version;
  }

}

/**
 * Describes a form
 */
class XMLFormDefinitions {

  /**
   * Checks to see if a xml form definition is valid.
   * 
   * @param mixed $definition
   *   The xml form definition to validated. Can be a string a DOMDocument or a SimpleXMLElement
   * @return boolean
   *   TRUE if $definition is a valid form definition, FALSE otherwise.
   */
  public static function isValid(&$definition) {
    $path = drupal_get_path('module', 'xml_form_api') . '/XMLFormDefinition.xsd';
    if (is_string($definition)) {
      $document = new DOMDocument();
      $document->loadXML($definition);
      return @$document->schemaValidate($path);
    }
    else if (get_class($definition) == 'DOMDocument') {
      return @$definition->schemaValidate($path);
    }
    else if (get_class($definition) == 'SimpleXMLElement') {
      $form = dom_import_simplexml($definition);
      $document = new DOMDocument('1.0');
      $form = $document->importNode($form, true);
      $form = $document->appendChild($form);
      return @$document->schemaValidate($path);
    }
    return FALSE;
  }

  /**
   * Create
   */
  public static function Create(array $properties, array $form) {
    $definition = new SimpleXMLElement('<definition xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"></definition>');
    self::AddPropertiesToDefinition($definition, $properties);
    self::AddFormToDefinition($definition, new FormElement($form));
    return $definition;
  }

  /**
   * 
   * @param SimpleXMLElement $definition 
   * @param array $properties
   */
  private static function AddPropertiesToDefinition(SimpleXMLElement $definition, array &$properties) {
    $form_properties = $definition->addChild('properties');
    if (isset($properties['root_name'])) {
      $form_properties->addChild('root_name', $properties['root_name']);
    }
    if (isset($properties['schema_uri'])) {
      $form_properties->addChild('schema_uri', $properties['schema_uri']);
    }
    $namespaces = $form_properties->addChild('namespaces');
    if ($properties['default_uri']) {
      $namespaces->addAttribute('default', $properties['default_uri']);
    }
    if (isset($properties['namespaces'])) {
      foreach ($properties['namespaces'] as $prefix => $uri) {
        $namespace = $namespaces->addChild('namespace', $uri);
        $namespace->addAttribute('prefix', $prefix);
      }
    }
  }

  /**
   *
   * @param SimpleXMLElement $definition
   * @param FormElement $form
   */
  private static function AddFormToDefinition(SimpleXMLElement $definition, FormElement $root) {
    $form = $definition->addChild('form');
    $properties = $form->addChild('properties');
    foreach ($root->controls as $key => $value) {
      self::AddFormControlToProperties($properties, $key, $value);
    }
    $children = $form->addChild('children');
    foreach ($root->children as $key => $child) {
      self::AddFormElementToChildren($children, $key, $child);
    }
  }

  private static function AddFormControlToProperties(SimpleXMLElement $properties, $key, $value) {
    if ($value instanceof FormPropertyInterface) {
      $value = $value->toDrupalForm();
    }
    if (is_bool($value)) {
      $value = $value ? 'true' : 'false';
    }
    $key = trim($key, '#');
    if (!is_array($value)) {
      $control = $properties->addChild($key, $value);
      if ($key == 'option') { // Hacktastic...
        $control->addAttribute('key', $value);
      }
    }
    else {
      $control = $properties->addChild($key);
      foreach ($value as $index => $item) {
        $index = trim($index, '#');
        $name = ($key == 'options') ? 'option' : $index; // Hacktastic...
        self::AddFormControlToProperties($control, $name, $item);
      }
    }
  }

  private static function AddFormElementToChildren(SimpleXMLElement $children, $key, FormElement $child) {
    $element = $children->addChild('element');
    if (isset($key)) {
      $element->addAttribute('name', $key);
    }
    $properties = $element->addChild('properties');
    foreach ($child->controls as $key => $value) {
      self::AddFormControlToProperties($properties, $key, $value);
    }
    $children = $element->addChild('children');
    foreach ($child->children as $key => $grandchild) {
      self::AddFormElementToChildren($children, $key, $grandchild);
    }
  }

  /**
   * Get form properties.
   * 
   * @param sting $xml_definition
   * 
   * @return string 
   */
  public static function GetFormProperties(&$xml_definition) {
    if (!self::isValid($xml_definition)) {
      return array(
        'root_name' => NULL,
        'schema_uri' => NULL,
        'namespaces' => array(),
        'default_uri' => NULL
      );
    }
    $definition = simplexml_load_string($xml_definition);
    $properties = $definition->properties;
    $root_name = (string) $properties->root_name;
    $schema_uri = isset($properties->schema_uri) ? (string) $properties->schema_uri : NULL;
    $namespaces = array();
    $default_uri = NULL;
    if (isset($properties->namespaces)) {
      $default_uri = isset($properties->namespaces['default']) ? (string) $properties->namespaces['default'] : NULL;
      $children = $properties->namespaces->children();
      foreach ($children as $namespace) {
        $prefix = (string) $namespace['prefix'];
        $namespaces[$prefix] = (string) $namespace;
      }
    }
    return array(
      'root_name' => $root_name,
      'schema_uri' => $schema_uri,
      'namespaces' => $namespaces,
      'default_uri' => $default_uri,
    );
  }

  /**
   * Get a drupal form definition from the given XML based one.
   * 
   * @param string $xml_definition 
   * 
   * @return array
   */
  public static function GetDrupalForm(&$xml_definition) {
    if (!self::isValid($xml_definition)) {
      return array(); // Empty form.
    }
    $definition = simplexml_load_string($xml_definition);
    return self::GetElement($definition->form);
  }

  /**
   *
   * @param SimpleXMLElement $element
   * @return type 
   */
  private static function GetElementProperties(SimpleXMLElement $element) {
    $output = array();
    if (isset($element->properties)) {
      $properties = $element->properties->children();
      foreach ($properties as $property) {
        $name = $property->getName();
        $output["#$name"] = self::GetElementProperty($property);
      }
    }
    return $output;
  }

  /**
   *
   * @param SimpleXMLElement $property
   * @return type 
   */
  private static function GetElementProperty(SimpleXMLElement $property) {
    $children = $property->children();
    if (count($children) == 0) {
      return cast_string_to_boolean((string) $property); // If cast fails the string is returned.
    }
    $output = array();
    foreach ($children as $child) {
      $name = self::GetElementPropertyName($child);
      $output[$name] = self::GetElementProperty($child);
    }
    return $output;
  }

  private static function GetElementPropertyName(SimpleXMLElement $property) {
    if (isset($property['key'])) {
      $key = (string) $property['key'];
      return strcasecmp($key, 'NULL') == 0 ? NULL : $key;
    }
    return $property->getName();
  }

  private static function GetElementChildren(SimpleXMLElement $element) {
    $output = array();
    if (isset($element->children)) {
      $elements = $element->children->children();
      foreach ($elements as $element) {
        $key = isset($element['name']) ? (string) $element['name'] : NULL;
        array_add($output, $key, self::GetElement($element));
      }
    }
    return $output;
  }

  private static function GetElement(SimpleXMLElement $element) {
    $output = self::GetElementProperties($element);
    return array_merge($output, self::GetElementChildren($element));
  }

}

