<?php

/**
 * @file
 * An admin interface to initialize the migration batch from old API.
 */

/**
 * Implements hook_form().
 */
function pbs_media_manager_migrate_form(array $form, array &$form_state) {
  
  // Load the fields using Cove Player module.
  $cove_fields = _pbs_media_manager_migrate_find_cove_fields();
  
  if (empty($cove_fields)) {
    drupal_set_message(t('No existing Cove fields found.'), 'error', FALSE);
  }
  else {
    // Get the list of fields that are available for migration, either as
    // targets or destinations
    $options = _pbs_media_manager_migrate_options($cove_fields);
    
    // Segment out just the bundles (content types) for winnowing.
    
    $bundles = array('select' => t('-- Select a content type --'));
    $bundles += drupal_map_assoc(array_keys($options));

    // Get the selected bundle, if it exists
    $bundle_selected = isset($form_state['values']['bundle']) ? $form_state['values']['bundle'] : key($bundles);
  
    $form['description_wrapper'] = array(
      '#type' => 'container',
    );
    $form['description_wrapper']['description'] = array(
      '#type' => 'markup',
      '#markup' => t('Migrate your existing Cove API player fields to PBS Media manager. Backup the database first. <strong>This cannot be undone.</strong>'),
    );
    
    $form['bundle'] = array(
      '#type' => 'select',
      '#title' => 'Content Type',
      '#options' => $bundles,
      '#default_value' => $bundle_selected,
      '#ajax' => array(
        'callback' => 'pbs_media_manager_migrate_fields_replace',
        'event' => 'change',
        'wrapper' => 'fields-replace',
      ),
      '#description' => t('Select the COVE field you would like to migrate.'),
      '#required' => TRUE,
      '#element_validate' => array('pbs_media_manager_migrate_element_validate'),
    );
    
    $form['fields'] = array(
      '#type' => 'fieldset',
      '#title' => 'Migration settings',
      '#prefix' => '<div id="fields-replace">',
      '#suffix' => '</div>',
      
    );
    
    $form['fields']['cove_field'] = array(
      '#type' => 'select',
      '#title' => 'COVE Field',
      '#options' => _pbs_media_manager_migrate_get_options('cove_api_player', $bundle_selected, $options),
      '#default_value' => isset($form_state['values']['cove_field']) ? $form_state['values']['cove_field'] : 'select',
      '#description' => t('Select the Media Manager field you would like to migrate to.'),
      '#element_validate' => array('pbs_media_manager_migrate_element_validate'),
      // TODO: Why no asterisk after options load?
      '#states' => array(
        'required' => array(
          ':input[name="bundle"]' => array('filled' => TRUE),
        ),
      ),
      
    );
  
    $form['fields']['media_manager_field'] = array(
      '#type' => 'select',
      '#title' => 'Media Manager Field',
      '#options' => _pbs_media_manager_migrate_get_options('pbs_media_manager_player',
        $bundle_selected, $options),
      '#default_value' => isset($form_state['values']['media_manager_field']) ? $form_state['values']['media_manager_field'] : 'select',
      '#description' => t('Select the Media Manager field you would like to migrate to.'),
      '#element_validate' => array('pbs_media_manager_migrate_element_validate'),
      // TODO: Why no asterisk after options load?
      '#states' => array(
        'required' => array(
          ':input[name="bundle"]' => array('filled' => TRUE),
        ),
      ),
    );
    
    $form['start_migration_button'] = array(
      '#type' => 'submit',
      '#value' => t('Migrate'),
    );
    
  
    return $form;
  }
}

/**
 * Implements hook_form_submit().
 */
function pbs_media_manager_migrate_form_submit(&$form, $form_state) {
  
  //dpm('submitted!');
  //dpm($form, 'form');
  //dpm($form_state, 'form state');
  /*
  
  // Load the fields using Cove Player module.
  module_load_include('modules', 'pbs_media_manager_player');
  $fields = _pbs_media_manager_migrate_find_cove_fields();
  
  if (empty($fields)) {
    drupal_set_message(t('No existing Cove fields found.'), 'error', FALSE);
    drupal_goto('admin/config/media/pbs_media_manager', array(), 301);
  }
  

  // Prepare the batch.
  $operations = array();
  foreach (array_keys($fields) as $field) {
    $operations[] = array(
      'pbs_media_manager_migrate_batch_operation',
      array('field' => $field),
    );
  }

  $batch = array(
    'title' => t('Media Manager Player Migration'),
    'operations' => $operations,
    'finished' => 'pbs_media_manager_migrate_batch_finish',
    'file' => drupal_get_path('module', 'pbs_media_manager_migrate') . "/pbs_media_manager_migrate.inc",
  );

  // Initate batch.
  batch_set($batch);
  */
}


/**
 * Validation for required select elements
 *
 * @param $element
 * @param $form_state
 */
function pbs_media_manager_migrate_element_validate($element, &$form_state) {
  if (empty($element['#value']) || $element['#value'] == 'select') {
    form_error($element, t('%field is required.', array('%field' => $element['#title'])));
  }
}
