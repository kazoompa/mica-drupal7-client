<?php

/**
 * @file
 * AgateClient class
 */

$path_module_protobuf = drupal_get_path('module', 'obiba_protobuf');
$path_module_protos = drupal_get_path('module', 'mica_client_model');

include_once($path_module_protobuf . '/protobuf/Protobuf.php');
use \DrSlump\Protobuf;

Protobuf::autoload();

include_once($path_module_protos . '/protos/Mica.php');
include_once($path_module_protos . '/protos/MicaSearch.php');
include_once("mica_client_facet_search_query_dto_builders.inc");

/**
 * Class MicaClient
 */
class MicaClient {
  const child = \obiba\mica\ParentChildFilterDto\Relationship::CHILD;
  const parent = \obiba\mica\ParentChildFilterDto\Relationship::PARENT;
  const AUTHORIZATION_HEADER = 'Authorization';

  const COOKIE_HEADER = 'Cookie';

  const SET_COOKIE_HEADER = 'Set-Cookie';

  const OBIBA_COOKIE = 'obibaid';

  const MICA_COOKIE = 'micasid';

  const HEADER_BINARY = 'application/x-protobuf';

  const HEADER_JSON = 'application/json';

  const SIZE_RESPONSE = 20;

  const PAGINATE_studies = 0;
  const PAGINATE_variables = 1;
  const PAGINATE_datasets = 2;
  const PAGINATE_networks = 3;

  protected $micaUrl;

  private $lastResponse;

  /**
   * Mica client from a given url or from the one retrieved from 'mica_url' variable.
   * @param null $mica_url
   */
  public function __construct($mica_url = NULL) {
    $this->micaUrl = (isset($mica_url) ? $mica_url : variable_get_value('mica_url')) . '/ws';
  }

  /**
   * Get the last response (if any).
   * @return mixed
   */
  public function getLastResponse() {
    return $this->lastResponse;
  }

  /**
   * Set the last response after a http call.
   * @param $lastResponse
   */
  protected function setLastResponse($lastResponse) {
    $this->lastResponse = $lastResponse;
    if (isset($lastResponse)) {
      $this->setLastResponseCookies();
    }
  }

  /**
   * Get the last response headers (if any).
   * @return array
   */
  protected function getLastResponseHeaders() {
    if ($this->lastResponse != NULL) {
      $result = array();
      foreach (explode("\r\n", $this->lastResponse->headers) as $header) {
        $h = explode(":", $header, 2);
        if (count($h) == 2) {
          if (!array_key_exists($h[0], $result)) {
            $result[$h[0]] = array();
          }
          array_push($result[$h[0]], trim($h[1]));
        }
      }
      return $result;
    }
    return '';
  }

  /**
   * Get the header value(s) from the last response.
   * @param $header_name
   * @return array
   */
  protected function getLastResponseHeader($header_name) {
    $headers = $this->getLastResponseHeaders();

    if (array_key_exists($header_name, $headers)) {
      return $headers[$header_name];
    }
    return array();
  }

  /**
   * Forwards the 'Set-Cookie' directive(s) to the drupal client if the user was authenticated by Agate.
   */
  private function setLastResponseCookies() {
    foreach ($this->getLastResponseHeader(self::SET_COOKIE_HEADER) as $cookie_str) {
      $cookie = $this->parseCookie($cookie_str);
      $keys = array_keys($cookie);
      $name = $keys[0];
      $value = $cookie[$name];
      $expire = isset($cookie['Max-Age']) ? time() + intval($cookie['Max-Age']) : 0;
      $path = isset($cookie['Path']) ? $cookie['Path'] : '/';
      $domain = isset($cookie['Domstudyain']) ? $cookie['Domain'] : NULL;

      if (isset($_COOKIE[self::OBIBA_COOKIE])) {
        // forwards cookies to client if not anonymous user
        setcookie($name, $value, $expire, $path, $domain);
      }
      $_SESSION[$name] = $value;
    }
  }

  /**
   * Add authorization headers
   * @param array $headers
   * @return array
   */
  protected function authorizationHeader(array $headers) {
    if (isset($_COOKIE[self::OBIBA_COOKIE])) {
      // authenticate by cookies coming from request (case of regular user logged in via Agate)
      watchdog('obiba_mica', 'Auth by cookies from request');
      $obibaid = $_COOKIE[self::OBIBA_COOKIE];
      $micasid = isset($_COOKIE[self::MICA_COOKIE]) ? $_COOKIE[self::MICA_COOKIE] : NULL;
      $cookie = $this->cookieHeaderValue($obibaid, $micasid);

      if (isset($headers[self::COOKIE_HEADER])) {
        array_push($headers[self::COOKIE_HEADER], $cookie);
      }
      else {
        $headers[self::COOKIE_HEADER] = array($cookie);
      }
    }
    else {
      // authenticate by anonymous credentials
      watchdog('obiba_mica', 'Auth by anonymous credentials');
      $credentials = variable_get_value('mica_anonymous_name') . ':' . variable_get_value('mica_anonymous_password');
      $headers[self::AUTHORIZATION_HEADER] = array('Basic ' . base64_encode($credentials));
    }

    return $headers;
  }

