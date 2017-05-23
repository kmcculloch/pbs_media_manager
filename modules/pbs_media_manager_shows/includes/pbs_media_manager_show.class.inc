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

}
