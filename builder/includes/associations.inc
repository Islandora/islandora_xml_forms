<?php

/**
 * @file
 *
 * Associate smociate..
 */

/**
 * Gets an array of all associations as constrained by the given parameters.
 *
 * @param array $forms
 *   Limit the associations returned to only ones that associate the given forms.
 * @param array $models
 *   Limit the associations returned to only ones that associate the given content models.
 * @param array $dsid
 *   Limit the associations returned to only ones that associate the given datastreams.
 * @param boolean $only_enabled
 *   Return only the hook associations that are enabled.
 *
 * @return array
 *   All the associations that match the given constraints.
 */
function xml_form_builder_get_associations(array $forms = array(), array $models = array(), array $dsids = array(), $only_enabled = FALSE) {
  $associations = xml_form_builder_get_database_associations($forms , $models, $dsids);
  $associations = array_merge($associations, xml_form_builder_get_hook_associations($forms , $models, $dsids, $only_enabled));
  return $associations;
}

/**
 * Gets an array of all associations stored in the database as constrained by the given parameters.
 *
 * @param array $forms
 *   Limit the associations returned to only ones that associate the given forms.
 * @param array $models
 *   Limit the associations returned to only ones that associate the given content models.
 * @param array $dsid
 *   Limit the associations returned to only ones that associate the given datastreams.
 *
 * @return array
 *   All the associations that match the given constraints.
 */
function xml_form_builder_get_database_associations(array $forms = array(), array $models = array(), array $dsids = array()) {
  $query = db_select('xml_form_builder_form_associations', 'fa')->fields('fa');
  $conditions = db_or();
  $add_condition = function($value, $key, $label) use(&$conditions) { $conditions->condition($label, $value); };
  array_walk($forms, $add_condition, 'form_name');
  array_walk($models, $add_condition, 'content_model');
  array_walk($dsids, $add_condition, 'dsid');
  $query->condition($conditions);
  $associations = array();
  foreach ($query->execute() as $result) {
    $title_field = &$result->title_field;
    $title_field = str_replace("'", "", $title_field);
    $title_field = substr($title_field, 1, strlen($title_field) - 2);
    $title_field = explode("][", $title_field);
    $associations[] = (array) $result;
  }
  return $associations;
}

/**
 * Gets an array of all hook defined associations as constrained by the given parameters.
 *
 * @param array $forms
 *   Limit the associations returned to only ones that associate the given forms.
 * @param array $models
 *   Limit the associations returned to only ones that associate the given content models.
 * @param array $dsid
 *   Limit the associations returned to only ones that associate the given datastreams.
 * @param boolean $only_enabled
 *   Return only the hook associations that are enabled.
 *
 * @return array
 *   All the associations that match the given constraints.
 */
function xml_form_builder_get_hook_associations(array $forms = array(), array $models = array(), array $dsids = array(), $enabled_only = FALSE) {
  $associations = module_invoke_all('islandora_xml_form_builder_form_associations');
  $associations += module_invoke_all('islandora_content_model_forms_form_associations'); // @todo: Remove deprecated "islandora_content_model_forms_form_associations" invocation.
  $filtered_associations = array();
  foreach ($associations as $id => $association) {
    $association['id'] = $id;
    $exclude = !empty($forms) && !in_array($association['form_name'], $forms);
    $exclude |= !empty($models) && !in_array($association['content_model'], $models);
    $exclude |= !empty($dsids) && !in_array($association['dsid'], $dsids);
    if (!$exclude) {
      $filtered_associations[] = $association;
    }
  }
  if ($enabled_only) {
    $query = db_select('xml_form_builder_association_hooks', 'cm')->fields('cm');
    foreach ($query->execute() as $enabled) {
      $only_enabled = function($o) use($enabled) { return !($o['id'] == $enabled->id && $enabled->enabled == 0); };
      $filtered_associations = array_filter($filtered_associations, $only_enabled);
    }
  }
  return $filtered_associations;
}

