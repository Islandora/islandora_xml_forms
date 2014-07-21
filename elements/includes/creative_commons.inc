<?php

/**
 * @file
 * Code for creative commons form elements.
 */


class CreativeCommons {

  public static $modificationOptions = array(
    'y' => 'Yes',
    'n' => 'No',
    'sa' => 'Yes, as long as others share alike',
  );

  public static $countries = array(
    'international' => 'International',
    'ar' => 'Argentina',
    'au' => 'Australia',
    'at' => 'Austria',
    'be' => 'Belgium',
    'br' => 'Brazil',
    'bg' => 'Bulgaria',
    'ca' => 'Canada',
    'cl' => 'Chile',
    'cn' => 'China Mainland',
    'co' => 'Colombia',
    'cr' => 'Costa Rica',
    'hr' => 'Croatia',
    'cz' => 'Czech Republic',
    'dk' => 'Denmark',
    'ec' => 'Ecuador',
    'eg' => 'Egypt',
    'ee' => 'Estonia',
    'fi' => 'Finland',
    'fr' => 'France',
    'de' => 'Germany',
    'gr' => 'Greece',
    'gt' => 'Guatemala',
    'hk' => 'Hong kong',
    'hu' => 'Hungary',
    'in' => 'India',
    'ie' => 'Ireland',
    'il' => 'Israel',
    'it' => 'Italy',
    'jp' => 'Japan',
    'lu' => 'Luxembourg',
    'mk' => 'Macedonia',
    'my' => 'Malaysia',
    'mt' => 'Malta',
    'mx' => 'Mexico',
    'nl' => 'Netherlands',
    'nz' => 'New Zealand',
    'no' => 'Norway',
    'pe' => 'Peru',
    'ph' => 'Philippines',
    'pl' => 'Poland',
    'pt' => 'Portugal',
    'pr' => 'Puerto Rico',
    'ro' => 'Romania',
    'rs' => 'Serbia',
    'sg' => 'Singapore',
    'si' => 'Slovenia',
    'za' => 'South Africa',
    'kr' => 'South Korea',
    'es' => 'Spain',
    'se' => 'Sweden',
    'ch' => 'Switzerland',
    'tw' => 'Taiwan',
    'th' => 'Thailand',
    'uk' => 'UK: England & Wales',
    'scotland' => 'Uk: Scotland',
    'ug' => 'Uganda',
    'us' => 'United states',
    'vn' => 'Vietnam',
  );

