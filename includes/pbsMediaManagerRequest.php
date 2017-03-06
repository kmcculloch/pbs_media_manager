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

  /**
   * PbsMediaManagerRequest constructor.
   *
   * If there are setting already entered through the settings form they are
   * preloaded during construction.  They can be overriden via setters.
   */
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

  /**
   * Sets the API key overriding whatever settings were entered.
   *
   * @param string $apiKey
   *   The API key from PBS.
   *
   * @return $this
   */
  public function setKey($apiKey) {
    $this->key = $apiKey;
    return $this;
  }

  /**
   * Sets the API Secret value overriding whatever settings were entered.
   *
   * @param string $apiSecret
   *   The API Secret value.
   *
   * @return $this
   */
  public function setSecret($apiSecret) {
    $this->secret = $apiSecret;
    return $this;
  }

  /**
   * Sets the resource to retrieve.
   *
   * The response (assuming data is returned) will either be this type of object
   * or a list of this type of object.
   *
   * @param string $resourceName
   *   The resource name to be found in the Media Manager API.
   *
   * @return $this
   */
  public function setResource($resourceName) {
    if (empty($this->mainResource)) {
      $this->mainResource($resourceName);
    }
    $this->resource = $resourceName;
    return $this;
  }

  /**
   * Sets the parent resource to search.
   *
   * If provided it will filter the list of resources being sought by this
   * resource type. E.g if resource is Episodes and parent resource is Series it
   * will return a list of episodes from within that series.
   *
   * @param string $resourceName
   *   The name of the parent resource to search.
   */
  public function setParentResource($resourceName) {
    $this->resource = $this->mainResource;
    $this->mainResource = $resourceName;
  }

  /**
   * Set the method you wish to call.
   *
   * Currently only get and list are supported, and are automatically detected
   * based on other values.  This method if mostly for future expansion to
   * handle create, update, and delete requests.
   *
   * @param string $methodName
   *   The name of the method. Currently get or list.
   *
   * @return $this
   */
  public function setMethod($methodName) {
    $this->method = strtolower($methodName);
    return $this;
  }

  /**
   * Set additional parameters.
   *
   * Provide an array of parameters to include in the query string of the
   * request.
   *
   * @param array $parameters
   *   Parameter list to be used in the query.
   *
   * @return $this
   */
  public function setParameters(array $parameters) {
    $this->parameters = $parameters;
    return $this;
  }

  /**
   * Sets the resource ID for get requests, or list requests with a parent.
   *
   * If no parent resource is set, this will force a get request of the primary
   * resource. If a parent is sit, this will force a list request within that
   * parent (so the ID will be assumed to be the parent resource). If not set
   * the search will be assumed to be a list request of the primary resource.
   *
   * @param string $resourceId
   *   The ID or Slug to use in the request.
   *
   * @return $this
   */
  public function setResourceId($resourceId) {
    $this->resourceId = $resourceId;
    return $this;
  }

  /**
   * Run the actual request and return a response object.
   *
   * @return bool|\PbsMediaManagerResponse
   *   Returns a response object or FALSE if nothing is found.
   *   TODO: throw an exception instead of returning FALSE on error.
   */
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
    $authorization = base64_encode($this->key . ':' . $this->secret);
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
