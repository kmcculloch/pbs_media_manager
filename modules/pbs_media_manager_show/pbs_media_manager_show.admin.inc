<?php

/**
 * @file
 * Admin functions for the module.
 */

/**
 * Implements hook_form().
 */
function pbs_media_manager_show_admin_form($form, &$form_state) {

  $form = array();

  $form['pbs_media_manager_show_run_cron'] = array(
    '#type' => 'checkbox',
    '#title' => t('Update on cron'),
    '#description' => t('Update entity list using a periodic cron task. WARNING: it is strongly encouraged to make sure cron is being run by drush if you use this feature.'),
    '#default_value' => variable_get('pbs_media_manager_show_run_cron', TRUE),
  );

  $form['pbs_media_manager_show_cron_frequency'] = array(
    '#type' => 'textfield',
    '#title' => t('Minutes between runs'),
    '#description' => t('How many minutes between updates?'),
    '#default_value' => variable_get('pbs_media_manager_show_cron_frequency', 120),
    '#size' => 5,
    '#states' => array(
      'visible' => array(
        ':input[name="pbs_media_manager_show_run_cron"]' => array('checked' => TRUE),
      ),
    ),
  );
  return system_settings_form($form);

}

/**
 * Autocomplete for finding shows by title.
 *
 * @param string $string
 *   The string that will be searched.
 */
function pbs_media_manager_show_widget_autocomplete($string = '') {
  $matches = array();
  if ($string) {

    $query = db_select('pbs_media_manager_shows', 'p')
      ->fields('p', array('id', 'title'));

    $or = db_or();
    $or->condition('title', db_like($string) . '%', 'LIKE');
    $or->condition('nola_root', db_like($string) . '%', 'LIKE');

    $result = $query->condition($or)
      ->range(0, 10)
      ->execute();

    foreach ($result as $prog) {
      $matches[$prog->title . " [$prog->id]"] = check_plain($prog->title);
    }
  }
  drupal_json_output($matches);
}

/**
 * Implements hook_form().
 *
 * Builds the importer form.
 */
function pbs_media_manager_show_importer_form($form, &$form_state) {

  $defaults = array(
    'limit' => 25,
    'parameters' => array(
      'audience-scope' => '',
      'audience' => '',
      'slug' => '',
      'franchise-slug' => '',
      'genre-slug' => '',
      'franchise-id' => '',
      'genre-id' => '',
      'page' => 0,
      'sort' => '',
    ),
  );

  $values = variable_get('pbs_media_manager_show_import_filters', $defaults);

  $form = array();

  $form['header'] = array(
    '#prefix' => '<div>',
    '#markup' => t("You can limit the shows imported by setting parameters on the search."),
    '#suffix' => '</div>',
  );

  $form['limit'] = array(
    '#type' => 'textfield',
    '#title' => t('Shows per API call'),
    '#default_value' => $values['limit'],
    '#description' => t('This determines the number of shows pulled per API call'),
  );

  // Show only the parameters that make sense with the method.
  $form['parameters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );
  $form['parameters']['audience-scope'] = array(
    '#type' => 'textfield',
    '#title' => t('Audience Scope'),
    '#description' => t('e.g. national'),
    '#default_value' => $values['parameters']['audience-scope'],
  );
  $form['parameters']['audience'] = array(
    '#type' => 'textfield',
    '#title' => t('Audience'),
    '#description' => t('PBS Audience value'),
    '#default_value' => $values['parameters']['audience'],
  );
  $form['parameters']['slug'] = array(
    '#type' => 'textfield',
    '#title' => t('Slug'),
    '#description' => t('e.g. chefs-life'),
    '#default_value' => $values['parameters']['slug'],
  );
  $form['parameters']['franchise-slug'] = array(
    '#type' => 'textfield',
    '#title' => t('Franchise Slug'),
    '#default_value' => $values['parameters']['franchise-slug'],
  );
  $form['parameters']['genre-slug'] = array(
    '#type' => 'select',
    '#title' => t('Genre Slug'),
    '#options' => pbs_media_manager_show_genre_options('slug'),
    '#default_value' => $values['parameters']['genre-slug'],
  );
  $form['parameters']['franchise-id'] = array(
    '#type' => 'textfield',
    '#title' => t('Franchise ID'),
    '#default_value' => $values['parameters']['franchise-id'],
  );
  $form['parameters']['genre-id'] = array(
    '#type' => 'select',
    '#title' => t('Genre ID'),
    '#options' => pbs_media_manager_show_genre_options('id'),
    '#default_value' => $values['parameters']['genre-id'],
  );
  $form['parameters']['page'] = array(
    '#type' => 'textfield',
    '#title' => t('Initial Page'),
    '#default_value' => $values['parameters']['page'],
  );

  // Use an AJAX callback to display results without refreshing the page.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Batch Import Shows'),
  );

  return $form;
}

/**
 * Genre option select lists.
 *
 * @param string $type
 *   'id' or 'slug'.
 *
 * @return array
 *   Returns an array suitable for the select list options.
 */

