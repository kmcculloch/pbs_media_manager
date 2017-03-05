<?php

/**
 * @file
 * Admin form functions for settings form for Media Manager API.
 */

define('PBS_MEDIA_MANAGER_STAGE_ENDPOINT', 'https://media-staging.services.pbs.org/api/v1');
define('PBS_MEDIA_MANAGER_PRODUCTION_ENDPOINT', 'https://media.services.pbs.org/api/v1');

/**
 * Settings form for Media Manager API.
 */
function pbs_media_manager_admin_site_settings() {
  $form = array();
  $form['pbs_media_manager_settings'] = array(
    '#title' => t('PBS Media Manager API Settings'),
    '#type' => 'fieldset',
  );
  $form['pbs_media_manager_settings']['pbs_media_manager_key'] = array(
    '#title' => t('API Key'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('pbs_media_manager_key'),
  );
  $form['pbs_media_manager_settings']['pbs_media_manager_secret'] = array(
    '#title' => t('API Secret'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => variable_get('pbs_media_manager_secret'),
  );
  $form['pbs_media_manager_settings']['pbs_media_manager_endpoint'] = array(
    '#title' => t('Media Manager Endpoint'),
    '#type' => 'select',
    '#options' => array(
      PBS_MEDIA_MANAGER_STAGE_ENDPOINT => 'Staging',
      PBS_MEDIA_MANAGER_PRODUCTION_ENDPOINT => 'Production',
    ),
    '#default_value' => variable_get('pbs_media_manager_endpoint', PBS_MEDIA_MANAGER_PRODUCTION_ENDPOINT),
  );

  // If there are settings, allow them to be tested by getting a station list.
  $test_disabled = empty(variable_get('pbs_media_manager_key')) || empty(variable_get('pbs_media_manager_secret'));
  $form['pbs_media_manager_settings']['test_button'] = array(
    '#type' => 'submit',
    '#value' => t('Test Connection'),
    '#disabled' => $test_disabled,
    '#ajax' => array(
      'callback' => '_pbs_media_manager_api_connection_test_callback',
      'wrapper' => 'box',
      'name' => 'submit1',
    ),
  );
  return system_settings_form($form);
}

/**
 * Test form submit handler for AJAX Callback.
 *
 * @param array $form
 *   Form array.
 * @param array $form_state
 *   Submitted form state.
 *
 * @return array
 *   Returns a response array.
 */
function _pbs_media_manager_api_connection_test_callback(array $form, array &$form_state) {

  $args = array();

  // Filters.
  foreach ($form['parameters'] as $label => $element) {
    if ($label[0] != '#' && $label != 'fields') {
      if ($element['#value']) {
        $args[$label] = $element['#value'];
      }
    }
  }

  if (pbs_media_manager_api_connection_test()) {
    $response = t('Connection attempt succeeded.');
  }
  else {
    $response = t('Connection attempt failed.');
  }

  $element = $form['box'];
  $element['#markup'] = $response;

  return $element;

}
