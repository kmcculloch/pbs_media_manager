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

  // Initialize the progress counter.
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
  }

  // Load each entity.
  $load = entity_load($entity_type, [$entity_id], $conditions = FALSE, $reset =
    TRUE);

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

    // If there's no response at all, show an error message.
    if (!$response) {
      form_set_error(
        'no_response',
        t('Request failed most likely due to invalid request. Check recent log entries for more detail')
      );
    }
    // If there is a response, but there is no data attribute in the
    // response, there' something wrong with the COVE ID and the migration
    // fails.
    elseif (!isset($response['data'])) {
      // If an error for this video doesn't exist yet, create it.
      if (!isset($context['results']['errors'][$legacyID])) {
        $context['results']['errors'][$legacyID] = [
          'cove id' => $legacyID,
        ];
      }

      // Add this instance of the video to the error.
      $context['results']['errors'][$legacyID]['instances'][] = [
        'node title' => $active->title,
      ];
    }
    else {

      // Construct the video's URL from the slug.
      $slug = $response['data']['attributes']['slug'];
      
      $url = 'http://www.pbs.org/video/' . $slug;

      // Map the results to the field elements.
      $active->{$media_manager_field}[$field_language][$delta] = [
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

  if (!empty($active->{$media_manager_field}[$field_language])) {
    // Update the node.
    entity_save($entity_type, $active);

    // Get the entity agnostic id.
    $entity_ids = entity_extract_ids($entity_type, $active);
    $entity_id = $entity_ids[0];

    $context['results']['updated'][] = $entity_id;

    // Update the current node and the progress.
    $context['sandbox']['current_node'] = $entity_id;
    $context['sandbox']['progress']++;

  }

}

/**
 * Batch 'finished' callback.
 *
 * @param bool $success
 *   Success flag.
 * @param array $results
 *   Results list.
 * @param array $operations
 *   Operations run.
 */
function pbs_media_manager_player_migrate_batch_finish($success, array $results, array $operations) {

  if ($success) {
    drupal_get_messages('warning');

    $migrated = count($results['updated']);
    drupal_set_message(t('Migration complete! %migrated items were migrated.', array('%migrated' => $migrated)));

    if (isset($results['errors'])) {

      // TODO: Theme this pretty.
      $list = '<pre>' . print_r($results['errors'], 1) . '</pre>';
      $list = filter_xss($list, array('pre'));
      drupal_set_message(t('The following videos are not available in Media Manager. !list', array('!list' => $list)), 'warning');

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