/**
 * Get form names, optional condition on datastream ID.
 *
 * @param string|array $content_model
 *   Either a single string or an array of strings containing content model
 *   PIDs.
 * @param string|array|null $ds
 *   Either a single string or an array of strings containing datastream
 *   identifiers, or NULL to not filter based on datastream identifier.
 *
 * @return array
 *   An associative array listing the forms, where the key is the unique form
 *   name, and the value is a string listing the form name and the datastream
 *   on which it operates.
 */
function xml_form_builder_get_form_names($content_model, $ds = NULL) {
  module_load_include('inc', 'xml_form_builder', 'XMLFormRepository');
  $list = array();
  $query = db_select('xml_form_builder_form_associations', 'fa')
    ->fields('fa', array('form_name', 'dsid', 'content_model'))
    ->condition('content_model', $content_model);

  if ($ds !== NULL) {
    $query->condition('dsid', $ds);
  }
  $result = $query->execute();
  foreach ($result as $data) {
    if ($data->content_model == $content_model && XMLFormRepository::Valid($data->form_name)) {
      $list[$data->form_name] = "{$data->form_name} ({$data->dsid})";
    }
  }

  $hook = xml_form_builder_form_associations(TRUE);
  $cmodels = (array)$content_model;
  foreach ($hook as $data) {
    if (in_array($data['content_model'], $cmodels) && XMLFormRepository::Valid($data['form_name'])) {
      if ($ds === NULL || $ds == $data['dsid']) {
        $list[$data['form_name']] = "{$data['form_name']} ({$data['dsid']})";
      }
    }
  }

  return $list;
}

/**
 * Lists the existing forms.
 */
function xml_form_builder_associations_form($form, &$form_state, $form_name) {
  // Make sure we can find our own builder, in the case of AJAX.
  form_load_include($form_state, 'inc', 'xml_form_builder', 'includes/associations');

  $list = array();

  $hook_forms = xml_form_builder_form_associations($form_name);

  foreach ($hook_forms as $key => $hook_form) {
    $hook_form['id'] = $key;
    $hook_form['type'] = 'hook';
    // tranform a potential string to boolean
    $hook_form['template'] = ($hook_form['template'] != '');
    $list[] = $hook_form;
  }

  $result = db_select('xml_form_builder_form_associations', 'fa')
    ->fields('fa')
    ->condition('form_name', $form_name)
    ->execute()
    ->fetchAll();
  foreach ($result AS $data) {
    $list[] = array(
      'type' => 'custom',
      'id' => $data->id,
      'content_model' => $data->content_model,
      'form_name' => $data->form_name,
      'dsid' => $data->dsid,
      'title_field' => $data->title_field,
      'transform' => $data->transform,
      'template' => $data->template != '',
    );
  }

  $transforms = xml_form_builder_get_transforms();

  $form += array(
    'list' => array(
      '#type' => 'fieldset',
      '#title' => t('Current associations'),
      '#value' => theme('xml_form_builder_association_table', array(
                  'list' => $list,
                )),
    ),
    'fieldset' => array(
      '#type' => 'fieldset',
      '#title' => t('Add Association'),
      '#collapsible' => TRUE,
      'content_model' => array(
        '#type' => 'textfield',
        '#title' => t('Content Model'),
        '#required' => TRUE,
        '#autocomplete_path' => MENU_XML_FORM_BUILDER_CONTENT_MODEL_AUTOCOMPLETE,
        '#description' => t('The content model to associate with a form. If the content model has no decendants it will not show up in autocomplete.'),
        '#default_value' => isset($form_state['values']['content_model']) ? $form_state['values']['content_model'] : NULL,
      ),
      'dsid' => array(
        '#type' => 'textfield',
        '#description' => t('The datastream ID of where the objects metadata is stored.'),
        '#title' => t('Metadata Datastream ID'),
        '#required' => TRUE,
        '#default_value' => isset($form_state['values']['dsid']) ? $form_state['values']['dsid'] : NULL,
      ),
      'form_name' => array(
        '#type' => 'value',
        '#title' => t('Form Name'),
        '#value' => $form_name,
      ),
      'title_field' => array(
        '#type' => 'select',
        '#title' => t('Title Field'),
        '#description' => t('The form field that you want to use for the objects label.'),
        '#prefix' => '<div id="ahah-wrapper">',
        '#suffix' => '</div>',
        '#options' => xml_form_builder_get_title_options($form_name),
        '#required' => TRUE,
      ),
      'transform' => array(
        '#type' => 'select',
        '#title' => t('XSL Transform'),
        '#options' => $transforms,
        '#default_value' => 'No Transform',
        '#description' => t('A xsl transform for setting the Fedora Object\'s Dublin Core metadata datastream.'),
      ),
      'file' => array(
        '#type' => 'file',
        '#title' => t('Upload Template Document'),
        '#description' => t('A sample metadata file used to prepopulate the form on ingest.'),
      ),
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Add Association'),
      ),
    ),
  );

  return $form;
}

