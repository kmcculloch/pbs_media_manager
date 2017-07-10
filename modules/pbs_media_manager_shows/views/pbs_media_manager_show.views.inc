<?php

/**
 * @file
 * Views hooks.
 */

/**
 * Implements hook_views_data_alter().
 */
function pbs_media_manager_show_views_data_alter(&$data) {

  if (isset($data['pbs_media_manager_shows']) && isset($data['pbs_media_manager_shows']['genre_title'])) {
    $data['pbs_media_manager_shows']['genre_title']['filter']['handler'] =
      'pbs_media_manager_show_handler_filter_genres';
  }
}
