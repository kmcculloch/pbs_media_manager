<?php

namespace Drupal\pbs_media_manager;

/**
 * Class PbsMediaManagerRequest handles requests to media manager api.
 *
 * @package Drupal\pbs_media_manager
 */
class PbsMediaManagerResponse {

  protected $rawJson;
  protected string $apiVersion;
  protected array $links;
  protected array $meta;
  protected array $data;

  public function __construct($rawResponse) {
    $this->rawJson = json_decode($rawResponse);
    $this->apiVersion = $this->rawJson->jsonapi->version;
    $this->links = $this->rawJson->links;
    $this->meta = $this->rawJson->meta;
    $this->data = $this->rawJson->data;
  }

  public function getApiVersion() {
    return $this->apiVersion;
  }

  public function getLinks() {
    return $this->links;
  }

  public function getMetaData() {
    return $this->meta;
  }

  public function getFullData() {
    return $this->data;
  }

  public function getCompleteResponse() {
    return $this->rawJson;
  }

}
