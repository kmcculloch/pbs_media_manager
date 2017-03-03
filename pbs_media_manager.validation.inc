<?php

/**
 * @file
 * Validation function for media manager requests.
 */

/**
 * Validate individual arguments to be sent to a COVE API Request.
 *
 * @param string $key
 *   The filter or option.
 * @param string $value
 *   The value of the filter or option.
 * @param string $method
 *   The method being called.
 *
 * @return bool
 *   TRUE if the filter or option if it is valid.
 */
function _pbs_media_manager_allowed_values($key, $value, $method) {
  // Set up arrays to handle most of the cases.
  $patterns = array(
    // Franchises method.
    'franchises' => array(
      'slug' => 'string',
      'page' => 'int',
      'sort' => 'string',
    ),
    // Shows method.
    'shows' => array(
      'audience_scope' => 'string',
      'audience' => 'string',
      'franchise-slug' => 'string',
      'franchise-id' => 'string',
      'genre-id' => 'string',
      'genre-slug' => 'string',
      'page' => 'int',
      'sort' => 'string',
    ),
  );
  if (!isset($patterns[$method][$key])) {
    return FALSE;
  }
  switch ($patterns[$method][$key]) {
    case 'int':
      return is_numeric($value);

    case 'string':
      return is_string($value);

    default:
      return FALSE;
  }
}