/**
 * Implements hook_action_info().
 */
function pbs_media_manager_show_genre_options($type = NULL) {
  $genres = array();

  $genres[] = array(
    'slug' => 'arts_and_music',
    'id' => 'c52e4777-9649-4977-b13c-a3194c3e42fe',
    'title' => 'Arts and Music',
  );

  $genres[] = array(
    'slug' => 'culture',
    'id' => '9a9f5b68-539c-4aea-83cc-a3f8d67505a0',
    'title' => 'Culture',
  );

  $genres[] = array(
    'slug' => 'drama',
    'id' => 'a6c8f8a3-fd3a-4ffe-9a0d-d32e6e584d5d',
    'title' => 'Drama',
  );

  $genres[] = array(
    'slug' => 'food',
    'id' => '46578c3e-f467-4332-9400-959a31729a0f',
    'title' => 'Food',
  );

  $genres[] = array(
    'slug' => 'history',
    'id' => 'd68d138e-dae6-4fa9-9148-addc88fa6efb',
    'title' => 'History',
  );

  $genres[] = array(
    'slug' => 'home_and_howto',
    'id' => '313ba604-842f-434d-b40b-d6f31cc3cd1a',
    'title' => 'Home and How To',
  );

  $genres[] = array(
    'slug' => 'indie_films',
    'id' => '913015f1-c747-4109-a64b-4876c5929846',
    'title' => 'Indie Films',
  );

  $genres[] = array(
    'slug' => 'news_and_public_affairs',
    'id' => '32aabe94-4417-40a9-ab55-bd22707e275f',
    'title' => 'News and Public Affairs',
  );

  $genres[] = array(
    'slug' => 'science_and_nature',
    'id' => '09684cc5-9a7e-460f-841f-4d95987a8f53',
    'title' => 'Science and Nature',
  );

  $genre_slug_options = array('-- Select --');
  $genre_id_options = array('-- Select --');

  foreach ($genres as $genre) {
    $genre_slug_options[$genre['slug']] = $genre['title'] . ' (' . $genre['slug'] . ')';
    $genre_id_options[$genre['id']] = $genre['title'] . ' (' . $genre['id'] .
      ')';
  }

  asort($genre_slug_options);
  asort($genre_id_options);

  switch ($type) {
    case 'id':
      return $genre_id_options;

    case 'slug':
      return $genre_slug_options;

    default:
      break;
  }
}

/**
 * Implements hook_form_validate().
 */
function pbs_media_manager_show_importer_form_validate($form, &$form_state) {
  $values = $form_state['values'];

  // Make sure the timeout is a number.
  if (!is_int((int) $values['limit'])) {
    form_set_error('limit', t('Limit must be an integer between -1-1000'));
  }
  elseif ((int) $values['limit'] < -1 || (int) $values['limit'] >= 1000) {
    form_set_error('limit', t('Timeout must be an integer between -1-1000'));
  }

  // For Genre IDs and Slugs, if none are selected we must pass a true NULL.
  if (empty($values['genre-id'])) {
    $form_state['values']['genre-id'] = NULL;
  }

  if (empty($values['genre-slug'])) {
    $form_state['values']['genre-slug'] = NULL;
  }

}

/**
 * Implements hook_form_submit().
 */
function pbs_media_manager_show_importer_form_submit($form, &$form_state) {

  $filters = array(
    'limit' => $form_state['values']['limit'],
    'parameters' => array(
      'audience-scope' => $form_state['values']['audience-scope'],
      'audience' => $form_state['values']['audience'],
      'slug' => $form_state['values']['slug'],
      'franchise-slug' => $form_state['values']['franchise-slug'],
      'genre-slug' => $form_state['values']['genre-slug'],
      'franchise-id' => $form_state['values']['franchise-id'],
      'genre-id' => $form_state['values']['genre-id'],
      'page' => $form_state['values']['page'],
    ),
  );

  variable_set('pbs_media_manager_show_import_filters', $filters);

  // Convert saved_filters back to arguments.
  $args = array();
  foreach ($filters['parameters'] as $label => $value) {
    if ($value != '') {
      $args[$label] = $value;
    }
  }

  module_load_include('module', 'pbs_media_manager');
  $response = pbs_media_manager_request('show', '', '', '', $args, 0);

  if (array_key_exists('errors', $response)) {
    drupal_set_message(t('Unable to get list of shows from PBS.'), 'error');
    watchdog('pbs_media_manager_show', 'Unable to get list of shows from Media Manager API: %error', array('%error' => $response['errors']['response']), WATCHDOG_ERROR);
    return;
  }

  // Offset.
  if ($form_state['values']['limit'] > 0 && $form_state['values']['limit'] <=
    count($response)) {
    $remaining = (int) $form_state['values']['limit'] - 1;
  }
  else {
    $remaining = count($response) - 1;
  }

  $operations = array();
  while ($remaining >= 0) {
    $operations[] = array('pbs_media_manager_show_importer_batch_operation', array($response[$remaining]));
    $remaining--;
  }

  variable_set('pbs_media_manager_show_importer_last_count', count($operations));

  $batch = array(
    'title' => t('PBS Show Import'),
    'operations' => $operations,
    'finish' => 'pbs_media_manager_show_importer_batch_finish',
    'file' => drupal_get_path('module', 'pbs_media_manager_show') . "/pbs_media_manager_show.admin.inc",
  );

  batch_set($batch);
}

