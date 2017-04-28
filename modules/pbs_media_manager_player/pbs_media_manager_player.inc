<?php

/**
 * @file
 * Update functions for migrating COVE API Player fields to Media
 * Manager Player fields.
 */

/**
 * Test page
 */
function pbs_media_manager_migrate_test() {
  
  $output = 'foo';
  
  $output .= pbs_media_manager_player_migrate_batch_operation('field_cove3',
    $context);
  
  return $output;
  
}

/**
 * Will get field machine name, array of instances, and &$context
 */
function pbs_media_manager_player_migrate_batch_operation($field_name,
  &$context) {
  
  // load field via field_read_field() setting
  // $include_additional['include_inactive'] to TRUE to also pull inactive
  // fields
  $field = field_read_field($field_name, $include_additional = array('include_inactive'));
  
  dpm($field, 'field before');
  // update field array to swap module and type
  $field['module'] = 'pbs_media_manager_player';
  //$field['type'] = 'pbs_media';
  
  dpm($field, 'field after');
  
  
  // load field instances via field_read_instances() setting
  // $include_additional['include_inactive'] to TRUE to also pull inactive
  // fields
  $field_instances = field_read_instances(array('field_name' => $field_name), $include_additional = array('include_inactive'));
  
  
  
  // update each field instance to swap widget module and widget type and update
  foreach ($field_instances as $instance) {
    
    dpm($instance, 'instance before');
    $instance['widget']['module'] = 'pbs_media_manager_player';
    $instance['widget']['type'] = 'pbs_media_id';
    foreach ($instance['display'] as &$display) {
      $display['type'] = 'pbs_media_manager_player_formatter';
      $display['module'] = 'pbs_media_manager_player';
    }
    dpm($instance, 'instance after');
    field_update_instance($instance);
  }
  
  // update the field
  field_update_field($field);
  
}


function pbs_media_manager_player_migrate_batch_finish($success, $results,
  $operations) {
  
  drupal_set_message('Success!');
}