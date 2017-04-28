<?php

/**
 * @file
 * An admin interface to initialize the migration batch from old API.
 */

/**
 * Implements hook_form().
 */
function pbs_media_manager_player_migration_form() {

  $form = array();

  $form['description_wrapper'] = array(
    '#type' => 'container',
  );
  $form['description_wrapper']['description'] = array(
    '#type' => 'markup',
    '#markup' => t('Migrate you existing Cove API player fields to PBS Media manager. Backup the database first. <strong>This cannot be undone.</strong>'),
  );

  $form['start_migration_button'] = array(
    '#type' => 'submit',
    '#value' => t('Migrate Field(s)'),
  );

  return $form;

}

/**
 * Implements hook_form_submit().
 */
function pbs_media_manager_player_migration_form_submit(&$form, $form_state) {

  // Load the fields using Cove Player module.
  module_load_include('modules', 'pbs_media_manager_player');
  $fields = _pbs_media_manager_player_find_cove_fields();

  if (empty($fields)) {
    drupal_set_message(t('No existing Cove fields found.'), 'error', FALSE);
    drupal_goto('admin/config/media/pbs_media_manager', array(), 301);
  }

  // Prepare the batch.
  $operations = array();
  foreach (array_keys($fields) as $field) {
    $operations[] = array(
      'pbs_media_manager_player_migration_batch_operation',
      array('field' => $field),
    );
  }

  $batch = array(
    'title' => t('Media Manager Player Migration'),
    'operations' => $operations,
    'finish' => 'pbs_media_manager_player_migration_batch_finish',
    'file' => drupal_get_path('module', 'pbs_media_manager_player') . "/pbs_media_manager_player.inc",
  );

  // Initate batch.
  batch_set($batch);

}