/**
 * Form submit handler.
 */
function xml_form_builder_associations_form_submit($form, &$form_state) {
  $object_keys = array(
    'content_model',
    'form_name',
    'dsid',
    'title_field',
    'transform',
  );

  $object = array_intersect_key(
    $form_state['values'],
    array_combine($object_keys, $object_keys)
  );


  $object['template'] = '';
  $file_uploaded = $_FILES['files']['error']['file'] == 0;
  if ($file_uploaded) {
    $definition = new DOMDocument();
    $definition->load($_FILES['files']['tmp_name']['file']);
    $object['template'] = $definition->saveXML();
  }
  if (drupal_write_record('xml_form_builder_form_associations', $object) !== FALSE) {
    drupal_set_message(t('Successfully added association.'));
  }
  else {
    drupal_set_message(t('Failed to add association.'), 'error');
  }
}

/**
 * Get an array of "array paths" from which an object label may be extracted.
 *
 * @param string $form_name
 *   The name of the form in question.
 *
 * @return array
 *   An array of options from which a title may be extracted.
 */
function xml_form_builder_get_title_options($form_name) {
  if (isset($form_name)) {
    module_load_include('inc', 'xml_form_builder', 'XMLFormRepository');
    $doc = XMLFormRepository::Get($form_name);
    if ($doc) {
      $xpath = new DOMXPath($doc);
      $result = $xpath->query('//element[@name]');
      $elements = dom_node_list_to_array($result);
      if (count($elements) > 0) {
        $output = array();
        foreach ($elements as $element) {
          $result = $xpath->query('ancestor-or-self::node()[@name]', $element);
          $ancestors = dom_node_list_to_array($result);
          $value = $label = '';
          if (count($ancestors) > 0) {
            foreach ($ancestors as $ancestor) {
              $name = $ancestor->getAttribute('name');
              $value .= "['$name']";
              $label .= "['$name']";
            }
          }
          $output[$value] = $label;
        }
        return $output;
      }
    }
  }
  return array(NULL => 'invalid');
}

class XmlFormBuilderFormAssociationArrayFilter {
  protected $form_name;
  function __construct($form_name) {
    $this->form_name = $form_name;
  }

  function filter($form_info) {
    return $form_info['form_name'] == $this->form_name;
  }
}

/**
 * Returns the associations defined by the hook along with their status from
 * the database.
 *
 * @param boolean $only_return_enabled
 *   If this value is true then only associations that are enabled are returned.
 *
 * @return array
 */
function xml_form_builder_form_associations($form_name, $only_return_enabled = FALSE) {
  $hook_associations = module_invoke_all('islandora_xml_form_builder_form_associations');

  // TODO: Remove deprecated "islandora_content_model_forms_form_associations" invocation.
  $hook_associations += module_invoke_all('islandora_content_model_forms_form_associations');

  $return_associations = array();

  $associations = db_select('xml_form_builder_association_hooks', 'cm')
    ->fields('cm')
    ->execute();

  foreach ($associations as $association) {
    if (isset($hook_associations[$association->id])) {
      $hook_associations[$association->id]['enabled'] = ($association->enabled == 1);
      $hook_associations[$association->id]['in_db'] = TRUE;

      if (($only_return_enabled && $association->enabled == 1) || !$only_return_enabled) {
        $return_associations[$association->id] = $hook_associations[$association->id];
      }

      unset($hook_associations[$association->id]);
    }
  }

  foreach ($hook_associations as $key => $assocation) {
    $assocation['enabled'] = TRUE;
    $assocation['in_db'] = FALSE;
    $return_associations[$key] = $assocation;
  }

  // XXX: Don't like that this is necessary to filter, but anyway...
  $filter = new XmlFormBuilderFormAssociationArrayFilter($form_name);
  $return_associations = array_filter($return_associations, array($filter, 'filter'));

  return $return_associations;
}

