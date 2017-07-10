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
    return array();
  }

  // Get the list of fields that are available for migration, either as
  // targets or destinations.
  $options = _pbs_media_manager_migrate_options($cove_fields);

  // Segment out just the bundles (content types) for winnowing.
  $bundles = array('select' => t('-- Select a content type --'));
  $bundles += drupal_map_assoc(array_keys($options));

  // Get the selected bundle, if it exists.
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

/**
 * Implements hook_form_submit().
 */
function pbs_media_manager_migrate_form_submit(&$form, $form_state) {
  // TODO: This should probably all be in a batch for performance.
  // The bundle value is stored as entity_type:bundle. We need to split it up.
  $bundle_info = explode(':', $form_state['values']['bundle']);
  $entity_type = $bundle_info[0];
  $bundle = $bundle_info[1];

  $cove_field = $form_state['values']['cove_field'];
  $media_manager_field = $form_state['values']['media_manager_field'];

  $entityQuery = new EntityFieldQuery();
  $query = $entityQuery->entityCondition('entity_type', $entity_type)
    ->propertyCondition('type', $bundle)
    ->fieldCondition($cove_field, 'object_id', 'NULL', '!=');
  $entities = $query->execute();

  if (empty($entities[$entity_type])) {
    drupal_set_message(t('No content using @field were found on @bundle.',
                       array(
                         '@field' => $cove_field,
                         '@bundle' => $bundle,
                       )));
    return;
  }

  // Prepare the batch.
  $operations = array();

  foreach ($entities[$entity_type] as $entity) {
    // Extract the entity ID from the $entity object.  The ID will be the
    // first item in the array.
    $entity_ids = entity_extract_ids($entity_type, $entity);
    $entity_id = $entity_ids[0];

    $operations[] = array(
      'pbs_media_manager_player_migrate_batch_operation',
      array(
        'type' => $entity_type,
        'cove_field' => $cove_field,
        'media_manager_field' => $media_manager_field,
        'entity_id' => $entity_id,
      ),
    );
  }

  $batch = array(
    'title' => t('Media Manager Player Migration'),
    'init_message' => t('Beginning migration...'),
    'error_message' => t('Whoops!'),
    'operations' => $operations,
    'finished' => 'pbs_media_manager_player_migrate_batch_finish',
    'file' => drupal_get_path('module', 'pbs_media_manager_migrate') . "/pbs_media_manager_migrate.inc",
  );

  // Initiate batch.
  batch_set($batch);

}

/**
 * Validation for required select elements.
 *
 * @param array $element
 *   Form element array.
 * @param $form_state
 *   Form state array.
 */
function pbs_media_manager_migrate_element_validate(array $element, &$form_state) {
  if (empty($element['#value']) || $element['#value'] == 'select') {
    form_error($element, t('%field is required.', array('%field' => $element['#title'])));
  }
}

/**
 * Implements hook_form_validate().
 *
 * Validation for the migration form. Checks that the chosen Media Manager
 * field has enough values allowed to hold the Cove Field items.
 */
function pbs_media_manager_migrate_form_validate($form, &$form_state) {

  // Only run this if cove_field and media_manager_field have values.
  if (!empty($form_state['values']['cove_field']) &&
      !empty($form_state['values']['media_manager_field']) &&
      $form_state['values']['cove_field'] != 'select' &&
      $form_state['values']['media_manager_field'] != 'select') {

    $error = FALSE;
    $cove_field = $form_state['values']['cove_field'];
    $media_manager_field = $form_state['values']['media_manager_field'];

    $cove_field_info = field_info_field($cove_field);
    $media_manager_field_info = field_info_field($media_manager_field);

    // If $media_manager_field's cardinality is unlimited, no error.
    if ($media_manager_field_info['cardinality'] == '-1') {
      $error = FALSE;
    }
    // If $cove_field's cardinality is unlimited, but $media_manager_field's is
    // not, validation fails.
    elseif ($cove_field_info['cardinality'] == '-1') {
      $error = TRUE;
    }
    // If $cove_field cardinality's is less than $media_manager_field's
    // cardinality, validation fails.
    elseif ($media_manager_field_info['cardinality'] < $cove_field_info['cardinality']) {
      $error = TRUE;
    }

    if ($error) {
      form_set_error($media_manager_field,
                     t('%mm_field must be able to hold at least as many items as %cove_field. Check the field settings, then try again.',
                       array(
                         '%mm_field' => $media_manager_field,
                         '%cove_field' => $cove_field,
                       )
                     ));
    }
  }
}
