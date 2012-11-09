<?php

/**
 * Autocomplete the content model name.
 *
 * @param string $string
 *   A search string.
 * @return string
 *   The rendered JSON results.
 */
function xml_form_builder_content_model_autocomplete($string) {
  $content_models = xml_form_builder_get_content_model_names();
  $output = array();
  foreach ($content_models as $value) {
    if (preg_match("/{$string}/i", $value) !== 0) {
      $output[$value] = $value;
    }
  }
  $output[$string] = $string;
  return drupal_json_output($output);
}

/**
 * Gets a map of form names suitable for use as select #options.
 */
function xml_form_builder_get_content_model_names() {
  $content = xml_form_builder_query_content_models();
  return islandora_content_model_forms_parse_query($content);
}

/**
 * Perform a resource index query to determine get a list of content models.
 *
 * Only returns content models with at least one subscribing object. 
 *
 * @return array
 *   An array of RI results, as given by the Tuque RI query interface.
 */
function xml_form_builder_query_content_models() {
  module_load_include('inc', 'islandora', 'includes/tuque');

  $connection = new IslandoraTuque();

  $query = 'select $object $model from <#ri>
  where (walk($model <fedora-model:hasModel><info:fedora/fedora-system:ContentModel-3.0>
  and $model <fedora-model:hasModel> $object))
  minus $object <mulgara:is><info:fedora/fedora-system:FedoraObject-3.0>
  minus $object <mulgara:is><info:fedora/fedora-system:ContentModel-3.0>
  minus $object <mulgara:is><info:fedora/fedora-system:ServiceDefinition-3.0>
  minus $object <mulgara:is><info:fedora/fedora-system:ServiceDeployment-3.0>
  order by $object';

  $results = $connection->repository->ri->itqlQuery($query);

  return $results;
}

/**
 * Minor array transformation.
 *
 * @param array $content
 *   The array of results as returned from Tuque's RI query interface.
 * @return array
 *   An array of results in a more usable format.
 */
function xml_form_builder_parse_query($content) {
  $content_models = array();
  foreach ($content as $model) {
    $content_models[] = $model['object']['value'];
  }

  $content_models = array_unique($content_models);
  $content_models = array_values($content_models);

  return $content_models;
}
