<?php

/**
 * @file
 * Features hook implementations.
 */

/**
 * Helper function so we can so we can identify things.
 */
function xml_form_builder_default_dc_config_get_mapped_features() {
  $mapped =& drupal_static(__FUNCTION__, array());

  if (empty($mapped)) {
    $query = db_select('xml_form_builder_default_xslts', 'mappings')
      ->fields('mappings', array());

    $alias = $query->join('xml_form_builder_xslts', 'x', 'x.xslt_id = mappings.xslt_id');
    $query->addField($alias, 'name', 'xslt_name');
    $query->addField($alias, 'xslt');

    $configs = $query->execute()
      ->fetchAllAssoc('id', PDO::FETCH_ASSOC);

    foreach ($configs as $config) {
      $mapped["{$config['content_model']}--{$config['dsid']}"] = $config;
    }
  }
  return $mapped;
}

/**
 * Implements hook_features_export_options().
 */
function xml_form_builder_default_dc_config_features_export_options() {
  $configs = xml_form_builder_default_dc_config_get_mapped_features();

  $map_to_name = function ($config) {
    return $config['name'];
  };
  $features_config = array_map($map_to_name, $configs);

  return $features_config;
}

/**
 * Implements hook_features_export().
 */
function xml_form_builder_default_dc_config_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  $export['dependencies']['xml_form_builder'] = 'xml_form_builder';
  foreach ($data as $component) {
    $export['features']['xml_form_builder_default_dc_config'][$component] = $component;
  }

  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function xml_form_builder_default_dc_config_features_export_render($module_name, $data, $export = NULL) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');

  $code = array();
  $drop_fields = drupal_map_assoc(array('id', 'xslt_id'));
  foreach (array_intersect_key(xml_form_builder_default_dc_config_get_mapped_features(), drupal_map_assoc($data)) as $key => $item) {
    $code[$key] = array_diff_key($item, $drop_fields);
  }

  $serialized_code = 'return ' . features_var_export($code) . ';';
  return array('xml_form_builder_default_dc_config' => $serialized_code);
}

/**
 * Implements hook_features_revert().
 */
function xml_form_builder_default_dc_config_features_revert($module_name) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');
  $values = module_invoke($module_name, 'xml_form_builder_default_dc_config');
  $configs = xml_form_builder_default_dc_config_get_mapped_features();

  foreach (array_intersect_key($values, $configs) as $key => $item) {
    if ($item['xslt_name'] == $configs[$key]['xslt_name']) {
      xml_form_builder_remove_default_dc_transform_mapping($configs[$key]['xslt_id']);
    }
  }

  drupal_static_reset('xml_form_builder_default_dc_config_get_mapped_features');
  xml_form_builder_default_dc_config_features_rebuild($module_name);
}

/**
 * Implements hook_features_rebuild().
 */
function xml_form_builder_default_dc_config_features_rebuild($module_name) {
  module_load_include('inc', 'xml_form_builder', 'includes/db');

  $values = module_invoke($module_name, 'xml_form_builder_default_dc_config');
  if (!empty($values)) {
    $xslts = db_select('xml_form_builder_xslts', 'x')
      ->fields('x', array())
      ->execute()
      ->fetchAllAssoc('name', PDO::FETCH_ASSOC);

    foreach (array_diff_key($values, xml_form_builder_default_dc_config_get_mapped_features()) as $key => $info) {
      if (isset($xslts[$info['xslt_name']])) {
        if ($xslts[$info['xslt_name']]['xslt'] != $info['xslt']) {
          features_log(t('Islandora default DC XSLT %name already exists, but with different content... Linking anyway.', array(
            '%name' => $info['xslt_name'],
          )));
        }
      }
      else {
        $xslts[$info['xslt_name']] = array(
          'name' => $info['xslt_name'],
          'xslt' => $info['xslt'],
          'xslt_id' => xml_form_builder_add_xslt($info['xslt'], $info['xslt_name']),
        );
      }

      xml_form_builder_add_default_dc_transform_mapping(
        $info['content_model'],
        $info['dsid'],
        $xslts[$info['xslt_name']]['xslt_id'],
        $info['name']
      );
    }
  }
}
