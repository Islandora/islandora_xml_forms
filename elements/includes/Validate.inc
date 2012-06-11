<?php

/**
 * @file
 *
 * Callback for validiate.
 */
module_load_include('inc', 'xml_form_elements', 'includes/Element');

/**
 * Implements hook_elements. Registers Elements.
 * 
 * @return array
 */
function xml_form_elements_ahah_validate_form() {
  list($form_id, $form_build_id, $form, $form_state) = Ahah::getFormInfo();
  try {
    $drupal_form = Ahah::processForm($form_id, $form, $form_state);
    $xml_form = new XMLForm($form_state);
    $document = $xml_form->submit($form, $form_state);
    libxml_use_internal_errors(TRUE); // Prevent Drupal from catching the error.  
    libxml_clear_errors();
    $valid = $document->valid();
    $errors = libxml_get_errors();
    libxml_use_internal_errors(FALSE); // Allow Drupal to catch errors
    $xml = explode("\n", $document->saveXML());
    $error_messages;
    if (count($errors)) {
      foreach ($errors as $error) {
        switch ($error->level) {
          case LIBXML_ERR_WARNING:
            $error_message = "Warning $error->code: ";
            break;
          case LIBXML_ERR_ERROR:
            $error_message = "Error $error->code: ";
            break;
          case LIBXML_ERR_FATAL:
            $error_message = "Fatal Error $error->code: ";
            break;
        }
        $error_message .= trim($error->message);
        $error_messages[] = $error_message;
      }
    }
    $status = $valid ? 'status' : 'error';
    $state = $valid ? 'valid' : 'invalid';
    drupal_set_message(t('The Document is %s', array('%s' => $state)), $status);
    if (!$valid) {
      drupal_set_message(implode('<br/>', $error_messages), $status);
    }
  } catch (Exception $e) {
    $message = 'File: ' . $e->getFile() . '</br>';
    $message .= 'Line: ' . $e->getLine() . '</br>';
    $message .= 'Error: ' . $e->getMessage() . '</br>';
    drupal_set_message($message, 'error');
  }
  Ahah::respond();
  unset($_SESSION['messages']); // Prevent Memory leaks
  exit();
}