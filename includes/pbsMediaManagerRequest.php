<?php

/**
 * Class PbsMediaManagerRequest handles requests to media manager api.
 *
 * @package Drupal\pbs_media_manager
 */
class PbsMediaManagerRequest {

  protected $endpoint;
  protected $resource;
  protected $mainResource;
  protected $method;
  protected $parameters;
  protected $resourceId;
  protected $key;
  protected $secret;

  public function __construct() {
    $config = pbs_media_manager_get_config();
    $this->key = (empty($api_key)) ? $config['api_key'] : $api_key;
    $this->secret = (empty($api_secret)) ? $config['api_secret'] : $api_secret;
    $this->endpoint = (empty($endpoint)) ? $config['endpoint'] : $endpoint;

  }

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
    if (empty($this->mainResource)) {
      $this->mainResource($resourceName);
    }
    $this->resource = $resourceName;
    return $this;
  }

  public function setParentResource($resourceName) {
    $this->resource = $this->mainResource;
    $this->mainResource = $resourceName;
  }

  public function setMethod($methodName) {
    $this->method = strtolower($methodName);
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

    if (empty($this->endpoint) || empty($this->api_key) || empty($this->api_secret)) {
      watchdog('pbs_media_manager', 'API endpoint, key, and secret are all required to make a request.', array(), WATCHDOG_ERROR, PBS_MEDIA_MANAGER_SETTING_URL);
    }

    // There are three primary structures to requests from Media Manager, two
    // for getting lists, and one for getting individual resources.
    // Lists:
    // https://media.services.pbs.org/api/v1/episodes/
    // https://media.services.pbs.org/api/v1/seasons/{season_id}/episodes/
    // Individual Resources
    // https://media.services.pbs.org/api/v1/episodes/[id|slug]/
    //
    // When we are getting a list $this->mainResource is the first noun, and
    // $this->resource is the second (which is the one being returned).
    // Currently only get and list are supported.
    // TODO: add support for more verbs.
    $elements = array(
      $this->endpoint,
      $this->mainResource,
      $this->resourceId,
    );

    if ($this->method == 'list' && $this->resource != $this->mainResource) {
      $elements[] = $this->resource;
    }

    $uri = implode('/', $elements);

    if (!empty($this->parameters)) {
      $uri .= '?' . http_build_query($this->parameters);
    }

    // Assemble the authentication header for basic auth.
    $authorization = base64_encode($api_key . ':' . $api_secret);
    $options['headers'] = array(
      'Content-Type' => 'application/json',
      'Authorization' => 'Basic ' . $authorization,
    );

    $response = drupal_http_request($uri, $options);

    if ($response->code === '200') {
      return new PbsMediaManagerResponse($response->data);
    }
    else {
      watchdog(
        'pbs_media_manager',
        'PBS Media Manager API returned %code. Message %error.',
        array(
          '%code' => $response->code,
          '%error' => $response->error,
        ), WATCHDOG_ERROR);
      return FALSE;
    }
  }

}
