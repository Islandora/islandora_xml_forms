<?php

/**
 * Form builder to select from associated forms, based on some given conditions.
 *
 * @param array $conditions
 *   An array of conditions on which to match.  Can include a few different 
 *   keys:
 *   - models: An array of strings containing a content model PIDs to which we
 *     should restrict the options. 
 *   - datastream: A string containing a datastream identifier, with which 
 *     we should restrict the options. (Only forms affecting the given
 *     datastream)
 *   - pid: A string containing the pid of an object, of which we'll grab the 
 *     models and process as if multiple "models" were given.
 *   - object: An AbstractFedoraObject, from which we'll obtain our list of
 *     models.
 * @param array $parameters
 *   An optional associative array of parameters to pass off to the XMLForm.
 *   Currently, just one key is used:
 *   - xml: Some XML with which to prepopulate the form, when we're building
 *     it.
 */
function xml_form_builder_selectable_form($form, &$form_state, $conditions = array(), $parameters = array()) {
  form_load_include($form_state, 'form.inc', 'xml_form_builder', 'includes/selectable');
  if (!isset($form_state['storage']['xml_form_builder']['form_name'])) {
    if (isset($conditions['pid'])) {
      $obj = islandora_load_object($conditions['pid']);
      $conditions['models'] = array_merge((array)$conditions['models'], $obj->models);
    }

    if ($conditions['object']) {
      $conditions['models'] = array_merge((array)$conditions['models'], $conditions['object']->models);
    }

    module_load_include('inc', 'xml_form_builder', 'includes/associations');
    $datastream = isset($conditions['datastream'])?
      $conditions['datastream']:
      NULL;
    $form_names = xml_form_builder_get_form_names($conditions['models'], $datastream);

    $form += array(
      'fieldset' => array(
        '#type' => 'fieldset',
        '#title' => t('Select Associated Form'),
        'form_name' => array(
          '#type' => 'select',
          '#title' => t('Form Select'),
          '#options' => $form_names,
        ),
        'submit' => array(
          '#type' => 'submit',
          '#title' => t('Submit'),
        ),
      ),
    );
  }
  else {
    $form_name = $form_state['storage']['xml_form_builder']['form_name'];
    // Get the XML form, and return it.
    $xml = $form_state['storage']['xml_form_builder']['xml'][$form_name] = isset($parameters['xml'])?
      $parameters['xml']:
      NULL;

    $form += xml_form_builder_get_form($form_state, $form_name, $xml);
  }

  return $form;
}

function xml_form_builder_selectable_form_submit($form, &$form_state) {
  if ($form_state['values']['form_name'] && !isset($form_state['storage']['xml_form_builder']['form_name'])) {
    list($form_state['storage']['xml_form_builder']['content_model'], $form_state['storage']['xml_form_builder']['form_name']) = explode(' ', $form_state['values']['forms'], 2);
    $form_state['rebuild'] = TRUE;
  }
  else {
    $xml_form = new XMLForm($form_state);
    $form_name = $form_state['storage']['xml_form_builder']['form_name'];
    $form_state['storage']['xml_form_builder']['xml'] = $xml_form->submit($form, $form_state);
  }
}

/**
 * Get the XML from the proper spot in the form storage.
 *
 * @param array $form_state
 *   The array of form state, which has at least had the second page built.
 * @param string $form_name
 *   The name of the form for which to grab the processed XML.
 *
 * @return string
 *   A string containing the XML.
 */
function xml_form_builder_selectable_form_get_xml($form_state, $form_name) {
  return $form_state['storage']['xml_form_builder']['xml'][$form_name];
}

