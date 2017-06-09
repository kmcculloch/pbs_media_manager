<?php

/**
 * @file
 * Update functions for migrating COVE API Player fields to Media
 * Manager Player fields.
 */


/**
 * Batch operation to migrate COVE API Player fields to Media Manager Player
 * fields.
 *
 * @param $field_name
 * @param $context
 *
 */
function pbs_media_manager_player_migrate_batch_operation($field_name,
  &$context) {
  // load field via field_read_field() setting
  // $include_additional['include_inactive'] to TRUE to also pull inactive
  // fields
  $field = field_read_field($field_name, $include_additional = array('include_inactive'));
  
  // update field array to swap module
  $field['module'] = 'pbs_media_manager_player';
  
  // update the field
  field_update_field($field);

  // load field instances via field_read_instances() setting
  // $include_additional['include_inactive'] to TRUE to also pull inactive
  // fields
  $field_instances = field_read_instances(array('field_name' => $field_name), $include_additional = array('include_inactive'));
  
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($field_instances);
  }
  
  // Update each field instance to swap widget module and type and display
  // formatter and module, then update the instance.
  foreach ($field_instances as $instance) {

    $instance['widget']['module'] = 'pbs_media_manager_player';
    $instance['widget']['type'] = 'pbs_media_id';
    
    foreach ($instance['display'] as &$display) {
      $display['type'] = 'pbs_media_manager_player_formatter';
      $display['module'] = 'pbs_media_manager_player';
    }

    field_update_instance($instance);
    $context['sandbox']['progress']++;
  
    // Inform the batch engine that we are not finished,
    // and provide an estimation of the completion level we reached.
    if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
      $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
    }
  }
  
}


function pbs_media_manager_player_migrate_batch_finish($success, $results,
  $operations) {
  
  if ($success) {
    drupal_set_message('Success!');
    dpm($results, 'results');
    drupal_goto('admin/config/media/pbs_media_manager', array(), 301);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }
  
}