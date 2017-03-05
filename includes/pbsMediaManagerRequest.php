<?php

namespace Drupal\pbs_media_manager;

use Drupal\pbs_media_manager\PbsMediaManagerResponse;

/**
 * Class PbsMediaManagerRequest handles requests to media manager api.
 *
 * @package Drupal\pbs_media_manager
 */
class PbsMediaManagerRequest {

  protected $endpoint;
  protected $resource;
  protected $method;
  protected $parameters;
  protected $resourceId;
  protected $key;
  protected $seceret;

  /**
   * Set the endpoint for requests from this instance.
   *
   * @return PbsMediaManagerRequest
   *   Returns self for chaing.
   */
  public function setEndpoint($endpointUrl) {
    $this->endpoint = $endpointUrl;
    return $this;
  }

  public function setKey($apiKey) {
    $this->key = $apiKey;
    return $this;
  }

  public function setSeceret($apiSeceret) {
    $this->seceret = $apiSeceret;
    return $this;
  }

  public function setResource($resourceName) {
    $this->resource = $resourceName;
    return $this;
  }

  public function setMethod($methodName) {
    $this->method = $methodName;
    return $this;
  }

  public function setParameters(array $parameters) {
    $this->parameters = $parameters;
    return $this;
  }

  public function setResourceId($resoureId) {
    $this->resourceId = $resourceId;
    return $this;
  }

  public function execute() {
    return new PbsMediaManagerResponse();
  }

}
