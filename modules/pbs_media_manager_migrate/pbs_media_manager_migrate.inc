<?php

/**
 * @file
 * Batch API functions for migration.
 *
 * Helps convert COVE API Player fields to Media Manager Player fields.
 */

/**
 * Batch operation to migrate COVE API Player fields to Media Manager Player.
 *
 * @param string $entity_type
 *   The type of entity being worked on.
 * @param string $cove_field
 *   Old field name.
 * @param string $media_manager_field
 *   New field name.
 * @param int $entity_id
 *   The ID of the entity to update.
 * @param array $context
 *   Drupal batch function context array.
 */
function pbs_media_manager_player_migrate_batch_operation($entity_type, $cove_field, $media_manager_field, $entity_id, array &$context) {

  // Load each entity.
  $load = entity_load($entity_type, [$entity_id], $reset = TRUE);

  if (empty($load)) {
    return;
  }

  $active = current($load);

  // Get the field language.
  $field_language = field_language($entity_type, $active, $cove_field);

  // Load the field data.
  $fields = field_get_items($entity_type, $active, $cove_field);
  if (!$fields) {
    return;
  }
  foreach ($fields as $delta => $field) {

    // Call the API using the Legacy Assets Endpoint.
    $resource = 'assets/legacy';
    $resourceId = NULL;
    $parentResource = NULL;
    $legacyID = $field['object_id'];

    $response = pbs_media_manager_request($resource, $resourceId, $parentResource, $legacyID);

    // If there's no response, show an error message.
    if (!$response) {
      form_set_error(
        'no_response',
        t('Request failed most likely due to invalid request. Check recent log entries for more detail')
      );
    }
    else {

      // Construct the video's URL from the slug.
      $slug = $response['data']['attributes']['slug'];

      // TODO: Replace 'beta' with 'www' once new PBS.org goes live.
      $url = 'http://beta.pbs.org/video/' . $slug;

      // Map the results to the field elements.
      $active->$media_manager_field[$field_language][$delta] = [
        'url'           => $url,
        'original_url'  => $url,
        'asset_id'      => $response['data']['id'],
        'slug'          => $slug,
        'title'         => $response['data']['attributes']['title'],
        'description'   => $response['data']['attributes']['description_long'],
        'legacy_id'     => $legacyID,
        'premiere_date' => $response['data']['attributes']['premiered_on'],
        'encore_date'   => $response['data']['attributes']['encored_on'],
      ];

      // If the video isn't published, we need to store an error for
      // display after the operation is complete.
      if (isset($response['data']['attributes']['publish_state']) &&
          empty($response['data']['attributes']['publish_state'])
      ) {

        // If an error for this video doesn't exist yet, create it.
        if (!isset($context['results']['errors'][$legacyID])) {
          $context['results']['errors'][$legacyID] = [
            'cove id'       => $legacyID,
            'video title'   => $response['data']['attributes']['title'],
            'generated url' => $url,
          ];
        }

        // Add this instance of the video to the error.
        $context['results']['errors'][$legacyID]['instances'][] = [
          'node title' => $active->title,
        ];
      }
    }
  }

  if (!empty($active->$media_manager_field[$field_language])) {
    // Update the node.
    entity_save($entity_type, $active);
    $context['results']['updated'][] = $active->nid;

    // Update the current node and the progress.
    $context['sandbox']['current_node'] = $active->nid;
    $context['sandbox']['progress']++;

  }

}

/**
 * Batch 'finished' callback.
 *
 * @param $success
 * @param $results
 * @param $operations
 */
function pbs_media_manager_player_migrate_batch_finish($success,
  $results,
  $operations) {

  if ($success) {
    drupal_get_messages('warning');

    $migrated = count($results['updated']);
    drupal_set_message('Migration complete! ' . $migrated . ' items were migrated.');

    if (count($results['errors'])) {

      // TODO: Theme this pretty.
      $list = '<pre>' . print_r($results['errors'], 1) . '</pre>';
      $message = t('The following videos are not available in Media Manager. !list', array('!list' => $list));

      drupal_set_message($message, 'warning');

    }

  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments',
                 array(
                   '%error_operation' => $error_operation[0],
                   '@arguments' => print_r($error_operation[1], TRUE),
                 ));
    drupal_set_message($message, 'error');
  }

}
