<?php
/**
 * @file
 * Provides an EntityAPIController for our custom show entities.
 */

/**
 * Extending the EntityAPIController for the pbs_media_managerShow entity.
 */
class PbsMediaShowEntityController extends EntityAPIController {

  /**
   * Creates the content for this entity.
   *
   * @param $entity
   * @param string $view_mode
   * @param null $langcode
   * @param array $content
   *
   * @return array
   */
  public function buildContent($entity, $view_mode = 'full', $langcode = NULL, $content = array()) {

    $build = parent::buildContent($entity, $view_mode, $langcode, $content);

    return $build;

  }

  /**
   * Overrides EntityAPIController::load().
   *
   * @param $ids
   *   The list of IDs to load passed to parent.
   * @param $conditions
   *   List of conditions passed to parent .
   *
   * @returns array
   *   Returns an array of entities.
   */
  public function load($ids = array(), $conditions = array()) {
    $entities = parent::load($ids, $conditions);

    // Unserialize the image field.
    if (is_array($entities)) {
      foreach ($entities as $show) {
        $images = unserialize($show->images);
        $show->images = $images;
      }
    }

    return $entities;
  }

  /**
   * Overrides EntityAPIControllerInterface.
   *
   * @param DatabaseTransaction $transaction
   *   Optionally a DatabaseTransaction object to use. Allows overrides to pass
   *   in their transaction object.
   *
   * @return
   *   SAVED_NEW or SAVED_UPDATED is returned depending on the operation
   *   performed.
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
class PbsMediaShowEntity extends Entity {

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
   *
   * @return array
   *   Returns an array of Episode records.
   */
  public function getLatestEpisodes($count) {
    $seasons = $this->getSeasons();

    $total = count($seasons);
    $episodes = array();

    // Sort the seasons by ordinal value.
    usort($seasons, function ($a, $b) {
      return $a['attributes']['ordinal'] < $b['attributes']['ordinal'];
    });

    // For each seasons load the list of episodes from MM and build up the
    // the episode list until it reaches count.  Then return list.
    foreach ($seasons as $season) {

      // Load the season's episodes.
      $season_episodes = pbs_media_manager_request('episode', $season['id'], 'season', '', array('sort' => '-ordinal'));

      foreach ($season_episodes as $episode) {
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
  }

  /**
   * Makes the actual request of the API.
   *
   * @param string $resource
   *   The primary resource to lookup. Defaults to the show itself.
   *
   * @return bool|object
   */
  private function makeRequest($resource = 'show') {
    module_load_include('module', 'pbs_media_manager');

    if ($resource == 'show') {
      return pbs_media_manager_request('show', $this->pbs_media_manager_show_id);
    }

    return pbs_media_manager_request($resource, $this->pbs_media_manager_show_id, 'show');

  }

}
