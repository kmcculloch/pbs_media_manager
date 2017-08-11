<?php

/**
 * @file
 * Filter handler for genres.
 */

/**
 * Extending the views_handler_filter_in_operator class for the genres filter.
 */
class pbs_media_manager_show_handler_filter_genres extends
 views_handler_filter_in_operator {

  /**
   * Get the list of possible genres.
   *
   * @inheritdoc
   */
  public function get_value_options() {

    // Get all the genres.
    $genres = db_select('pbs_media_manager_shows', 'shows')
      ->fields('shows', array('genre_slug', 'genre_title'))
      ->execute()
      // ->fetchAllAssoc('genre_slug');.
      ->fetchAllKeyed();

    // TODO: How to only select NOT NULL?
    // Remove the empty result.
    $genres = array_filter($genres);

    // Sort by title.
    asort($genres);

    $this->value_options = $genres;

    return $genres;

  }

}
