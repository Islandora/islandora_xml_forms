<?php

/**
 * @file
 *
 * Associate smociate..
 */

/**
 * Gets the requested association by its ID if it exists.
 *
 * @param string|int
 *   The ID of the hook/database association to fetch.
 *
 * @return array
 *   The requested association if it exists, FALSE otherwise.
 */
function xml_form_builder_get_association($id) {
  $associations = xml_form_builder_get_associations(array(), array(), array(), FALSE);
  foreach ($associations as $association) {
    if ($association['id'] == $id) {
      return $association;
    }
  }
  return FALSE;
}

/**
 * Gets an array of all associations as constrained by the given parameters.
 *
 * @param array $forms
 *   Limit the associations returned to only ones that associate the given
 *   forms.
 * @param array $models
 *   Limit the associations returned to only ones that associate the given
 *   content models.
 * @param array $dsid
 *   Limit the associations returned to only ones that associate the given
 *   datastreams.
 * @param boolean $only_enabled
 *   Return only the hook associations that are enabled.
 *
 * @return array
 *   All the associations that match the given constraints.
 */
function xml_form_builder_get_associations(array $forms = array(), array $models = array(), array $dsids = array(), $only_enabled = TRUE) {
  $associations = xml_form_builder_get_database_associations($forms , $models, $dsids);
  $associations = array_merge($associations, xml_form_builder_get_hook_associations($forms , $models, $dsids, $only_enabled));
  return $associations;
}

/**
 * Gets an array of all associations stored in the database as constrained by
 * the given parameters.
 *
 * Effectively it works as if each parameter has is and'ed with the others.
 *
 * @param array $forms
 *   Limit the associations returned to only ones that associate the given
 *   forms.
 * @param array $models
 *   Limit the associations returned to only ones that associate the given
 *   content models.
 * @param array $dsid
 *   Limit the associations returned to only ones that associate the given
 *   datastreams.
 *
 * @return array
 *   All the associations that match the given constraints.
 */
function xml_form_builder_get_database_associations(array $forms = array(), array $models = array(), array $dsids = array()) {
  $query = db_select('xml_form_builder_form_associations', 'fa')->fields('fa');
  $add_conditions = function(SelectQuery $query, $label, array $values) {
    if (!empty($values)) {
      $conditions = db_or();
      foreach ($values as $value) {
        $conditions->condition($label, $value);
      }
      $query->condition($conditions);
    }
  };
  $add_conditions($query, 'form_name', $forms);
  $add_conditions($query, 'content_model', $models);
  $add_conditions($query, 'dsid', $dsids);
  $associations = array();
  foreach ($query->execute() as $result) {
    $result->title_field = unserialize($result->title_field);
    $associations[$result->id] = (array) $result;
    $associations[$result->id]['in_db'] = TRUE;
    $associations[$result->id]['enabled'] = TRUE;
  }
  return $associations;
}

/**
 * Gets an array of all hook defined associations as constrained by the given
 * parameters.
 *
 * @param array $forms
 *   Limit the associations returned to only ones that associate the given
 *   forms.
 * @param array $models
 *   Limit the associations returned to only ones that associate the given
 *   content models.
 * @param array $dsid
 *   Limit the associations returned to only ones that associate the given
 *   datastreams.
 * @param boolean $only_enabled
 *   Return only the hook associations that are enabled.
 *
 * @return array
 *   All the associations that match the given constraints.
 */
function xml_form_builder_get_hook_associations(array $forms = array(), array $models = array(), array $dsids = array(), $only_enabled = TRUE) {
  $associations = module_invoke_all('islandora_xml_form_builder_form_associations');
  // @todo: Remove deprecated "islandora_content_model_forms_form_associations" invocation.
  $associations += module_invoke_all('islandora_content_model_forms_form_associations');
  // Filter associations based on given parameters
  $filtered_associations = array();
  foreach ($associations as $id => $association) {
    $association['id'] = $id;
    $exclude = !empty($forms) && !in_array($association['form_name'], $forms);
    $exclude |= !empty($models) && !in_array($association['content_model'], $models);
    $exclude |= !empty($dsids) && !in_array($association['dsid'], $dsids);
    if (!$exclude) {
      $filtered_associations[$id] = $association;
      $filtered_associations[$id]['in_db'] = FALSE;
      // Assume everything is enabled unless specified in the
      // "xml_form_builder_association_hooks" table.
      $filtered_associations[$id]['enabled'] = TRUE;
    }
  }
  // Set the enabled property for all filtered associations.
  $query = db_select('xml_form_builder_association_hooks', 'cm')->fields('cm');
  foreach ($query->execute() as $enabled) {
    if (isset($filtered_associations[$enabled->id])) {
      $filtered_associations[$enabled->id]['enabled'] = $enabled->enabled != 0;
    }
  }
  // Only include enabled associations.
  if ($only_enabled) {
    $filtered_associations = array_filter($filtered_associations, function($o) { return $o['enabled']; });
  }
  return $filtered_associations;
}

/**
 * Wraps invocation of hook of the same name.
 */
function xml_form_builder_get_transforms() {
  $tranforms = module_invoke_all('xml_form_builder_get_transforms');
  // @todo Remove (deprecated) invokation if
  // "islandora_content_model_forms_get_transforms".
  $tranforms += module_invoke_all('islandora_content_model_forms_get_transforms');
  $tranforms['No Transform'] = 'No Transform';
  return $tranforms;
}


/**
 * Gets the path to the transform identified by the given transform name.
 *
 * @param string $transfrom
 *   The short name for the transform.
 *
 * @return
 *   The path to the given transform if found, FALSE otherwise.
 */
function xml_form_builder_get_transform_file($transform) {
  $transforms = xml_form_builder_get_transforms();
  // No transform has no file.
  unset($transforms['No Transform']);
  foreach ($transforms as $name => $path) {
    if ($transform == $name) {
      return $path;
    }
  }
  return FALSE;
}

/**
 * Transform form generated XML to Dublin Core.
 *
 * @param string $transform
 *   The path to the transform xslt.
 * @param DOMDocument $source_document
 *   The source document to transform.
 * @param FedoraObject $object
 *   The object that the transform is form.
 *
 * @return DOMDocument
 *   The transformed DC.
 */
function xml_form_builder_transform_document($transform, $source_document, $object) {
  if ($transform !== NULL) {
    $xsl = new DOMDocument();
    $xsl->load($transform);
    $xslt = new XSLTProcessor();
    $xslt->importStyleSheet($xsl);
    $document = $xslt->transformToDoc($source_document);
    if ($document) {
      $xpath = new DOMXPath($document);
      // Set the Label
      $results = $xpath->query("*[local-name()='title']");
      $results->item(0)->nodeValue = $object->label;
      // Set the Pid
      $results = $xpath->query("*[local-name()='identifier']");
      $results->item(0)->nodeValue = $object->id;
      if (isset($document->documentElement)) {
        return $document;
      }
    }
  }
  return NULL;
}

/**
 * Implements hook_xml_form_builder_get_transforms().
 *
 * Grabs any XSLTs from the "transforms" directory inside of this module.
 */
function xml_form_builder_xml_form_builder_get_transforms() {
  $p = drupal_get_path('module', 'xml_form_builder');
  $transforms = array();
  $include_path = "$p/transforms";
  $dir = opendir($include_path);
  if ($dir !== FALSE) {
    while (($file = readdir($dir)) !== FALSE) {
      if (preg_match('/\.xslt?$/', $file)) {
        $transforms["$file"] = "$include_path/$file";
      }
    }
  }
  closedir($dir);
  return $transforms;
}
