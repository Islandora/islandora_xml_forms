<?php

/**
 * @file
 * Code for `creative_commons` form elements.
 */

/**
 * Constants
 */
const XML_FORM_ELEMENTS_CREATIVE_COMMONS_BASE_URL = 'http://creativecommons.org/';
const XML_FORM_ELEMENTS_CREATIVE_COMMONS_API_URL = 'https://api.creativecommons.org/rest/1.5/';
const XML_FORM_ELEMENTS_CREATIVE_COMMONS_LICENSE_URI_DEFAULT = 'http://creativecommons.org/licenses/by/4.0/';
const XML_FORM_ELEMENTS_CREATIVE_COMMONS_DISABLED_DEFAULT = TRUE;

/**
 * Creates a `creative_commons` form element.
 *
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The element definition.
 */
function xml_form_elements_creative_commons($element, &$form_state) {
  $license_options = array(
    'none' => t('Do not output any license'),
    'by' => t('Creative Commons Attribution'),
    'zero' => t('CC0 Universal (Public Domain)'),
    'mark' => t('Public Domain Mark'),
    'manual' => t('Manually enter a license URI'),
  );

  $modification_options = array(
    'y' => t('Yes'),
    'n' => t('No'),
    'sa' => t('Yes, as long as others share alike'),
  );

  $commercial_options = array(
    'y' => t('Yes'),
    'n' => t('No'),
  );

  // List retrieved on 2018-04-28 from
  // https://api.creativecommons.org/rest/1.5/support/jurisdictions
  $countries = array(
    'international' => t('International'),
    'ar' => t('Argentina'),
    'au' => t('Australia'),
    'at' => t('Austria'),
    'be' => t('Belgium'),
    'br' => t('Brazil'),
    'bg' => t('Bulgaria'),
    'ca' => t('Canada'),
    'cl' => t('Chile'),
    'cn' => t('China Mainland'),
    'co' => t('Colombia'),
    'cr' => t('Costa Rica'),
    'hr' => t('Croatia'),
    'cz' => t('Czech Republic'),
    'dk' => t('Denmark'),
    'ec' => t('Ecuador'),
    'eg' => t('Egypt'),
    'ee' => t('Estonia'),
    'fi' => t('Finland'),
    'fr' => t('France'),
    'de' => t('Germany'),
    'gr' => t('Greece'),
    'gt' => t('Guatemala'),
    'hk' => t('Hong Kong'),
    'hu' => t('Hungary'),
    'igo' => t('IGO'),
    'in' => t('India'),
    'ie' => t('Ireland'),
    'il' => t('Israel'),
    'it' => t('Italy'),
    'jp' => t('Japan'),
    'kr' => t('Korea'),
    'lu' => t('Luxembourg'),
    'mk' => t('Macedonia'),
    'my' => t('Malaysia'),
    'mt' => t('Malta'),
    'mx' => t('Mexico'),
    'nl' => t('Netherlands'),
    'nz' => t('New Zealand'),
    'no' => t('Norway'),
    'pe' => t('Peru'),
    'ph' => t('Philippines'),
    'pl' => t('Poland'),
    'pt' => t('Portugal'),
    'pr' => t('Puerto Rico'),
    'ro' => t('Romania'),
    'rs' => t('Serbia'),
    'sg' => t('Singapore'),
    'si' => t('Slovenia'),
    'za' => t('South Africa'),
    'es' => t('Spain'),
    'se' => t('Sweden'),
    'ch' => t('Switzerland'),
    'tw' => t('Taiwan'),
    'th' => t('Thailand'),
    'ug' => t('Uganda'),
    'uk' => t('UK: England & Wales'),
    'scotland' => t('UK: Scotland'),
    'us' => t('United States'),
    've' => t('Venezuela'),
    'vn' => t('Vietnam'),
  );

  // Get an ID for ajax.
  if (isset($form_state['storage']['xml_form_elements'][$element['#name']]['license_fieldset_id'])) {
    $license_fieldset_id = $form_state['storage']['xml_form_elements'][$element['#name']]['license_fieldset_id'];
  }
  else {
    $license_fieldset_id = drupal_html_id('license_fieldset');
    $form_state['storage']['xml_form_elements'][$element['#name']]['license_fieldset_id'] = $license_fieldset_id;
  }

  // Get current values and store them in the element.
  $input = isset($form_state['input'][$element['#name']]) ? $form_state['input'][$element['#name']] : NULL;
  $values = xml_form_elements_creative_commons_get_value_array_from_input($element, $input, $form_state);
  $element['#value'] = $values;

  // Form elements.

  $element['license_fieldset'] = array(
    '#id' => $license_fieldset_id,
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#title' => t('License'),
  );

  $element['license_fieldset']['license_type'] = array(
    '#type' => 'select',
    '#title' => t('Select a license type'),
    '#description' => t('Select "@option" to avoid outputting a creative commons license. This will still output a blank element in the resulting XML, so make sure to use a cleanup template to remove it.', array('@option' => $license_options['none'])),
    '#options' => $license_options,
    '#default_value' => $values['license_fieldset']['license_type'],
    '#ajax' => array(
      'wrapper' => $license_fieldset_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
  );

  $element['license_fieldset']['allow_modifications'] = array(
    '#type' => 'select',
    '#title' => t('Allow modifications of your work?'),
    '#description' => t('Note that this is only relevant if you selected "@option" above.', array('@option' => $license_options['by'])),
    '#options' => $modification_options,
    '#default_value' => $values['license_fieldset']['allow_modifications'],
    '#ajax' => array(
      'wrapper' => $license_fieldset_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
    '#states' => array(
      'invisible' => array(
        ":input[name=\"{$element['#name']}[license_fieldset][license_type]\"]" => array('!value' => 'by'),
      ),
    ),
  );

  $element['license_fieldset']['allow_commercial'] = array(
    '#type' => 'select',
    '#title' => t('Allow commercial uses of your work?'),
    '#description' => t('Note that this is only relevant if you selected "@option" above.', array('@option' => $license_options['by'])),
    '#options' => $commercial_options,
    '#default_value' => $values['license_fieldset']['allow_commercial'],
    '#ajax' => array(
      'wrapper' => $license_fieldset_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
    '#states' => array(
      'invisible' => array(
        ":input[name=\"{$element['#name']}[license_fieldset][license_type]\"]" => array('!value' => 'by'),
      ),
    ),
  );

  $element['license_fieldset']['license_jurisdiction'] = array(
    '#type' => 'select',
    '#title' => t('License Jurisdiction'),
    '#options' => $countries,
    '#description' => t('Note that this is only relevant if you selected "@option" above.', array('@option' => $license_options['by'])),
    '#default_value' => $values['license_fieldset']['license_jurisdiction'],
    '#ajax' => array(
      'wrapper' => $license_fieldset_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
    '#states' => array(
      'invisible' => array(
        ":input[name=\"{$element['#name']}[license_fieldset][license_type]\"]" => array('!value' => 'by'),
      ),
    ),
  );

  $element['license_fieldset']['license_version'] = array(
    '#type' => 'textfield',
    '#title' => t('Override License Version'),
    '#description' => t('Optionally set to a previous version (leave empty to get newest).'),
    '#size' => 3,
    '#maxlength' => 3,
    '#default_value' => $values['license_fieldset']['license_version'],
    '#ajax' => array(
      'wrapper' => $license_fieldset_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
      'keypress' => TRUE,
    ),
    '#states' => array(
      'invisible' => array(
        ":input[name=\"{$element['#name']}[license_fieldset][license_type]\"]" => array(array('value' => 'none'), array('value' => 'manual')),
      ),
    ),
  );

  $element['license_fieldset']['license_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Manually enter a license URI'),
    '#description' => t('Note that you need to set the license type to "@option" for this to take effect.', array('@option' => $license_options['manual'])),
    '#default_value' => $values['license_fieldset']['license_uri'],
    '#ajax' => array(
      'wrapper' => $license_fieldset_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
      'keypress' => TRUE,
    ),
    '#states' => array(
      'invisible' => TRUE,
      'visible' => array(
        ":input[name=\"{$element['#name']}[license_fieldset][license_type]\"]" => array('value' => 'manual'),
      ),
    ),
  );

  // Prepare license output markup.
  $cc_markup = t('None.');
  // Only get markup if user wants a license and have a license URI set.
  if ($values['license_fieldset']['license_type'] != 'none' && !empty($values['license_fieldset']['license_uri'])) {
    // Get data from storage.
    $current_query = xml_form_elements_creative_commons_make_value_array('', '', '', '', '', $values['license_fieldset']['license_uri']);
    $response = xml_form_elements_creative_commons_get_api_response($element['#name'], 'api_data', $current_query, $form_state);
    if ($response) {
      if ($response->getName() == 'error') {
        $cc_markup = t('None (form has errors).');
      }
      else {
        $cc_markup = $response->html->asXml();
        // Mend bogus <html> tags.
        $cc_markup = preg_replace('#^<html>#', '<div>', $cc_markup);
        $cc_markup = preg_replace('#</html>$#', '</div>', $cc_markup);
      }
    }
    else {
      $cc_markup = t('"@uri" (manually generated since Creative Commons API could not be reached).', array('@uri' => $values['license_fieldset']['license_uri']));
    }
  }
  // Generate markup element.
  $element['license_fieldset']['license_output'] = array(
    '#type' => 'item',
    '#markup' => '<strong>' . t('Selected license:') . '</strong> ' . $cc_markup,
    '#states' => array(
      'invisible' => array(
        ":input[name=\"{$element['#name']}[license_fieldset][license_type]\"]" => array('value' => 'none'),
      ),
    ),
  );

  // Element for graceful degradation w/out JS.
  $element['license_fieldset']['license_update'] = array(
    '#type' => 'button',
    '#value' => t('Update license'),
    '#limit_validation_errors' => array(),
    '#submit' => array('xml_form_elements_creative_commons_license_update_submit'),
    '#return_value' => 'update-license',
    '#prefix' => '<noscript>',
    '#suffix' => '</noscript>',
  );

  // Add validator.
  $element['#element_validate'] = array('xml_form_elements_creative_commons_validate');

  return $element;
}

/**
 * Submit handler for graceful no-JS degradation.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 */
function xml_form_elements_creative_commons_license_update_submit($form, &$form_state) {
  // If we have to manually update the form, force rebuild.
  $form_state['rebuild'] = TRUE;
}

/**
 * Validator for the `creative_commons` element.
 *
 * Validation is actually done elsewhere. We use this function only to set the
 * element's value for form builder.
 *
 * @param array $element
 *   The form element.
 * @param array $form_state
 *   The form state.
 * @param array $form
 *   The form.
 */
function xml_form_elements_creative_commons_validate(&$element, &$form_state, $form) {
  // Set the value in $form_state so that form builder can use it.
  $form_state['values'][$element['#name']] = is_string($element['#value']) ? $element['#value'] : $element['#value']['license_fieldset']['license_uri'];
}

/**
 * Get current value array for `creative_commons` form element.
 *
 * @param array $element
 *   The `creative_commons` form element.
 * @param array|NULL $input
 *   The user input (or NULL to retrieve defaults).
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The associative array representing the form values.
 */
function xml_form_elements_creative_commons_get_value_array_from_input(&$element, $input, &$form_state) {
  // Get defaults (from element or, if unset, from the module).
  $disabled_default = empty($element['#default_value']) ? XML_FORM_ELEMENTS_CREATIVE_COMMONS_DISABLED_DEFAULT : FALSE;
  $license_uri_default = !empty($element['#default_value']) ? $element['#default_value'] : XML_FORM_ELEMENTS_CREATIVE_COMMONS_LICENSE_URI_DEFAULT;
  list($license_default, $commercial_default, $derivatives_default, $version_default, $jurisdiction_default) = xml_form_elements_creative_commons_parse_uri($license_uri_default);
  // If disabled, we set license type and license URI accordingly.
  if ($disabled_default) {
    $license_default = 'none';
    $license_uri_default = '';
  }
  $error = FALSE;
  // Read input if set, otherwise populate with defaults.
  if (isset($input) && $input) {
    $license = $input['license_fieldset']['license_type'];
    // Disable if license is not recognized (or 'none').
    if (!in_array($license, array('zero', 'mark', 'by', 'manual'))) {
      $license = 'none';
      $disabled = TRUE;
    }
    else {
      $disabled = FALSE;
    }
    $commercial = $input['license_fieldset']['allow_commercial'];
    $derivatives = $input['license_fieldset']['allow_modifications'];
    $version = $input['license_fieldset']['license_version'];
    // Clean up version.
    if (!empty($version) && !preg_match("/^[0-9]\.[0-9]$/", $version)) {
      $error = TRUE;
      form_set_error($element['#name'] . '][license_fieldset][license_version', t("Invalid CC license version (leave empty for newest)."));
    }
    $jurisdiction = $input['license_fieldset']['license_jurisdiction'];
    // Read license URI only if in 'manual' mode, otherwise wipe it.
    if ($license == 'manual') {
      $license_uri = isset($input['license_fieldset']['license_uri']) ? $input['license_fieldset']['license_uri'] : '';
      // Clean up URI.
      if (!empty($license_uri) && (
        (strpos("#", $license_uri) !== FALSE) ||
        !preg_match("#^" . XML_FORM_ELEMENTS_CREATIVE_COMMONS_BASE_URL . "[a-z]+/[a-z+-]+/[0-9]\.[0-9]/([a-z]+/)?$#", $license_uri)
      )) {
        $error = TRUE;
        form_set_error($element['#name'] . '][license_fieldset][license_uri', t("Invalid license URI format."));
      }
    }
    else {
      $license_uri = '';
    }
  }
  else {
    $disabled = $disabled_default;
    $license = $license_default;
    $commercial = $commercial_default;
    $derivatives = $derivatives_default;
    $version = $version_default;
    $jurisdiction = $jurisdiction_default;
    $license_uri = $license_uri_default;
  }
  if (!in_array($license, array('none', 'manual'))) {
    // Check if there is a newer version unless user sets the URI manually.
    $current_query = xml_form_elements_creative_commons_make_value_array($license, $derivatives, $commercial, $jurisdiction, '', '');
    $response = xml_form_elements_creative_commons_get_api_response($element['#name'], 'api_data_get', $current_query, $form_state);
    if ($response) {
      $license_uri = (string) $response->{'license-uri'};
      $license_value_array = xml_form_elements_creative_commons_parse_uri($license_uri);
      $new_version = $license_value_array[3];
      if ($version != '') {
        if ((float) $new_version < (float) $version) {
          // The queried version is too high.
          $error = TRUE;
          form_set_error($element['#name'] . '][license_fieldset][license_version', t("CC license version not (yet) available (leave empty for newest)."));
        }
        elseif ((float) $new_version == (float) $version) {
          // The queried version is already the newest.
          $version = '';
        }
        else {
          // The queried version is lower and we need to replace it in the URI.
          $license_uri = str_replace($new_version, $version, $license_uri);
          if ($license_uri == XML_FORM_ELEMENTS_CREATIVE_COMMONS_LICENSE_URI_DEFAULT) {
            // Warn that the module default seems to be deprecated.
            watchdog('xml_form_elements', 'Creative Commons default license seems to be deprecated. Please announce the developers.', array(), WATCHDOG_WARNING);
          }
          $current_query = xml_form_elements_creative_commons_make_value_array('', '', '', '', '', $license_uri);
          $response = xml_form_elements_creative_commons_get_api_response($element['#name'], 'api_data', $current_query, $form_state);
          if ($response) {
            if ($response->getName() == 'error') {
              // The license URI is not recognized.
              $error = TRUE;
              form_set_error($element['#name'] . '][license_fieldset][license_version', t("CC license version does not exist (leave empty for newest)."));
            }
          }
          else {
            drupal_set_message(t("Warning: Could not reach Creative Commons API to validate your input."));
          }
        }
      }
      else {
        // Manually update cache for output.
        $current_query = xml_form_elements_creative_commons_make_value_array('', '', '', '', '', $license_uri);
        xml_form_elements_creative_commons_store_api_response($element['#name'], 'api_data', $current_query, $response, $form_state);
      }
    }
    else {
      // We have to build the license URI without the help of the API.
      drupal_set_message(t('Warning: Could not query the Creative Commons API. Creating license manually.'));
      $license_uri = xml_form_elements_creative_commons_value($commercial, $derivatives, $jurisdiction, FALSE, $version, $license);
    }
  }
  elseif ($license == 'manual' && $license_uri != '') {
    // Check manually entered license URI.
    $current_query = xml_form_elements_creative_commons_make_value_array('', '', '', '', '', $license_uri);
    $response = xml_form_elements_creative_commons_get_api_response($element['#name'], 'api_data', $current_query, $form_state);
    if ($response) {
      if ($response->getName() == 'error') {
        // The license URI is not recognized.
        $error = TRUE;
        form_set_error($element['#name'] . '][license_fieldset][license_uri', t("CC license URI seems to be invalid. Perhaps you would prefer to choose one automatically?"));
      }
    }
    else {
      // We could not reach the API.
      drupal_set_message(t("Warning: Could not reach Creative Commons API to validate your input."));
    }
  }
  // In no error occurred, we reset all the variables based on the URI,
  // except for the license type.
  if (!$error && $license_uri != '') {
    list($new_license, $commercial, $derivatives, $new_version, $jurisdiction) = xml_form_elements_creative_commons_parse_uri($license_uri);
    // Keep blank version only if not in manual mode.
    if ($license == 'manual' || $version != '') {
      $version = $new_version;
    }
    // Unset dummy variables.
    unset($new_license);
    unset($new_version);
  }
  // Force values into $form_state['input']
  $form_state['input'][$element['#name']] = xml_form_elements_creative_commons_make_value_array($license, $derivatives, $commercial, $jurisdiction, $version, $license_uri);
  // Return value array.
  return $form_state['input'][$element['#name']];
}

/**
 * Generate `creative_commons` value array.
 *
 * @param string $license
 *   The license type.
 * @param string $derivatives
 *   Modifications.
 * @param string $commercial
 *   Commercial.
 * @param string $jurisdiction
 *   Legal jurisdiction code.
 * @param string $version
 *   License version.
 * @param string $license_uri
 *   License URI.
 *
 * @return array
 *   The associative array representing the `creative_commons` input values.
 */
function xml_form_elements_creative_commons_make_value_array($license, $derivatives, $commercial, $jurisdiction, $version, $license_uri) {
  return array(
    'license_fieldset' => array(
      'license_type' => $license,
      'allow_modifications' => $derivatives,
      'allow_commercial' => $commercial,
      'license_jurisdiction' => $jurisdiction,
      'license_version' => $version,
      'license_uri' => $license_uri,
    ));
}

/**
 * Get API path for license.
 *
 * @param string $license
 *   A string containing the license ('zero', 'mark', 'by' or 'sampling+').
 *
 * @return string|FALSE
 *   The path where the license can be retrieved from using the API
 *   ('zero', 'mark', 'recombo' or 'standard'), or FALSE if $license was
 *   not recognized.
 */
function xml_form_elements_creative_commons_get_api_path($license) {
  $zero = array('zero');
  $mark = array('mark');
  $recombo = array('sampling+');
  $standard = array('by');
  $subpath = '';
  if (in_array($license, $zero)) {
    $subpath = 'zero';
  }
  elseif (in_array($license, $mark)) {
    $subpath = 'mark';
  }
  elseif (in_array($license, $recombo)) {
    $subpath = 'recombo';
  }
  elseif (in_array($license, $standard)) {
    $subpath = 'standard';
  }
  else {
    return FALSE;
  }
  return 'license/' . $subpath . '/get';
}

/**
 * Get path for license.
 *
 * @param string $license
 *   A string containing the license ('zero', 'mark', 'by' or 'sampling+').
 *
 * @return string|FALSE
 *   The path where the license is found ('licenses' or 'publicdomain'),
 *   or FALSE if $license was not recognized.
 */
function xml_form_elements_creative_commons_get_license_path($license) {
  $publicdomain = array('zero', 'mark');
  $licenses = array('by', 'sampling+');
  if (in_array($license, $publicdomain)) {
    return 'publicdomain';
  }
  elseif (in_array($license, $licenses)) {
    return 'licenses';
  }
  return FALSE;
}

/**
 * Parse Creative Commons URI.
 *
 * @param string $license_uri
 *   A string containing the CC license URI.
 *
 * @return array
 *   An array containing:
 *   - The license ('zero', 'mark', 'by' or 'sampling+')
 *   - Commercial use ('y', 'n'; 'y' if license not 'by')
 *   - Derivative creation ('y', 'n', 'sa'; 'y' if license not 'by')
 *   - Jurisdiction ('international' if international or not applicable)
 *   - Version
 */
function xml_form_elements_creative_commons_parse_uri($license_uri) {
  $creative_commons_base_url = XML_FORM_ELEMENTS_CREATIVE_COMMONS_BASE_URL;

  // Reversing this array facilitates value_callback mangling by community.
  $default_value_array = array_reverse(explode('/',
    str_replace($creative_commons_base_url, "", $license_uri)
  ));
  // Remove the license path.
  array_pop($default_value_array);
  $properties = explode('-', array_pop($default_value_array));
  $license = $properties[0];

  $commercial = 'y';
  $derivatives = 'y';

  foreach ($properties as $property) {
    switch ($property) {
      case 'by':
        break;

      case 'nc':
        $commercial = 'n';
        break;

      case 'nd':
        $derivatives = 'n';
        break;

      case 'sa':
        $derivatives = 'sa';
        break;
    }
  }

  $version = array_pop($default_value_array);

  $jurisdiction = array_pop($default_value_array);
  if (empty($jurisdiction)) {
    $jurisdiction = 'international';
  }

  return array($license, $commercial, $derivatives, $version, $jurisdiction);
}

/**
 * Ajax callback to render the CC license.
 *
 * @param array $form
 *   The form.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The markup to replace the target (wrapper) with.
 */
function xml_form_elements_creative_commons_ajax(&$form, &$form_state) {
  $parents = $form_state['triggering_element']['#parents'];
  array_pop($parents);
  $creative_commons_element = drupal_array_get_nested_value($form, $parents);

  return $creative_commons_element;
}

/**
 * Store an API response in storage.
 *
 * @param string $element_name
 *   The `creative_commons` element's name.
 * @param string $storage_folder
 *   The storage "subfolder".
 * @param array $values
 *   The associative value array that was used in the query
 *   (@see xml_form_elements_creative_commons_make_value_array())
 * @param SimpleXMLElement $response
 *   The API response.
 * @param array $form_state
 *   The form state.
 */
function xml_form_elements_creative_commons_store_api_response($element_name, $storage_folder, $values, $response, &$form_state) {
  if (!isset($form_state['storage']['xml_form_elements'][$element_name][$storage_folder])) {
    $form_state['storage']['xml_form_elements'][$element_name][$storage_folder] = array();
  }
  $form_state['storage']['xml_form_elements'][$element_name][$storage_folder]['query'] = $values;
  $form_state['storage']['xml_form_elements'][$element_name][$storage_folder]['response'] = ($response !== FALSE) ? $response->asXML() : FALSE;
}

/**
 * Perform API request and cache in storage.
 *
 * @param string $element_name
 *   The `creative_commons` element's name.
 * @param string $storage_folder
 *   The storage "subfolder".
 * @param array $values
 *   The associative value array to use in the query
 *   (@see xml_form_elements_creative_commons_make_value_array()).
 * @param array $form_state
 *   The form state.
 *
 * @return mixed
 *   SimpleXMLElement the return from the REST API.
 *   FALSE if failed request.
 */
function xml_form_elements_creative_commons_get_api_response($element_name, $storage_folder, $values, &$form_state) {
  if (isset($form_state['storage']['xml_form_elements'][$element_name][$storage_folder]['query']) &&
    $form_state['storage']['xml_form_elements'][$element_name][$storage_folder]['query'] == $values &&
    isset($form_state['storage']['xml_form_elements'][$element_name][$storage_folder]['response']) &&
    ($form_state['storage']['xml_form_elements'][$element_name][$storage_folder]['response'] !== FALSE)) {
    // Get response from storage.
    $response = simplexml_load_string($form_state['storage']['xml_form_elements'][$element_name][$storage_folder]['response'], 'SimpleXMLElement');
  }
  else {
    if (!empty($values['license_fieldset']['license_uri'])) {
      $response = xml_form_elements_get_creative_commons_from_uri($values['license_fieldset']['license_uri'], FALSE);
    }
    else {
      $response = xml_form_elements_get_creative_commons($values['license_fieldset']['allow_commercial'], $values['license_fieldset']['allow_modifications'], $values['license_fieldset']['license_jurisdiction'], FALSE, $values['license_fieldset']['license_version'], $values['license_fieldset']['license_type']);
    }
    // Cache response.
    xml_form_elements_creative_commons_store_api_response($element_name, $storage_folder, $values, $response, $form_state);
  }
  return $response;
}

/**
 * Ask the creative commons REST API for a license.
 *
 * Left as a function for use by community modules.
 *
 * @param string $commercial
 *   'y' to allow 'n' to disallow.
 * @param string $derivatives
 *   'y' to allow 'n' to disallow, 'sa' to share alike.
 * @param string $jurisdiction
 *   Legal jurisdiction code.
 * @param bool $disabled
 *   TRUE returns FALSE, FALSE (default) performs request.
 * @param string $version
 *   Specify (earlier) version (blank (default) for newest).
 * @param string $license
 *   Get licenses other than 'by' (default): 'zero', 'mark', 'sampling+'.
 *
 * @return mixed
 *   SimpleXMLElement the return from the REST API.
 *   FALSE if failed request.
 */
function xml_form_elements_get_creative_commons($commercial, $derivatives, $jurisdiction, $disabled = FALSE, $version = '', $license = 'by') {
  // Do not make request if disabled. Otherwise, either get an up-to-date
  // license if $version is empty, or get license details if $version is
  // specified.
  if ($disabled) {
    return FALSE;
  }
  elseif ($version != '') {
    return xml_form_elements_get_creative_commons_from_uri(xml_form_elements_creative_commons_value($commercial, $derivatives, $jurisdiction, $disabled, $version, $license));
  }
  $path = xml_form_elements_creative_commons_get_api_path($license);
  if (!$path) {
    $path = 'standard';
  }
  // $commercial, $derivatives and $jurisdiction only make sense for
  // attribution licenses.
  $response = drupal_http_request(url(
    XML_FORM_ELEMENTS_CREATIVE_COMMONS_API_URL . $path,
    ($license == 'by') ? array(
      'query' => array(
        'commercial' => $commercial,
        'derivatives' => $derivatives,
        'jurisdiction' => $jurisdiction,
      ),
    ) : array()
  ));
  if ($response->code != 200) {
    error_log("xml_form responded {$response->code}");
    return FALSE;
  }
  return simplexml_load_string($response->data, 'SimpleXMLElement');
}

/**
 * Ask the creative commons REST API for a license based on URI.
 *
 * Left as a function for use by community modules.
 *
 * @param string $license_uri
 *   A Creative Commonse license URI.
 * @param bool $disabled
 *   TRUE returns FALSE, FALSE (default) performs request.
 *
 * @return mixed
 *   SimpleXMLElement the return from the REST API.
 *   FALSE if failed request.
 */
function xml_form_elements_get_creative_commons_from_uri($license_uri, $disabled = FALSE) {
  // Do not make request if disabled.
  if ($disabled) {
    return FALSE;
  }
  $response = drupal_http_request(url(
    XML_FORM_ELEMENTS_CREATIVE_COMMONS_API_URL . 'details',
    array(
      'query' => array(
        'license-uri' => $license_uri,
      ),
    )
  ));
  if ($response->code != 200) {
    error_log("xml_form responded {$response->code}");
    return FALSE;
  }
  return simplexml_load_string($response->data, 'SimpleXMLElement');
}

/**
 * Gets the license URI value for the element.
 *
 * @param string $commercial
 *   'y' to allow 'n' to disallow.
 * @param string $derivatives
 *   'y' to allow 'n' to disallow.
 * @param string $jurisdiction
 *   Legal jurisdiction code.
 * @param bool $disabled
 *   TRUE returns empty string, FALSE (default) returns value.
 * @param string $enforced_version
 *   Override license version (empty string to take default).
 * @param string $license
 *   'by' (default), 'zero', 'mark'.
 *
 * @return string
 *   The license URI value of the element.
 *
 * Note: $commercial, $derivatives and $jurisdiction are not parsed for
 * licenses other than 'by', and we do not support the (deprecated!)
 * 'sampling+'.
 */
function xml_form_elements_creative_commons_value($commercial, $derivatives, $jurisdiction, $disabled = FALSE, $enforced_version = '', $license = 'by') {
  // Return empty string if disabled.
  if ($disabled) {
    return '';
  }
  $path = xml_form_elements_creative_commons_get_license_path($license);
  // Default to 'by' if $license cannot be handled otherwise.
  if (!$path || $license == 'sampling+') {
    // Either we do not know the license, or it is the deprecated one.
    $license = 'by';
    $path = 'licenses';
  }
  $arguments = '';
  if ($enforced_version !== '') {
    // Select specified version.
    $license_version = $enforced_version;
  }
  else {
    // Default to versions that should be available.
    $license_version = ($license == 'by') ? ($jurisdiction == 'international' ? 4.0 : '2.5') : '1.0';
  }
  if ($license == 'by') {
    if ($commercial == 'n') {
      $arguments = "$arguments-nc";
    }
    if ($derivatives == 'n') {
      $arguments = "$arguments-nd";
    }
    elseif ($derivatives == 'sa') {
      $arguments = "$arguments-sa";
    }
  }
  if ($license != 'by' || $jurisdiction == 'international') {
    // Not needed for international or 'zero', 'mark' licenses.
    $jurisdiction = '';
  }
  else {
    // If not international add the trailing slash.
    $jurisdiction .= '/';
  }
  return "http://creativecommons.org/licenses/$license$arguments/$license_version/$jurisdiction";
}
