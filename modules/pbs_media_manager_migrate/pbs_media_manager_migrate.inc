<?php

/**
 * @file
 * Update functions for migrating COVE API Player fields to Media
 * Manager Player fields.
 */

/**
 * Batch operation to migrate COVE API Player fields to Media Manager Player.
 *
 * @param $entity_type
 * @param $bundle
 * @param $cove_field
 * @param $media_manager_field
 * @param $context
 */
function pbs_media_manager_player_migrate_batch_operation($entity_type, $bundle, $cove_field, $media_manager_field, &$context) {

  if (!isset($context['sandbox']['progress'])) {
    // Count all entities of the selected bundle where the COVE field isn't
    // empty.
    $entityQuery = new EntityFieldQuery();
    $query = $entityQuery->entityCondition('entity_type', $entity_type)
      ->propertyCondition('type', $bundle)
      ->fieldCondition($cove_field, 'object_id', 'NULL', '!=')
      ->propertyOrderBy('nid', 'ASC')
      ->count()
      ->execute();

    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_node'] = 0;
    $context['sandbox']['max'] = $query;
    $context['results']['updated'] = array();
    $context['results']['errors'] = array();

  }

  // Limit the batch to 30 items so it doesn't time out.
  $limit = 30;

  // Find entities of the selected bundle where the COVE field isn't empty
  // for this batch.
  $entityQuery = new EntityFieldQuery();
  $query = $entityQuery->entityCondition('entity_type', $entity_type)
    ->propertyCondition('type', $bundle)
    ->fieldCondition($cove_field, 'object_id', 'NULL', '!=')
    ->propertyOrderBy('nid', 'ASC')
    ->propertyCondition('nid',
      $context['sandbox']['current_node'], '>')
    ->range(0, $limit);

  $entities = $query->execute();

  foreach ($entities as $entity_type => $entity) {

    foreach ($entity as $item) {

      // Currently, this only works with nodes. Submit a patch for other
      // entity types.
      if ($entity_type == 'node') {
        // Load each entity.
        $loaded_node = entity_load($entity_type, array($item->nid));

        foreach ($loaded_node as $node) {
          // Get the field language.
          $field_language = field_language('node', $node, $cove_field);

          // Load the field data.
          $fields = field_get_items($entity_type, $node, $cove_field);
          foreach ($fields as $delta => $field) {

            // Call the API using the Legacy Assets Endpoint.
            $resource = 'assets/legacy';
            $resourceId = NULL;
            $parentResource = NULL;
            $legacyID = $field['object_id'];

            $response =
              pbs_media_manager_request($resource, $resourceId, $parentResource,
                $legacyID);

            // If there's no response, show an error message.
            if (!$response) {
              form_set_error('no_response',
                t('Request failed most likely due to invalid request. Check recent log entries for more detail'));
            }
            else {

              // Construct the video's URL from the slug.
              $slug = $response['data']['attributes']['slug'];

              // TODO: Replace 'beta' with 'www' once new PBS.org goes live.
              $url = 'http://beta.pbs.org/video/' . $slug;

              // Map the results to the field elements.
              $node->$media_manager_field[$field_language][$delta] = array(
                'url' => $url,
                'original_url' => $url,
                'asset_id' => $response['data']['id'],
                'slug' => $slug,
                'title' => $response['data']['attributes']['title'],
                'description' => $response['data']['attributes']['description_long'],
                'legacy_id' => $legacyID,
                'premiere_date' => $response['data']['attributes']['premiered_on'],
                'encore_date' => $response['data']['attributes']['encored_on'],
              );

              // If the video isn't published, we need to store an error for
              // display after the operation is complete.
              if (isset($response['data']['attributes']['publish_state']) &&
                empty($response['data']['attributes']['publish_state'])) {

                // If an error for this video doesn't exist yet, create it.
                if (!isset($context['results']['errors'][$legacyID])) {
                  $context['results']['errors'][$legacyID] = array(
                    'cove id' => $legacyID,
                    'video title' => $response['data']['attributes']['title'],
                    'generated url' => $url,
                  );
                }

                // Add this instance of the video to the error.
                $context['results']['errors'][$legacyID]['instances'][] =
                  array(
                    'nid' => $node->nid,
                    'node title' => $node->title,
                  );
              }
            }
          }

          if (!empty($node->$media_manager_field[$field_language])) {
            // Update the node.
            node_save($node);
            $context['results']['updated'][] = $node->nid;

            // Update the current node and the progress.
            $context['sandbox']['current_node'] = $node->nid;
            $context['sandbox']['progress']++;

          }

        }

      }

    }

  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
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
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
    drupal_set_message($message, 'error');
  }

}
