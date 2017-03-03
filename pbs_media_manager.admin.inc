<?php

/**
 * @file
 * Admin form functions for settings form for Media Manager API.
 */

define('PBS_MEDIA_MANAGER_STAGE_ENDPOINT', 'https://media-staging.services.pbs.org/api/v1');
define('PBS_MEDIA_MANAGER_PRODUCTION_ENDPOINT', 'https://media.services.pbs.org/api/v1');

/**
 * Settings form for Cove API.
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
    '#title' => t('PBS COVE API Secret'),
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
  return system_settings_form($form);
}

/**
 * Validate the Cove API settings form.
 */
function pbs_media_manager_admin_site_settings_validate($form, &$form_state) {
  $values = $form_state['values'];
  $api_key = $values['pbs_media_manager_key'];
  $api_secret = $values['pbs_media_manager_secret'];
  $response = pbs_media_manager_request('franchises', array('slug' => 'masterpiece'), 0, $api_key, $api_secret);
  if (!$response) {
    form_set_error('pbs_media_manager_settings', t('Failed to connect to Media Manager API with those credentials.'));
  }
  else {
    drupal_set_message(t('Successfully connected with Media Manager API. Settings have been saved.'));
  }
}
