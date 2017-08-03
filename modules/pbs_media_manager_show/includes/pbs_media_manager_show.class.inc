<?php

/**
 * @file
 * Provides an EntityAPIController for our custom show entities.
 */

/**
 * Extending the EntityAPIController for the pbs_media_managerShow entity.
 */
class PbsMediaManagerShowEntityController extends EntityAPIController {

  /**
   * @inheritdoc
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {
    $build = parent::buildContent($entity, $view_mode, $langcode, $content);

    return $build;
  }

  /**
   * @inheritdoc
   */
  public function load($ids = array(), $conditions = array()) {
    $entities = parent::load($ids, $conditions);

    // Unserialize the image field.
    if (is_array($entities)) {
      foreach ($entities as $show) {
        if (!is_array($show->images)) {
          $images = unserialize($show->images);
          $show->images = $images;
        }
      }
    }

    return $entities;
  }

  /**
   * @inheritdoc
   */
  public function save($entity, DatabaseTransaction $transaction = NULL) {
    if (is_array($entity->images)) {
      $entity->images = serialize($entity->images);
    }

    return parent::save($entity, $transaction);
  }

}

/**
 * Project entity class extending the Entity class.
 */
class PbsMediaManagerShowEntity extends Entity {

  /**
   * Change the default URI from default/id to pbs_show/id.
   */
  protected function defaultUri() {
    return array('path' => 'pbs_show/' . $this->identifier());
  }

  /**
   * Get the full detail record for this show.
   */
  public function getFullDetail() {
    return $this->makeRequest();
  }

  /**
   * Uses the Media Manager API to pull a list of seasons for this Show.
   */
  public function getSeasons() {
    return $this->makeRequest('season');
  }

  /**
   * Uses the Media Manager API to pull a list of assets for this Show.
   */
  public function getAssets() {
    return $this->makeRequest('asset');
  }

  /**
   * Uses the Media Manager API to pull a list of specials for this Show.
   */
  public function getSpecials() {
    return $this->makeRequest('special');
  }

  /**
   * Uses the Media Manager API to pull a list of collections for this Show.
   */
  public function getCollections() {
    return $this->makeRequest('collection');
  }

  /**
   * Uses the Media Manager API to pull a list of taxonomies for this Show.
   */
  public function getTaxonomies() {
    return $this->makeRequest('taxonomies');
  }

  /**
   * Uses the Media Manager API to build list of episodes for the Show.
   *
   * Media Manager does not directly provide a way to do this, so this requires
   * several round trips to the API, and is therefore a VERY slow method.
   *
   * @param int $count
   *   The number of episodes to retrieve.
   * @param bool $include_specials
   *   Include specials in the list of returned episodes.
   *
   * @return array
   *   Returns an array of Episode records.
   */
  public function getLatestEpisodes($count, $include_specials = TRUE) {
    $seasons = $this->getSeasons();

    if ($include_specials) {
      $specials = $this->getSpecials();
    }
    else {
      $specials = [];
    }

    // If there are no season records we get an array with meta data. We don't
    // really care, so make it an empty array for easier processing.
    if (isset($seasons['meta'])) {
      $seasons = [];
    }

    // Same again specials.
    if (isset($specials['meta'])) {
      $specials = [];
    }

    $special_count = count($specials);
    $episodes = array();

    // Sort the seasons by ordinal value.
    usort($seasons, function ($a, $b) {
      return $a['attributes']['ordinal'] < $b['attributes']['ordinal'];
    });

    // Sort Specials by Premeir date.
    usort($specials, function ($a, $b) {
      return strtotime($a['attributes']['premiered_on']) < strtotime($b['attributes']['premiered_on']);
    });

    // If there are specials to merge into the list we need to track our
    // progress through that list.
    $special_index = 0;
    $date_tracker = 0;
    if ($special_count) {
      $date_tracker = strtotime($specials[0]['attributes']['premiered_on']);
    }

    // For each seasons load the list of episodes from MM and build up the.
    // The episode list until it reaches count.  Then return list.
    foreach ($seasons as $season) {
      // Load the season's episodes.
      $season_episodes = pbs_media_manager_request('episode', $season['id'], 'season', '', array('sort' => '-ordinal'));

      foreach ($season_episodes as $episode) {
        // If we have specials check to see if currently tracked on should be
        // merged in here.
        if ($date_tracker) {
          while ($date_tracker < strtotime($episode['attributes']['premiered_on'])) {
            $special = $specials[$special_index];
            $special_assets = pbs_media_manager_request('asset', $special['id'], 'special');
            foreach ($special_assets as $asset) {
              if ($asset['attributes']['object_type'] == 'full_length') {
                $episode['player'] = $asset;
              }
            }
            $episodes[] = $special;
            $special_index++;
            if ($special_index < $special_count) {
              $date_tracker = strtotime($specials[$special_index]['attributes']['premiered_on']);
            }

            // Return the list if we've hit the limit.
            if (count($episodes) >= $count) {
              return $episodes;
            }
          }
        }

        // Load the asset list for each episode to find the player and links.
        $episode_assets = pbs_media_manager_request('asset', $episode['id'], 'episode');

        foreach ($episode_assets as $asset) {
          if ($asset['attributes']['object_type'] == 'full_length') {
            $episode['player'] = $asset;
          }
        }

        $episodes[] = $episode;

        if (count($episodes) >= $count) {
          return $episodes;
        }
      }
    }

    // At this point, if there aren't enough shows on the list, add any
    // remaining specials.
    if ($include_specials) {
      foreach ($specials as $special) {
        $special_assets = pbs_media_manager_request('asset', $special['id'], 'special');
        foreach ($special_assets as $asset) {
          if ($asset['attributes']['object_type'] == 'full_length') {
            $special['player'] = $asset;
          }
        }

        $episodes[] = $special;
        if (count($episodes) >= $count) {
          return $episodes;
        }
      }
    }

    // Finally whatever we have is good enough.
    return $episodes;
  }

  /**
   * Makes the actual request of the API.
   *
   * @param string $resource
   *   The primary resource to lookup. Defaults to the show itself.
   *
   * @return bool|object
   *   Returns the response object or False.
   */
  private function makeRequest($resource = 'show') {
    module_load_include('module', 'pbs_media_manager');

    if ($resource == 'show') {
      return pbs_media_manager_request('show', $this->pbs_media_manager_show_id);
    }

    return pbs_media_manager_request($resource, $this->pbs_media_manager_show_id, 'show');
  }

}