  public static $commercialOptions = array(
    'y' => 'Yes',
    'n' => 'No',
  );
  /**
   * Creates a creative_commons form element.
   *
   * @param array $form_state
   *   The form state.
   *
   * @return array
   *   The element definition.
   */
  public static function xmlFormElementsCreativeCommons($element, &$form_state) {

    // Get an ID for ajax.
    if (isset($form_state['storage']['xml_form_elements'][$element['#name']]['license_output_id'])) {
      $license_output_id = $form_state['storage']['xml_form_elements'][$element['#name']]['license_output_id'];
    }
    else {
      // $license_output_id = drupal_html_id('license_output');
      $license_output_id = 'license_output';
      $form_state['storage']['xml_form_elements'][$element['#name']]['license_output_id'] = $license_output_id;
    }

    // Figure out license state.
    if (!isset($form_state['input'][$element['#name']]) && isset($element['#default_value'])) {
      // Reversing this array facilitates value_callback mangling by community.
      $default_value_array = array_reverse(explode('/', $element['#default_value']));
      // If there are only 3 elements, country is blank = international.
      if (count($default_value_array) == 3) {
        $default_keys = array('property' => 2);
      }
      else {
        $default_keys = array('jurisdiction' => 1, 'property' => 3);
      }
      $properties = explode('-', $default_value_array[$default_keys['property']]);

      $commercial = variable_get('xml_form_builder_cc_default_allow_commercial', 'y');
      $derivatives = variable_get('xml_form_builder_cc_default_allow_derivatives', 'y');

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

      if (array_key_exists('jurisdiction', $default_keys)) {
        $jurisdiction = empty($default_value_array[$default_keys['jurisdiction']]) ? variable_get('xml_form_builder_cc_default_jurisdiction', 'international') : $default_value_array[$default_keys['jurisdiction']];
      }
      else {
        $jurisdiction = 'international';
      }
    }
    else {
      $derivatives = isset($form_state['input'][$element['#name']]['license_fieldset']['allow_modifications']) ? $form_state['input'][$element['#name']]['license_fieldset']['allow_modifications'] : variable_get('xml_form_builder_cc_default_allow_derivatives', 'y');
      $commercial = isset($form_state['input'][$element['#name']]['license_fieldset']['allow_commercial']) ? $form_state['input'][$element['#name']]['license_fieldset']['allow_commercial'] : variable_get('xml_form_builder_cc_default_allow_commercial', 'y');
      $jurisdiction = isset($form_state['input'][$element['#name']]['license_fieldset']['license_jurisdiction']) ? $form_state['input'][$element['#name']]['license_fieldset']['license_jurisdiction'] : variable_get('xml_form_builder_cc_default_jurisdiction', 'international');
    }

    // Fix for static variables.
    $modification_options = array_map(array('CreativeCommons', 'xmlFormBuilderFixVariables'), CreativeCommons::$modificationOptions);
    $commercial_options = array_map(array('CreativeCommons', 'xmlFormBuilderFixVariables'), CreativeCommons::$commercialOptions);
    $countries = array_map(array('CreativeCommons', 'xmlFormBuilderFixVariables'), CreativeCommons::$countries);

    // Form elements.
    $element['license_fieldset'] = array(
      '#type' => 'fieldset',
      '#collapsed' => FALSE,
      '#collapsible' => TRUE,
      '#title' => t('License'),
    );

    $element['license_fieldset']['allow_modifications'] = array(
      '#type' => 'select',
      '#title' => t('Allow modifications of your work?'),
      '#options' => $modification_options,
      '#default_value' => $derivatives,
      '#ajax' => array(
        'wrapper' => $license_output_id,
        'callback' => 'xml_form_elements_creative_commons_ajax',
      ),
    );

    $element['license_fieldset']['allow_commercial'] = array(
      '#type' => 'select',
      '#title' => t('Allow commercial uses of your work?'),
      '#options' => $commercial_options,
      '#default_value' => $commercial,
      '#ajax' => array(
        'wrapper' => $license_output_id,
        'callback' => 'xml_form_elements_creative_commons_ajax',
      ),
    );

    $element['license_fieldset']['license_jurisdiction'] = array(
      '#type' => 'select',
      '#title' => t('License Jurisdiction'),
      '#options' => $countries,
      '#default_value' => $jurisdiction,
      '#ajax' => array(
        'wrapper' => $license_output_id,
        'callback' => 'xml_form_elements_creative_commons_ajax',
      ),
    );
    // Value gets populated if default value is populated.
    if (current_path() == 'system/ajax' || !$element['#value'] ||
      (isset($element['#default_value']) && $element['#value'] == $element['#default_value'])) {
      $element['#tree'] = TRUE;
    }
    else {
      // I win form builder.
      $element['#tree'] = FALSE;
    }

    $response = CreativeCommons::xmlFormElementsGetCreativeCommons($commercial, $derivatives, $jurisdiction);
    if ($response) {
      $form_state['storage']['xml_form_elements'][$element['#name']]['license_uri'] = (string) $response->{'license-uri'};
      $element['license_fieldset']['license_output'] = array(
        '#type' => 'item',
        '#id' => $license_output_id,
        '#markup' => '<strong>' . t('Selected license:') . '</strong><div>' . $response->html->asXml() . '</div>',
      );
    }
    else {
      $form_state['storage']['xml_form_elements'][$element['#name']]['license_uri'] = CreativeCommons::xmlFormElementsCreativeCommonsValue($commercial, $derivatives, $jurisdiction);
      $element['license_fieldset']['license_output'] = array();
    }

    return $element;
  }

  /**
   * Ask the creative commons REST API for a license.
   *
   * Left as a function for use by community modules.
   *
   * @param string $commercial
   *   'y' to allow 'n' to disallow.
   * @param string $derivatives
   *   'y' to allow 'n' to disallow.
   * @param string $jurisdiction
   *   Legal jurisdiction code.
   *
   * @return mixed
   *   SimpleXMLElement the return from the REST API.
   *   FALSE if failed request.
   */
  public static function xmlFormElementsGetCreativeCommons($commercial, $derivatives, $jurisdiction) {
    $response = drupal_http_request(url(
      'http://api.creativecommons.org/rest/1.5/license/standard/get',
      array(
        'query' => array(
          'commercial' => $commercial,
          'derivatives' => $derivatives,
          'jurisdiction' => $jurisdiction,
        ),
      )
    ));
    if ($response->code != 200) {
      return FALSE;
    }
    return simplexml_load_string($response->data, 'SimpleXMLElement');
  }

  /**
   * Gets the value for the element.
   *
   * @param string $commercial
   *   'y' to allow 'n' to disallow.
   * @param string $derivatives
   *   'y' to allow 'n' to disallow.
   * @param string $jurisdiction
   *   Legal jurisdiction code.
   *
   * @return string
   *   The value of the element.
   */
  public static function xmlFormElementsCreativeCommonsValue($commercial, $derivatives, $jurisdiction) {
    $arguments = '';
    if ($commercial == 'n') {
      $arguments = "$arguments-nc";
    }
    if ($derivatives == 'n') {
      $arguments = "$arguments-nd";
    }
    elseif ($derivatives == 'sa') {
      $arguments = "$arguments-sa";
    }
    return "http://creativecommons.org/licenses/by$arguments/2.5/$jurisdiction/";
  }

  /**
   * Static class variables can't be defined with the t().
   *
   * @param string $value
   *   array value to apply t() to
   *
   * @return string
   *   the t() modified string
   */
  public static function xmlFormBuilderFixVariables($value) {
    return t('%t', array('%t' => $value));
  }
}

/**
 * Ajax callback to render the CC license.
 */
function xml_form_elements_creative_commons_ajax(&$form, &$form_state) {
  $parents = $form_state['triggering_element']['#parents'];
  array_pop($parents);
  $creative_commons_element = drupal_array_get_nested_value($form, $parents);

  return $creative_commons_element['license_output'];
}