/**
 * Remove the association identified by $id.
 *
 * @param string $form_name
 *   The name of the form for which the associations are being adjusted.
 *   (used to redirect).
 * @param string|int $id
 *   The identifier for the form association.  A string for "default" forms
 *   (added in via associations), and an integer for associations added via
 *   the form.
 */
function xml_form_builder_disable_association($form_name, $id) {
  if (is_numeric($id)) {
    db_delete('xml_form_builder_form_associations')
      ->condition('id', intval($id))
      ->execute();
    drupal_set_message(t('Deleted the association ID:%id with the form %form_name.', array(
          '%id' => $id,
          '%form_name' => $form_name,
        )));
  }
  else {
    $hook_forms = xml_form_builder_form_associations($form_name);
    if (!isset($hook_forms[$id])) {
      drupal_set_message(t('Specified association does not exist.'), 'error');
    }
    else {
      if ($hook_forms[$id]['in_db']) {
        db_update('xml_form_builder_association_hooks')
          ->fields(array('enabled' => (int)FALSE))
          ->condition('id', $id)
          ->execute();
      }
      else {
        db_insert('xml_form_builder_association_hooks')
          ->fields(array(
              'id' => $id,
              'enabled' => (int)FALSE,
            ))
          ->execute();
      }

      drupal_set_message(t('Successfully disabled association.'));
    }
  }

  drupal_goto(xml_form_builder_get_associate_form_path($form_name));
}


/**
 * Enable a default association identified by $id.
 *
 * @param string $form_name
 *   The name of the form for which the associations are being adjusted.
 *   (used to redirect).
 * @param string $id
 *   The identifier for the form association. Note that only "default"
 *   associations added via hook_xml_form_builder_form_associations() can be
 *   enabled.
 */
function xml_form_builder_enable_association($form_name, $id) {
  $hook_forms = xml_form_builder_form_associations($form_name);
  if (!isset($hook_forms[$id])) {
    drupal_set_message(t('Specified association does not exist.'), 'error');
  }
  else {
    if ($hook_forms[$id]['in_db']) {
      db_update('xml_form_builder_association_hooks')
        ->fields(array('enabled' => (int)TRUE))
        ->condition('id', $id)
        ->execute();
    }
    else {
      db_insert('xml_form_builder_association_hooks')
        ->fields(array(
            'id' => $id,
            'enabled' => (int)TRUE,
          ))
        ->execute();
    }

    drupal_set_message(t('Successfully enabled association.'));
  }

  drupal_goto(xml_form_builder_get_associate_form_path($form_name));
}

/**
 * Wraps invocation of hook of the same name.
 *
 * Alter
 */
function xml_form_builder_get_transforms() {
  $tranforms = module_invoke_all('xml_form_builder_get_transforms');
  // TODO: Remove (deprecated) invokation if "islandora_content_model_forms_get_transforms".
  $tranforms += module_invoke_all('islandora_content_model_forms_get_transforms');
  $tranforms['No Transform'] = 'No Transform';
  return $tranforms;
}

/**
 * Implements hook_xml_form_builder_get_transforms().
 *
 * Grabs any XSL(T)s from the "transforms" directory inside of this module.
 */
function xml_form_builder_xml_form_builder_get_transforms() {
  $p = drupal_get_path('module', 'xml_form_builder');
  $transforms = array();
  $include_path = "$p/transforms";
  $dir = opendir($include_path);
  if ($dir !== FALSE) {
    while (($file = readdir($dir)) !== FALSE) {
      if (preg_match('/\.xslt?$/', $file)) {
        $transforms["$file (Islandora)"] = "$include_path/$file";
      }
    }
  }
  closedir($dir);
  return $transforms;
}