  /**
   * Add authorization by cookies header.
   * @param array $headers
   * @param $obibaid
   * @param $micasid
   * @return string
   */
  private function cookieHeaderValue($obibaid, $micasid) {
    $cookie = self::OBIBA_COOKIE . '=' . $obibaid;

    if (isset($micasid)) {
      $cookie = $cookie . '; ' . self::MICA_COOKIE . '=' . $micasid;
    }

    return $cookie;
  }

  /**
   * Explode a cookie string in a array.
   * @param $cookie_str
   * @return array
   */
  private function parseCookie($cookie_str) {
    $cookie = array();
    foreach (explode(';', $cookie_str) as $entry_str) {
      $entry = explode('=', $entry_str);
      $cookie[$entry[0]] = $entry[1];
    }
    return $cookie;
  }

  /**
   * @return HttpClient
   */
  protected function client() {
    $client = new HttpClient($formatter = FALSE);

    if (!isset($client->options['curlopts'])) {
      $client->options['curlopts'] = array();
    }

    $client->options['curlopts'] += array(
      CURLOPT_SSLVERSION => 3,
      CURLOPT_SSL_VERIFYHOST => FALSE,
      CURLOPT_SSL_VERIFYPEER => FALSE
    );

    Protobuf::setDefaultCodec(new Protobuf\Codec\Json());

    return $client;
  }


  protected function session_facets_builder($aggregations) {
    foreach ($aggregations as $type_aggregations => $aggregation) {
      if (!empty($aggregation->totalHits)) {
        if ($type_aggregations == 'variableResultDto') {
          $_SESSION['variables']['studies'] = $aggregations->studyResultDto->{'obiba.mica.StudyResultDto.result'}->summaries;
          $_SESSION['variables']['datasets'] =
            !empty($aggregations->datasetResultDto->{'obiba.mica.DatasetResultDto.result'}->datasets) ?
              $aggregations->datasetResultDto->{'obiba.mica.DatasetResultDto.result'}->datasets : NULL;
          $_SESSION['variables']['aggregations'] = $aggregation->aggs;
          $_SESSION['variables']['aggregations'][] = $aggregation->aggs;
          $_SESSION['variables']['aggregations']['totalHits'] = $aggregation->totalHits;
          $_SESSION['variables']['aggregations']['totalCount'] = $aggregation->totalCount;
        }

        if ($type_aggregations == 'studyResultDto') {
          $_SESSION['studies']['networks'] =
            !empty($aggregations->networkResultDto->{'obiba.mica.NetworkResultDto.result'}->networks) ?
              $aggregations->networkResultDto->{'obiba.mica.NetworkResultDto.result'}->networks : NULL;
          $_SESSION['studies']['aggregations'] = $aggregation->aggs;
          $_SESSION['studies']['aggregations']['totalHits'] = $aggregation->totalHits;
          $_SESSION['studies']['aggregations']['totalCount'] = $aggregation->totalCount;
        }
      }
    }
  }

  static function get_name_entity_by_id_aggregation($type, $agregation_facet, $id) {
    foreach ($_SESSION[$type][$agregation_facet] as $entity) {
      if ($entity->id == $id) {
        // dpm(mica_client_commons_get_localized_field($dataset,'name'));
        return mica_client_commons_get_localized_field($entity, 'name');
      }
    }
//dpm($type);
//dpm($id);

    return NULL;
  }

  public function pagination_list_search_parameters($current_page = 0, $type = NULL, $size = self::SIZE_RESPONSE) {
    $nb_pages = ceil($_SESSION[strtolower($type)]['aggregations']['totalHits'] / $size);
    if (!empty($current_page)) {
      $pageActuelle = intval($current_page);
      if ($pageActuelle > $nb_pages) {
        $pageActuelle = $nb_pages;
      }
    }
    else {
      $pageActuelle = 1;
    }
    $from = ($pageActuelle) * $size;
    return $from;
  }

  /**
   * Dto filter construction
   * @param $type
   * @param null $request_param url passed parameters,
   * @param $from
   * @internal param null $hierarchical_relation
   * @internal param null $type_dto_search must be 'study' or 'dataset',
   * @internal param null $spec_dto_params specific passed parameters,
   * @internal param null $size number of results
   * @internal param null $parent_child array must be 'parent'=>'<entity parent>' or 'child'=>'<entity parent>',
   * @return string
   */
  protected function dto_filter_construct($type, $request_param = NULL, $from = 0) {
    if ($from == 0) {
      unset($_COOKIE['page_' . $type]);
      setcookie('page_' . $type, 0, time() - 3600); // empty value and old timestamp
    }
    else {
      setcookie('page_' . $type, $_GET['page'], time() + 3600 * 24 * 180);
    }
    $queryDtoBuilder = JoinQueryDtoBuilder::newBuilder(json_decode($request_param), $this::SIZE_RESPONSE);
    Protobuf::setDefaultCodec(new Protobuf\Codec\Json);
    $json_query_encoded = //
      Protobuf::encode($queryDtoBuilder->documentQuery($type, $from, $this::SIZE_RESPONSE)->build());
    return $json_query_encoded;
  }

}