/**
 * Batch operation callback.
 *
 * The callback used for each batch operations, pulls in the filters to be used
 * and the batch context.
 *
 * @param array $raw_response
 *   The response from the API representing one show.
 * @param array $context
 *   The batch context array.
 */
function pbs_media_manager_show_importer_batch_operation(array $raw_response, array &$context) {

  // Check to make sure this show doesn't already exist:
  $result = db_select('pbs_media_manager_shows', 'p')
    ->fields('p')
    ->condition('pbs_media_manager_show_id', $raw_response['id'], '=')
    ->execute()
    ->fetchAll();

  if (count($result)) {
    // This show was previously imported, so we update it.
    _pbs_media_manager_show_update_entity($result[0]->id, $raw_response);
  }
  else {
    // New show.
    _pbs_media_manager_show_create_entity($raw_response);
  }

  // Flush the entity cache to ensure we have enough memory.
  entity_get_controller('pbs_media_manager_show')->resetCache();
  $context['message'] = t('Imported %show', array('%show' => $raw_response['attributes']['title']));
}

/**
 * Helper function to handle updating already existing entities.
 *
 * @param string $prog_id
 *   The ID for this show.
 * @param object $raw_entity
 *   The raw object from the API.
 */
function _pbs_media_manager_show_update_entity($prog_id, $raw_entity) {
  $show = entity_load('pbs_media_manager_show', array($prog_id), array(), TRUE)[$prog_id];

  if (!$show) {
    watchdog('pbs_media_manager_show', 'Unable to load existing show entity for %title (%id)',
             array('%title' => $raw_entity['title'], '%id' => $prog_id), WATCHDOG_NOTICE);
    return;
  }

  $show->title = $raw_entity['attributes']['title'];
  $show->resource_uri = $raw_entity['links']['self'];
  $show->slug = $raw_entity['attributes']['slug'];
  $show->nola_root = $raw_entity['attributes']['nola'];
  $show->short_description = $raw_entity['attributes']['description_short'];
  $show->long_description = $raw_entity['attributes']['description_long'];
  $show->images = serialize($raw_entity['attributes']['images']);

  if (isset($raw_entity['attributes']['links'][0]) && !empty($raw_entity['attributes']['links'][0]['value'])) {
    $show->link = $raw_entity['attributes']['links'][0]['value'];
  }

  if (isset($raw_entity['attributes']['genre']['slug'])) {
    $show->genre_slug = $raw_entity['attributes']['genre']['slug'];
    $show->genre_title = $raw_entity['attributes']['genre']['title'];
  }

  $show->save();

  watchdog('pbs_media_manager_show', 'Updated %title from pbs_media_manager',
           array('%title' => $show->title),
           WATCHDOG_INFO);
}

/**
 * Helper function to create new entities.
 *
 * @param object $raw_entity
 *   The raw object from the API.
 */
function _pbs_media_manager_show_create_entity($raw_entity) {
  $values = array(
    'title' => $raw_entity['attributes']['title'],
    'pbs_media_manager_show_id' => $raw_entity['id'],
    'link' => $raw_entity['attributes']['links'][0]['value'],
    'resource_uri' => $raw_entity['links']['self'],
    'slug' => $raw_entity['attributes']['slug'],
    'nola_root' => $raw_entity['attributes']['nola'],
    'short_description' => $raw_entity['attributes']['description_short'],
    'long_description' => $raw_entity['attributes']['description_long'],
    'genre_slug' => $raw_entity['attributes']['genre']['slug'],
    'genre_title' => $raw_entity['attributes']['genre']['title'],
    'images' => $raw_entity['attributes']['images'],
  );
  $show_entity = entity_create('pbs_media_manager_show', $values);
  $show_entity->save();

  watchdog('pbs_media_manager_show',
           'Create show entity for %title with date from pbs_media_manager',
           array('%title' => $show_entity->title),
           WATCHDOG_INFO);

}

/**
 * Batch finish callback.
 *
 * @param bool $success
 *   Batch success flag.
 * @param array $results
 *   List of results from operations.
 * @param array $operations
 *   List of operations run.
 */
function pbs_media_manager_show_importer_batch_finish($success, array $results, array $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = t('@count items successfully processed:', array('@count' => count($results)));
    $message .= theme('item_list', array('items' => $results));
    drupal_set_message($message);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array(
      '%error_operation' => $error_operation[0],
      '@arguments' => print_r($error_operation[1], TRUE),
    ));
    drupal_set_message($message, 'error');
  }
}
