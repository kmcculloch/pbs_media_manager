<?php

/**
 * Class PbsMediaManagerRequest handles requests to media manager api.
 *
 * @package Drupal\pbs_media_manager
 */
class PbsMediaManagerResponse {

  protected $rawJson;
  protected $apiVersion;
  protected $links;
  protected $meta;
  protected $data;

  /**
   * PbsMediaManagerResponse constructor.
   *
   * @param string $rawResponse
   *   Raw string of JSON from PBS Media Manager.
   */
  public function __construct($rawResponse) {
    $this->rawJson = json_decode($rawResponse);
    $this->apiVersion = $this->rawJson->jsonapi->version;
    $this->links = $this->rawJson->links;
    $this->meta = $this->rawJson->meta;
    $this->data = $this->rawJson->data;
  }

  /**
   * Gets the API version value sent in the response.
   *
   * @return string
   *   API version value sent in the response.
   */
  public function getApiVersion() {
    return $this->apiVersion;
  }

  /**
   * Returns the list of links from Media Manager.
   *
   * @return array
   *   An array of the list returned in the "links" section of the response.
   */
  public function getLinks() {
    return $this->links;
  }

  /**
   * Provides the data from the meta data section of the response.
   *
   * @return array
   *   An array of the meta data valued keyed by field name.
   */
  public function getMetaData() {
    return $this->meta;
  }

  /**
   * Returns the full data section of the response.
   *
   * This is the main data returned by the API. It will either be a list of
   * the requested resources or a collection of fields for a specific resource.
   *
   * @return array
   *   The data section of response.
   */
  public function getFullData() {
    return $this->data;
  }

  /**
   * Provides the file decoded JSON object of the response.
   *
   * @return mixed
   *   The full JSON object.
   */
  public function getCompleteResponse() {
    return $this->rawJson;
  }

}
