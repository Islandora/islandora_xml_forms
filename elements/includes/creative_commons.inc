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

/**
 * Return license type options.
 *
 * @return array
 *   Associative array mapping types to descriptions.
 */
function xml_form_elements_creative_commons_license_type_options() {
  // Create the 'none' option.
  $none_option = array('none' => t('Do not output any license'));
  // Create the 'manual' option.
  $manual_option = array('manual' => t('Manually enter a license URI'));
  // Create license options.
  // (Note: One could also retrieve this from the API; see
  // https://api.creativecommons.org/docs/readme_15.html#locale-xx;
  // however, make sure to provide at least the 'by' option not to break code.)
  $license_options = array(
    'by' => t('Creative Commons Attribution'),
    'zero' => t('CC0 Universal (Public Domain)'),
    'mark' => t('Public Domain Mark'),
  );
  // Return options.
  return array_merge($none_option, $license_options, $manual_option);
}

/**
 * Return derivative options for 'by' licenses.
 *
 * @return array
 *   Associative array mapping options to descriptions.
 */
function xml_form_elements_creative_commons_derivative_options() {
  return array(
    'y' => t('Yes'),
    'n' => t('No'),
    'sa' => t('Yes, as long as others share alike'),
  );
}

/**
 * Return commercial options for 'by' licenses.
 *
 * @return array
 *   Associative array mapping options to descriptions.
 */
function xml_form_elements_creative_commons_commercial_options() {
  return array(
    'y' => t('Yes'),
    'n' => t('No'),
  );
}

/**
 * Return jurisdiction options for 'by' licenses.
 *
 * @return array
 *   Associative array mapping options to descriptions.
 */
