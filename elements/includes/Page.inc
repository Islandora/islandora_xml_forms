<?php

/**
 * @file
 *
 */

/**
 * Theme Hook for tags elements.
 */
function theme_form_page($element) {
  return $element['#children'];
}