function xml_form_elements_creative_commons_jurisdiction_options() {
  // Create 'international' option.
  $international_option = array('' => t('International'));
  // Create jursidiction options.
  // (List retrieved on 2018-04-28 from
  // https://api.creativecommons.org/rest/1.5/support/jurisdictions)
  $jurisdiction_options = array(
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
  return array_merge($international_option, $jurisdiction_options);
}

/**
 * Create a `creative_commons` form element.
 *
 * @param array $element
 *   The form element.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   The element definition.
 */
function xml_form_elements_creative_commons($element, &$form_state) {
  // Get an ID for ajax.
  if (isset($form_state['storage']['xml_form_elements'][$element['#name']]['license_fieldset_id'])) {
    $license_fieldset_id = $form_state['storage']['xml_form_elements'][$element['#name']]['license_fieldset_id'];
  }
  else {
    $license_fieldset_id = drupal_html_id('license_fieldset');
    $form_state['storage']['xml_form_elements'][$element['#name']]['license_fieldset_id'] = $license_fieldset_id;
  }

  // Store the license type options in a variable.
  $license_options = xml_form_elements_creative_commons_license_type_options();

  // Form elements that need to be populated.

  // The wrapping field set.
  $element['license_fieldset'] = array(
    '#id' => $license_fieldset_id,
    '#type' => 'fieldset',
    '#collapsed' => FALSE,
    '#collapsible' => TRUE,
    '#title' => t('License'),
  );

  // License type chooser.
  $element['license_fieldset']['license_type'] = array(
    '#type' => 'select',
    '#title' => t('Select a license type'),
    '#description' => t('Select "@option" to avoid outputting a creative commons license.', array('@option' => $license_options['none'])),
    '#empty_option' => $license_options['none'],
    '#empty_value' => 'none',
    '#options' => array_diff_key($license_options, array('none' => '')),
    '#ajax' => array(
      'wrapper' => $license_fieldset_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
    ),
  );

  // Allow modification chooser for 'by' licenses.
  $element['license_fieldset']['allow_derivatives'] = array(
    '#type' => 'select',
    '#title' => t('Allow modifications of your work?'),
    '#description' => t('Note that this is only relevant if you selected "@option" above.', array('@option' => $license_options['by'])),
    '#options' => xml_form_elements_creative_commons_derivative_options(),
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

  // Allow commercial chooser for 'by' licenses.
  $element['license_fieldset']['allow_commercial'] = array(
    '#type' => 'select',
    '#title' => t('Allow commercial uses of your work?'),
    '#description' => t('Note that this is only relevant if you selected "@option" above.', array('@option' => $license_options['by'])),
    '#options' => xml_form_elements_creative_commons_commercial_options(),
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

  // Jurisdiction chooser for 'by' licenses.
  $element['license_fieldset']['license_jurisdiction'] = array(
    '#type' => 'select',
    '#title' => t('License Jurisdiction'),
    '#options' => xml_form_elements_creative_commons_jurisdiction_options(),
    '#description' => t('Note that this is only relevant if you selected "@option" above.', array('@option' => $license_options['by'])),
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

  // Version overriding checkbox.
  $element['license_fieldset']['override_license_version'] = array(
    '#type' => 'checkbox',
    '#title' => t('Override License Version'),
    '#description' => t('Optionally set to a previous version.'),
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

  // Version text field.
  $element['license_fieldset']['license_version'] = array(
    '#type' => 'textfield',
    '#title' => t('License Version'),
    '#description' => t('Choose a license version (leave empty to get newest). Note that this only takes effect if you checked "@title" above.', array('@title' => $element['license_fieldset']['override_license_version']['#title'])),
    '#size' => 3,
    '#maxlength' => 3,
    '#ajax' => array(
      'wrapper' => $license_fieldset_id,
      'callback' => 'xml_form_elements_creative_commons_ajax',
      'keypress' => TRUE,
    ),
    '#states' => array(
      'invisible' => array(
        array(":input[name=\"{$element['#name']}[license_fieldset][license_type]\"]" => array(array('value' => 'none'), array('value' => 'manual'))),
        array(":input[name=\"{$element['#name']}[license_fieldset][override_license_version]\"]" => array('checked' => FALSE)),
      ),
    ),
  );

  // License URI text field.
  $element['license_fieldset']['license_uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Manually enter a license URI'),
    '#description' => t('Note that you need to set the license type to "@option" for this to take effect.', array('@option' => $license_options['manual'])),
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

  // License markup element.
  $element['license_fieldset']['license_output'] = array(
    '#type' => 'item',
    '#states' => array(
      'invisible' => array(
        ":input[name=\"{$element['#name']}[license_fieldset][license_type]\"]" => array('value' => 'none'),
      ),
    ),
  );

  // Update button for graceful degradation w/out JS.
  $element['license_fieldset']['license_update'] = array(
    '#type' => 'button',
    '#value' => t('Update license'),
    '#limit_validation_errors' => array(array($element['#name'])),
    '#submit' => array('xml_form_elements_creative_commons_license_update_submit'),
    '#return_value' => 'update-license',
    '#prefix' => '<noscript>',
    '#suffix' => '</noscript>',
  );

  // Add validator.
  $element['#element_validate'] = array('xml_form_elements_creative_commons_validate');

  // Add javascript.
  $element['#attached'] = array('js' => array(drupal_get_path('module', 'xml_form_elements') . '/js/creative_commons.js'));

  // If there is no input, or the temporary storage contains data (meaning
  // that the validator already exectued), populate the element with values.
  if (!isset($form_state['input'][$element['#name']]) || isset($form_state['temporary'][$element['#name']])) {
    _xml_form_elements_creative_commons_populate_element($element, $form_state);
  }

  // Get all form errors, attach 'error' class to erroneous elements and
  // instruct the user to inform the admin if the error is due to a
  // misconfiguration.
  // Note: the first part is necessary when the error occurred on first load
  // (but otherwise results in the 'error' class being attached twice).
  $form_errors = form_get_errors();
  if ($form_errors) {
    foreach (array_keys($element['license_fieldset']) as $name) {
      if (in_array($element['#name'] . '][license_fieldset][' . $name, array_keys($form_errors))) {
        $element['license_fieldset'][$name]['#attributes']['class'][] = 'error';
      }
    }
    if (empty($form_state['input'][$element['#name']])) {
      // There was no input, so the form was probably built from the previous
      // step. Hence, it is likely that we are displaying the site default,
      // so the admin should be warned.
      drupal_set_message(t('It is likely that the Creative Commons form element "@display_name" has been misconfigured. Please inform the site administrator.', array('@display_name' => $element['license_fieldset']['#title'])), 'error');
      watchdog('xml_form_elements', 'It is likely that the Creative Commons form element "@element_name" in "@form_name" has been misconfigured.', array('@element_name' => $element['#name'], '@form_name' => $form_state['storage'][FormStorage::STORAGE_ROOT]['name']), WATCHDOG_WARNING);
    }
  }

  return $element;
}

/**
 * Helper function to populate `creative_commons` elements.
 *
 * @param array $element
 *   The form element.
 * @param array $form_state
 *   The form state.
 *
 * Note: only call this after all the elements have been defined.
 */
function _xml_form_elements_creative_commons_populate_element(&$element, &$form_state) {
  // If this is not the first run, get markup and values from temporary
  // storage; otherwise call utility function to process user input, resp.
  // the default value, and store the data.
  if (isset($form_state['temporary'][$element['#name']])) {
    list($markup, $values) = $form_state['temporary'][$element['#name']];
  }
  else {
    list($markup, $values) = xml_form_elements_creative_commons_get_processed_values($element, $form_state);
    $form_state['temporary'][$element['#name']] = array($markup, $values);
  }

  // Update $form_state values.
  form_set_value($element, $values, $form_state);

  // Update element value.
  $element['#value'] = $values;

  // Populate elements' values and default values.
  foreach ($values['license_fieldset'] as $key => $val) {
    $element['license_fieldset'][$key]['#value'] = $val;
    $element['license_fieldset'][$key]['#default_value'] = $val;
  }

  // Set license output markup.
  $element['license_fieldset']['license_output']['#markup'] = $markup;

  // Set required elements when necessary.
  if ($element['#required']) {
    $element['license_fieldset']['license_type']['#required'] = ($values['license_fieldset']['license_type'] != 'manual');
    $element['license_fieldset']['license_uri']['#required'] = ($values['license_fieldset']['license_type'] == 'manual');
  }

  // Replace the description of the type chooser when none was chosen.
  if ($element['license_fieldset']['license_type']['#value'] == 'none') {
    $element['license_fieldset']['license_type']['#description'] = t('Note that your selection will still output a blank element in the resulting XML, so make sure to use a cleanup template to remove it.');
  }
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
 * @param array $element
 *   The form element.
 * @param array $form_state
 *   The form state.
 * @param array $form
 *   The form.
 */
function xml_form_elements_creative_commons_validate(&$element, &$form_state, $form) {
  // Make sure creative commons form errors get displayed.
  $validation_sections =& drupal_static('form_set_error' . ':limit_validation_errors');
  $original_validation_sections = $validation_sections;
  $validation_sections = array(array($element['#name']));
  // Populate element.
  _xml_form_elements_creative_commons_populate_element($element, $form_state);
  // If a license is required, error if none was chosen.
  if (($element['license_fieldset']['license_type']['#value'] == 'none') && $element['license_fieldset']['license_type']['#required']) {
    form_set_error($element['#name'] . '][license_fieldset][license_type', t('@element is required.', array('@element' => $element['license_fieldset']['license_type']['#title'])));
  }
  // If a license is required, error if no URI was given in manual mode.
  if (($element['license_fieldset']['license_type']['#value'] == 'manual') && $form_state['values'][$element['#name']]['license_fieldset']['license_uri'] == '' && $element['license_fieldset']['license_uri']['#required']) {
    form_set_error($element['#name'] . '][license_fieldset][license_uri', t('@element is required.', array('@element' => $element['license_fieldset']['license_uri']['#title'])));
  }
  // Re-set validation sections.
  $validation_sections = $original_validation_sections;
  // If user submits, set the scalar value in $form_state so that form builder
  // can use it.
  if ($form_state['submitted'] && !$form_state['rebuild']) {
    $form_state['values'][$element['#name']] = is_string($element['#value']) ? $element['#value'] : $element['#value']['license_fieldset']['license_uri'];
  }
}

/**
 * Generate `creative_commons` value array.
 *
 * @param string $license
 *   The license type.
 * @param string $derivatives
 *   Derivatives.
 * @param string $commercial
 *   Commercial.
 * @param string $jurisdiction
 *   Legal jurisdiction code.
 * @param bool $override_version
 *   TRUE to override version, FALSE to get newest.
 * @param string $version
 *   License version.
 * @param string $license_uri
 *   License URI.
 *
 * @return array
 *   The associative array representing the `creative_commons` input values.
 */
function xml_form_elements_creative_commons_make_value_array($license, $derivatives, $commercial, $jurisdiction, $override_version, $version, $license_uri) {
  return array(
    'license_fieldset' => array(
      'license_type' => $license,
      'allow_derivatives' => $derivatives,
      'allow_commercial' => $commercial,
      'license_jurisdiction' => $jurisdiction,
      'override_license_version' => $override_version,
      'license_version' => $version,
      'license_uri' => $license_uri,
    ));
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
 * Test the Creative Commons version format validity.
 *
 * @param string $license_version
 *   The license version.
 *
 * @return bool
 *   TRUE if format is valid, FALSE otherwise.
 */
function xml_form_elements_creative_commons_license_version_has_valid_format($license_version) {
  if (empty($license_version) || preg_match("#^[0-9]\.[0-9]$#", $license_version)) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Test the Creative Commons license URI format validity.
 *
 * @param string $license_uri
 *   The license URI.
 *
 * @return bool
 *   TRUE if format is valid, FALSE otherwise.
 */
function xml_form_elements_creative_commons_license_uri_has_valid_format($license_uri) {
  if (empty($license_uri) || (
    preg_match("#^" . XML_FORM_ELEMENTS_CREATIVE_COMMONS_BASE_URL . "[a-z]+/[a-z+-]+/[0-9]\.[0-9]/([a-z]+/)?$#", $license_uri) &&
    (!preg_match("#/by[-/]#", $license_uri) ||
      preg_match("#^" . XML_FORM_ELEMENTS_CREATIVE_COMMONS_BASE_URL . xml_form_elements_creative_commons_get_license_path('by') . "/by((-nc)?(-nd|-sa)?|(-nd|-sa)?(-nc)?)/#", $license_uri))
  )) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Quick input array clean-up.
 *
 * @param array $input
 *   The input array.
 *
 * @return array
 *   The cleaned-up input array.
 */
function xml_form_elements_creative_commons_cleanup_input($input) {
  // Define dummy element.
  $dummy_input = xml_form_elements_creative_commons_make_value_array(NULL, NULL, NULL, NULL, NULL, NULL, NULL);
  // Reset to dummy element when $input is null or does not have the expected
  // format.
  if (empty($input) || !isset($input['license_fieldset'])) {
    $input = $dummy_input;
  }
  // Nuke unexpected keys.
  $input = array_intersect_key($input, $dummy_input);
  $input['license_fieldset'] = array_intersect_key($input['license_fieldset'], $dummy_input['license_fieldset']);
  // Get all allowed options.
  $allowed_licenses = array_keys(xml_form_elements_creative_commons_license_type_options());
  $by_options = array();
  $by_options['allow_derivatives'] = array_keys(xml_form_elements_creative_commons_derivative_options());
  $by_options['allow_commercial'] = array_keys(xml_form_elements_creative_commons_commercial_options());
  $by_options['license_jurisdiction'] = array_keys(xml_form_elements_creative_commons_jurisdiction_options());
  // Check if queried license type exists, is a string and is allowed, reset to
  // 'none' otherwise.
  if (!isset($input['license_fieldset']['license_type']) ||
    !is_string($input['license_fieldset']['license_type']) ||
    !in_array($input['license_fieldset']['license_type'], $allowed_licenses)) {
    $input['license_fieldset']['license_type'] = 'none';
  }
  // Clean up selected 'by' options.
  if ($input['license_fieldset']['license_type'] != 'by') {
    // Reset derivative, commercial and jurisdiction options when not 'by'.
    foreach ($by_options as $key => $opts) {
      $input['license_fieldset'][$key] = $opts[0];
    }
  }
  else {
    // Check if all selected options exist, are strings and allowed, reset them
    // otherwise.
    foreach ($by_options as $key => $opts) {
      if (!isset($input['license_fieldset'][$key]) ||
        !is_string($input['license_fieldset'][$key]) ||
        !in_array($input['license_fieldset'][$key], $opts)) {
        $input['license_fieldset'][$key] = $opts[0];
      }
    }
  }
  // If version override is integer, make it a string.
  if (isset($input['license_fieldset']['override_license_version']) &&
    is_int($input['license_fieldset']['override_license_version'])) {
    $input['license_fieldset']['override_license_version'] = (string) $input['license_fieldset']['override_license_version'];
  }
  // Clean up version override.
  if (!isset($input['license_fieldset']['override_license_version']) ||
    !is_string($input['license_fieldset']['override_license_version']) ||
    !in_array($input['license_fieldset']['override_license_version'], array('0', '1')) ||
    in_array($input['license_fieldset']['license_type'], array('none', 'manual'))) {
    $input['license_fieldset']['override_license_version'] = '0';
  }
  // Clean up version.
  if (!isset($input['license_fieldset']['license_version']) ||
    !is_string($input['license_fieldset']['license_version']) ||
    ($input['license_fieldset']['override_license_version'] != '1') ||
    in_array($input['license_fieldset']['license_type'], array('none', 'manual'))) {
    $input['license_fieldset']['license_version'] = '';
  }
  // Empty license URI if unset, not a string or not in manual mode.
  if (!isset($input['license_fieldset']['license_uri']) ||
    !is_string($input['license_fieldset']['license_uri']) ||
    ($input['license_fieldset']['license_type'] != 'manual')) {
    $input['license_fieldset']['license_uri'] = '';
  }
  // Return cleaned-up user input.
  return $input;
}

/**
 * Create CC element value array from license URI.
 *
 * @param string $license_uri
 *   The CC license URI.
 *
 * @return array
 *   The associative value array corresponding to the URI.
 */
function xml_form_elements_creative_commons_get_values_from_uri($license_uri) {
  // Get supported licenses (assuming 'by', as well as the special values
  // 'none' and 'manual').
  $supported_licenses = array_diff(array_keys(xml_form_elements_creative_commons_license_type_options()), array('none', 'manual'));

  // Make a default array based on the URI.
  $values = xml_form_elements_creative_commons_cleanup_input(
    xml_form_elements_creative_commons_make_value_array('manual', '', '', '', '0', '', $license_uri)
  );

  // If no license URI is set, default to no output.
  if (empty($license_uri)) {
    $values['license_fieldset']['license_type'] = 'none';
  }

  // Parse URI only if it is non-empty and valid.
  if (!empty($license_uri) && xml_form_elements_creative_commons_license_uri_has_valid_format($license_uri)) {
    // Reversing this array facilitates value_callback mangling by community.
    $uri_value_array = array_reverse(explode('/',
      preg_replace('#^' . XML_FORM_ELEMENTS_CREATIVE_COMMONS_BASE_URL . '#', '', $license_uri)
    ));
    // Get the license path.
    $path = array_pop($uri_value_array);
    // Get license with properties.
    $properties = explode('-', array_pop($uri_value_array));
    // If the license is supported and has correct path, set the type.
    if (in_array($properties[0], $supported_licenses) && (xml_form_elements_creative_commons_get_license_path($properties[0]) === $path)) {
      $values['license_fieldset']['license_type'] = $properties[0];
    }
    // Parse 'by' options; otherwise reset to manual if those options exist.
    if ($properties[0] == 'by') {
      foreach ($properties as $property) {
        switch ($property) {
          case 'by':
            break;

          case 'nc':
            $values['license_fieldset']['allow_commercial'] = 'n';
            break;

          case 'nd':
            $values['license_fieldset']['allow_derivatives'] = 'n';
            break;

          case 'sa':
            $values['license_fieldset']['allow_derivatives'] = 'sa';
            break;
        }
      }
    }
    elseif (count($properties) != 1) {
      $values['license_fieldset']['license_type'] = 'manual';
    }
    // Since we do not know if version is newest, we set version overriding.
    $values['license_fieldset']['override_license_version'] = '1';
    // Get version.
    $values['license_fieldset']['license_version'] = array_pop($uri_value_array);
    // Get jurisdiction.
    $values['license_fieldset']['license_jurisdiction'] = array_pop($uri_value_array);
    // Clean up jurisdiction.
    $supported_jurisdictions = array_keys(xml_form_elements_creative_commons_jurisdiction_options());
    if (!in_array($values['license_fieldset']['license_jurisdiction'], $supported_jurisdictions)) {
      $values['license_fieldset']['license_jurisdiction'] = $supported_jurisdictions[0];
    }
  }

  // Return the parsed values including the license URI.
  return $values;
}

/**
 * Create license URI from CC element value array.
 *
 * @param array $values
 *   The associative value array corresponding to the URI.
 *
 * @return string
 *   The CC license URI.
 */
function xml_form_elements_creative_commons_get_uri_from_values($values) {
  // Return empty string if license type 'none' was specified.
  if ($values['license_fieldset']['license_type'] == 'none') {
    return '';
  }
  // Return the specified license URI when in manual mode.
  if ($values['license_fieldset']['license_type'] == 'manual') {
    return $values['license_fieldset']['license_uri'];
  }
  // Get the specified license.
  $license = $values['license_fieldset']['license_type'];
  // Get supported licenses (assuming 'by', as well as the special values
  // 'none' and 'manual').
  $supported_licenses = array_diff(array_keys(xml_form_elements_creative_commons_license_type_options()), array('none', 'manual'));
  // Get the path for the specified license.
  $path = xml_form_elements_creative_commons_get_license_path($license);
  // Return the specified license URI if the license or its path are unknown.
  if (!in_array($license, $supported_licenses) || $path === FALSE) {
    return $values['license_fieldset']['license_uri'];
  }
  // Get the license arguments for 'by'.
  $arguments = '';
  if ($license == 'by') {
    if ($values['license_fieldset']['allow_commercial'] == 'n') {
      $arguments = "$arguments-nc";
    }
    if ($values['license_fieldset']['allow_derivatives'] == 'n') {
      $arguments = "$arguments-nd";
    }
    elseif ($values['license_fieldset']['allow_derivatives'] == 'sa') {
      $arguments = "$arguments-sa";
    }
  }
  // Get the specified jurisdiction.
  $jurisdiction = $values['license_fieldset']['license_jurisdiction'];
  // Clean up jurisdiction if non-empty.
  if (!empty($jurisdiction)) {
    // Get the supported jurisdictions (assuming the first one should map
    // to '').
    $supported_jurisdictions = array_keys(xml_form_elements_creative_commons_jurisdiction_options());
    // Empty jurisdiction if not supported or first (default) choice; otherwise
    // append a slash.
    if (!in_array($jurisdiction, $supported_jurisdictions) || $jurisdiction == $supported_jurisdictions[0]) {
      $jurisdiction = '';
    }
    else {
      $jurisdiction .= '/';
    }
  }
  // Get the specified version.
  $version = $values['license_fieldset']['license_version'];
  // If no version was specified, default to versions that should be available.
  if (empty($version)) {
    $version = ($license == 'by') ? ($jurisdiction == '' ? '4.0' : '2.5') : '1.0';
  }
  // Return the license URI.
  return XML_FORM_ELEMENTS_CREATIVE_COMMONS_BASE_URL . "$path/$license$arguments/$version/$jurisdiction";
}

/**
 * Get markup and values for `creative_commons` form element.
 *
 * @param array $element
 *   The `creative_commons` form element.
 * @param array $form_state
 *   The form state.
 *
 * @return array
 *   An array containing:
 *   - the markup for the chosen license;
 *   - the associative value array for that license.
 */
function xml_form_elements_creative_commons_get_processed_values(&$element, &$form_state) {
  // Get current form values or defaults.
  $values = isset($form_state['input'][$element['#name']]) ?
    xml_form_elements_creative_commons_cleanup_input(is_string($form_state['values'][$element['#name']]) ?
      xml_form_elements_creative_commons_get_values_from_uri($form_state['values'][$element['#name']]) :
      $form_state['values'][$element['#name']]) :
    xml_form_elements_creative_commons_get_values_from_uri(isset($element['#default_value']) ?
      $element['#default_value'] :
      '');

  // Set prefix for license HTML markup.
  $cc_prefix = '<strong>' . t('Selected license:') . '</strong> ';

  // Define none markup.
  $none = t('None.');

  // Create empty values.
  $empty_values = xml_form_elements_creative_commons_get_values_from_uri('');

  // If some values are missing, return filled-up value array.
  $missing_values = array('license_fieldset' => array_diff_key($empty_values['license_fieldset'], $values['license_fieldset']));
  if (!isset($values['license_fieldset']) || !empty($missing_values['license_fieldset'])) {
    $values['license_fieldset'] = array_merge($empty_values['license_fieldset'], isset($values['license_fieldset']) ? $values['license_fieldset'] : array());
    drupal_set_message(t('An internal error occurred while processing the Creative Commons form element "@display_name" (unexpected data loss). Please inform the site administrator.', array("@display_name" => $element['license_fieldset']['#title'])), 'error');
    watchdog('xml_form_elements', 'Unexpected data loss when processing element "@element_name" in "@form_name". Probably you just found a bug.', array('@element_name' => $element['#name'], '@form_name' => $form_state['storage'][FormStorage::STORAGE_ROOT]['name']), WATCHDOG_ERROR);
    return array($cc_prefix . t('None (internal error).'), $values);
  }

  // Remember manual mode.
  $manual = ($values['license_fieldset']['license_type'] == 'manual');

  // If in manual mode, try to guess the license.
  if ($manual) {
    $values = xml_form_elements_creative_commons_get_values_from_uri($values['license_fieldset']['license_uri']);
  }

  // If in manual mode with no license URI set, set to empty.
  if ($manual && empty($values['license_fieldset']['license_uri'])) {
    $values = $empty_values;
  }

  // If no output was requested, return empty.
  if ($values['license_fieldset']['license_type'] == 'none') {
    // If in manual mode, re-set mode.
    if ($manual) {
      $empty_values['license_fieldset']['license_type'] = 'manual';
    }
    return array($cc_prefix . $none, $empty_values);
  }

  // Know if we have errors.
  $error = FALSE;

  // Define the element for version errors.
  $error_element = $element['#name'] . ($manual ? '][license_fieldset][license_uri' : '][license_fieldset][license_version');

  // Define none with form errors markup.
  $none_err = t('None (form has errors).');

  // Validate the initial version.
  if (!xml_form_elements_creative_commons_license_version_has_valid_format($values['license_fieldset']['license_version'])) {
    $error = TRUE;
    form_set_error($element['#name'] . '][license_fieldset][license_version', t('Invalid CC license version format.'));
  }

  // Validate the initial license URI.
  if (!xml_form_elements_creative_commons_license_uri_has_valid_format($values['license_fieldset']['license_uri'])) {
    $error = TRUE;
    $test_uri = preg_replace('#^https?#', 'http', preg_replace('#/?$#', '/', $values['license_fieldset']['license_uri'], 1));
    if (xml_form_elements_creative_commons_license_uri_has_valid_format($test_uri)) {
      form_set_error($element['#name'] . '][license_fieldset][license_uri', t('The CC license URI needs to start with "http://" and end with a slash "/".'));
    }
    else {
      form_set_error($element['#name'] . '][license_fieldset][license_uri', t('Invalid CC license URI format.'));
    }
  }

  // If there was an error, return current values.
  if ($error) {
    // If in manual mode, re-set mode.
    if ($manual) {
      $values['license_fieldset']['license_type'] = 'manual';
    }
    return array($cc_prefix . $none_err, $values);
  }

  // Remember if _user_ wants to override version.
  $override_version = isset($form_state['input'][$element['#name']]) &&
    ((bool) ((int) $values['license_fieldset']['override_license_version']));

  // If not in (pure) manual mode (i.e. we know the license), get the newest.
  if ($values['license_fieldset']['license_type'] != 'manual') {
    // Get the appropriate API path for the requested license type.
    $api_path = xml_form_elements_creative_commons_get_api_path($values['license_fieldset']['license_type']);
    // Default to standard for unknown licenses.
    if (!$api_path) {
      $api_path = 'license/standard/get';
    }
    // Prepare query options.
    $api_options = array();
    if ($values['license_fieldset']['license_type'] == 'by') {
      $api_options = array(
        'commercial' => $values['license_fieldset']['allow_commercial'],
        'derivatives' => $values['license_fieldset']['allow_derivatives'],
        'jurisdiction' => $values['license_fieldset']['license_jurisdiction'],
      );
    }
    $response = xml_form_elements_creative_commons_query_api_get($api_path, $api_options);
    if ($response && $response->getName() != 'error') {
      // Get the newest license URI and get its version.
      $new_license_uri = (string) $response->{'license-uri'};
      $new_values = xml_form_elements_creative_commons_get_values_from_uri($new_license_uri);
      $new_version = $new_values['license_fieldset']['license_version'];
      // Get current version.
      $version = $values['license_fieldset']['license_version'];
      // Check version validity, if overriding.
      if (($values['license_fieldset']['override_license_version'] == '1') && $version != '' && ((float) $new_version != (float) $version)) {
        // Update the target license URI's version.
        $new_license_uri = str_replace($new_version, $version, $new_license_uri);
        // Compare versions.
        if ((float) $new_version < (float) $version) {
          // The specified version is too high.
          $error = TRUE;
          form_set_error($error_element, t("CC license version not (yet) available."));
        }
      }
      else {
        // We queried the newest version, or the specified version is already
        // the newest. We update the version and switch to non-overriding.
        $values['license_fieldset']['license_version'] = $new_version;
        $values['license_fieldset']['override_license_version'] = '0';
      }
    }
    else {
      // We have to build the license URI without the help of the API.
      if (!$response) {
        drupal_set_message(t("Could not query the Creative Commons API. Creating license URI manually."), 'warning');
      }
      elseif ($response->getName() == 'error') {
        drupal_set_message(t("The Creative Commons API unexpectedly returned an error. Creating license URI manually."), 'warning');
      }
      $new_license_uri = xml_form_elements_creative_commons_get_uri_from_values($values);
    }
    // If we had already a URI in manual mode and it differs from the new one,
    // reset to manual now; otherwise store the new license URI.
    if ($manual && (!empty($values['license_fieldset']['license_uri']) && $values['license_fieldset']['license_uri'] !== $new_license_uri)) {
      $values['license_fieldset']['license_type'] = 'manual';
    }
    else {
      $values['license_fieldset']['license_uri'] = $new_license_uri;
    }
    // Reset version overriding in automatic mode, if _user_ requested it.
    if (!$manual && $override_version) {
      $values['license_fieldset']['override_license_version'] = '1';
    }
  }

  // If there was an error, return current values.
  if ($error) {
    // If in manual mode, re-set mode.
    if ($manual) {
      $values['license_fieldset']['license_type'] = 'manual';
    }
    return array($cc_prefix . $none_err, $values);
  }

  // If in manual mode, or if the previous request was unsuccessful, or if a
  // different version was specified, query the API for validation for
  // non-empty URIs.
  if ((($values['license_fieldset']['license_type'] == 'manual') || !isset($new_version) || ($version != '' && ((float) $version != (float) $new_version)))
    && !empty($values['license_fieldset']['license_uri'])) {
    // Get the license details for the specified license URI.
    $response = xml_form_elements_creative_commons_query_api_details($values['license_fieldset']['license_uri']);
    if ($response) {
      if ($response->getName() == 'error') {
        // The license URI is not recognized.
        $error = TRUE;
        if ($values['license_fieldset']['license_type'] == 'manual') {
          // If only the jurisdiction was unknown, alert the user; otherwise
          // suggest choosing the license automatically.
          if (strpos($values['license_fieldset']['license_uri'], $new_license_uri) === 0) {
            form_set_error($element['#name'] . '][license_fieldset][license_uri', t("CC license jurisdiction unknown or not applicable for this license."));
          }
          else {
            form_set_error($element['#name'] . '][license_fieldset][license_uri', t("CC license URI seems to be invalid. Perhaps you would prefer to choose one automatically?"));
          }
        }
        else {
          form_set_error($error_element, t("CC license version does not seem to exist."));
        }
      }
    }
    else {
      // We could not reach the API.
      drupal_set_message(t("Could not reach Creative Commons API to validate your input."), 'warning');
    }
  }

  // If there was an error, return current values.
  if ($error) {
    // If in manual mode, re-set mode.
    if ($manual) {
      $values['license_fieldset']['license_type'] = 'manual';
    }
    return array($cc_prefix . $none_err, $values);
  }

  // Prepare the license markup.
  $cc_markup = $none;
  // If the license URI is set (as expected), get the API markup.
  if (!empty($values['license_fieldset']['license_uri'])) {
    if (!$response) {
      $cc_markup = t('"@uri" (manually generated since Creative Commons API could not be reached).', array('@uri' => $values['license_fieldset']['license_uri']));
    }
    elseif ($response->getName() == 'error') {
      // We should never be here.
      drupal_set_message(t('An unexpected error occurred with the Creative Commons form element "@display_name". Please inform the site administrator.', array('@display_name' => $element['license_fieldset']['#title'])), 'error');
      watchdog('xml_form_elements', 'The Creative Commons API returned an error on "@uri" that should have been cought already. This happened while processing element "@element_name" in "@form_name". Probably you just found a bug.', array(
        '@uri' => $element['license_fieldset']['license_uri'],
        '@element_name' => $element['#name'],
        '@form_name' => $form_state['storage'][FormStorage::STORAGE_ROOT]['name'],
      ), WATCHDOG_ERROR);
    }
    else {
      // Get the HTML markup for the selected license.
      $cc_markup = $response->html->asXml();
      // Mend bogus <html> tags.
      $cc_markup = preg_replace('#^<html>#', '<div>', $cc_markup);
      $cc_markup = preg_replace('#</html>$#', '</div>', $cc_markup);
    }
  }

  // If in manual mode, re-set mode.
  if ($manual) {
    $values['license_fieldset']['license_type'] = 'manual';
  }

  // Return current values.
  return array($cc_prefix . $cc_markup, $values);
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
 * Get a license from the CC API.
 *
 * @param string $path
 *   The path from which to query.
 * @param array $options
 *   The http query array to attach to the query.
 *
 * @return SimpleXMLElement|FALSE
 *   The response from the REST API as a SimpleXMLElement, or FALSE if request
 *   failed.
 *
 * Note that there may be a bug in the CC API. Querying, e.g., 'fi'
 * jurisdiction, 'sa' derivatives and 'y' or 'n' commercial, results in a
 * CC-BY-SA/CC-BY-NC-SA 4.0 International license, instead of the corresponding
 * 1.0 Finland version (which exists and is not deprecated).
 * This behaviour, unfortunately, breaks our logic, so users will have to
 * insert those licenses manually, and admins cannot set them as default.
 */
function xml_form_elements_creative_commons_query_api_get($path, $options = array()) {
  return _xml_form_elements_creative_commons_query_api($path, $options);
}

/**
 * Get license details from the CC API.
 *
 * @param string $license_uri
 *   The license URI for which to query details.
 *
 * @return SimpleXMLElement|FALSE
 *   The response from the REST API as a SimpleXMLElement, or FALSE if request
 *   failed.
 */
function xml_form_elements_creative_commons_query_api_details($license_uri) {
  return _xml_form_elements_creative_commons_query_api('details', array('license-uri' => $license_uri));
}

/**
 * Helper function to query the CC API.
 *
 * @param string $path
 *   The API path to query.
 * @param array $query
 *   The query.
 *
 * @return SimpleXMLElement|FALSE
 *   The response from the REST API as a SimpleXMLElement, or FALSE if request
 *   failed.
 */
function _xml_form_elements_creative_commons_query_api($path, $query = array()) {
  $response = drupal_http_request(url(XML_FORM_ELEMENTS_CREATIVE_COMMONS_API_URL . $path, array('query' => $query)));
  if ($response->code != 200) {
    watchdog('xml_form_elements', 'The Creative Commons API responded with code %code.', array('%code' => $response->code), WATCHDOG_WARNING);
    return FALSE;
  }
  return simplexml_load_string($response->data, 'SimpleXMLElement');
}

/**
 * Ask the creative commons REST API for a 'by' license.
 *
 * Left as a function for use by community modules.
 *
 * @deprecated As of release 7.x-1.11, this function remains only for backwards
 *   compatibilty. New code should use
 *   xml_form_elements_creative_commons_query_api_get() instead.
 *   Update your code, as this function might be removed.
 *
 * @param string $commercial
 *   'y' to allow, 'n' to disallow.
 * @param string $derivatives
 *   'y' to allow, 'n' to disallow, 'sa' to share alike.
 * @param string $jurisdiction
 *   Legal jurisdiction code.
 * @param bool $disabled
 *   If TRUE, return FALSE; otherwise perform query.
 *
 * @return mixed
 *   SimpleXMLElement the return from the REST API.
 *   FALSE if failed request or if `$disabled` evaluates to `TRUE`.
 */
function xml_form_elements_get_creative_commons($commercial, $derivatives, $jurisdiction, $disabled = FALSE) {
  // Show deprecated message.
  module_load_include('inc', 'islandora', 'includes/utilities');
  $deprecated_message = islandora_deprecated('7.x-1.11', t('Use @newfunction() instead.', array('@newfunction' => 'xml_form_elements_creative_commons_query_api_get')));
  trigger_error(filter_xss($deprecated_message), E_USER_DEPRECATED);
  // Return FALSE if disabled.
  if ($disabled) {
    return FALSE;
  }
  // This function only pertains to 'by' licenses.
  $api_path = 'license/standard/get';
  // Prepare query options.
  $api_options = array(
    'commercial' => $commercial,
    'derivatives' => $derivatives,
    'jurisdiction' => $jurisdiction,
  );
  // Return API response.
  return xml_form_elements_creative_commons_query_api_get($api_path, $api_options);
}

/**
 * Gets the 'by' license URI for the element.
 *
 * @deprecated As of release 7.x-1.11, this function remains only for backwards
 *   compatibilty. New code should use
 *   xml_form_elements_creative_commons_get_uri_from_values() instead.
 *   Update your code, as this function might be removed.
 *
 * @param string $commercial
 *   'y' to allow, 'n' to disallow.
 * @param string $derivatives
 *   'y' to allow, 'n' to disallow, 'sa' to share alike.
 * @param string $jurisdiction
 *   Legal jurisdiction code.
 * @param bool $disabled
 *   If TRUE, return empty string; otherwise generate the URI.
 *
 * @return string
 *   The value of the element, or the empty string `''` if `$disabled`
 *   evaluates to `TRUE`.
 */
function xml_form_elements_creative_commons_value($commercial, $derivatives, $jurisdiction, $disabled = FALSE) {
  // Show deprecated message.
  module_load_include('inc', 'islandora', 'includes/utilities');
  $deprecated_message = islandora_deprecated('7.x-1.11', t('Use @newfunction() instead.', array('@newfunction' => 'xml_form_elements_creative_commons_get_uri_from_values')));
  trigger_error(filter_xss($deprecated_message), E_USER_DEPRECATED);
  // Return empty string if disabled.
  if ($disabled) {
    return '';
  }
  // Generate form element values based on parameters.
  $values = xml_form_elements_creative_commons_make_value_array('by', $derivatives, $commercial, $jurisdiction, '', '');
  // Return the license URI.
  return xml_form_elements_creative_commons_get_uri_from_values($values);
}
