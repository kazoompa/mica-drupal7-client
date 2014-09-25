<?php

/**
 * @file
 * MicaStudyResource class
 */

$path_module_protobuf = drupal_get_path('module', 'obiba_protobuf');
$path_module_protos = drupal_get_path('module', 'mica_client_model');

include_once($path_module_protobuf . '/protobuf/Protobuf.php');
use \DrSlump\Protobuf;

Protobuf::autoload();

include_once($path_module_protos . '/protos/Mica.php');
include_once($path_module_protos . '/protos/MicaSearch.php');

/**
 * Class MicaStudyResource
 */
class MicaDatasetResource extends MicaClient {

  public function __construct($mica_url = NULL) {
    parent::__construct($mica_url);
    Protobuf::setDefaultCodec(new Protobuf\Codec\Json());
  }

  /**
   * Get the list of datasets.
   *
   * @param string $resource "datasets" or "harmonization-datasets" or "study-datasets"
   * @param null $study_id
   * @param null $query
   * @param int $from
   * @param $limit
   * @param null $sort field name to sort by
   * @param null $order "asc" or "desc"
   * @return array
   */
  public function get_datasets($resource = 'datasets', $study_id = NULL, $query = NULL, $from = 0, $limit = -1, $sort = NULL, $order = NULL) {
    $this->setLastResponse(NULL);
    $params = empty($from) ? 'from=0' : 'from=' . $from;
    $params .= empty($limit) ? '&limit=-1' : '&limit=' . $limit;
    $params .= empty($sort) ? '' : '&sort=' . $sort;
    $params .= empty($order) ? '' : '&order=' . $order;
    $params .= empty($query) ? '' : '&query=' . $query;
    $params .= empty($study_id) ? '' : '&study=' . $study_id;
    $url_datasets = $this->micaUrl . '/' . (empty($resource) ? 'datasets' : $resource) . '?' . $params;
    //dpm($url_study_datasets);
    $request = new HttpClientRequest($url_datasets, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON),
          )
        )
    ));
    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $dtos = array();
      return json_decode($data);
    } catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get a dataset by its ID.
   *
   * @param string $resource "dataset" or "harmonization-dataset" or "study-dataset"
   * @param $dataset_id
   * @return array|Protobuf\Message|null
   */
  public function get_dataset($resource = 'dataset', $dataset_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/' . (empty($resource) ? 'dataset' : $resource) . '/' . $dataset_id;

    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON),
          )
        ),
    ));

    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $dtoObj = '\obiba\mica\DatasetDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    } catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the list of variables of the dataset.
   *
   * @param $resource "dataset" or "harmonization-dataset" or "study-dataset"
   * @param $dataset_id
   * @param int $from
   * @param $limit
   * @param null $sort field name to sort by
   * @param null $order "asc" or "desc"
   * @return array|Protobuf\Message|mixed|null
   */
  public function get_variables($resource, $dataset_id, $from = 0, $limit = -1, $sort = NULL, $order = NULL) {
    $this->setLastResponse(NULL);
    $params = empty($from) ? 'from=0' : 'from=' . $from;
    $params .= empty($limit) ? '&limit=-1' : '&limit=' . $limit;
    $params .= empty($sort) ? '' : '&sort=' . $sort;
    $params .= empty($order) ? '' : '&order=' . $order;

    $url = $this->micaUrl . '/' . (empty($resource) ? 'dataset' : $resource) . '/' . $dataset_id . '/variables?' . $params;
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON),
          )
        ),
    ));
    $client = $this->client();
    try {
      $data = $client->execute($request);

      $this->setLastResponse($client->lastResponse);
      if (!empty($detail)) {
        return json_decode($data);
      }
      $dtoObj = '\obiba\mica\DatasetVariablesDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    } catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the list variable harmonization status for a harmonization dataset.
   *
   * @param $resource "dataset" or "harmonization-dataset" or "study-dataset"
   * @param $dataset_id
   * @param int $from
   * @param $limit
   * @param null $sort field name to sort by
   * @param null $order "asc" or "desc"
   * @return array|Protobuf\Message|null
   */
  public function get_variables_harmonizations($resource, $dataset_id, $from = 0, $limit = -1, $sort = NULL, $order = NULL) {
    $this->setLastResponse(NULL);
    $params = empty($from) ? 'from=0' : 'from=' . $from;
    $params .= empty($limit) ? '&limit=-1' : '&limit=' . $limit;
    $params .= empty($sort) ? '' : '&sort=' . $sort;
    $params .= empty($order) ? '' : '&order=' . $order;

    $url = $this->micaUrl . '/' . (empty($resource) ? 'dataset' : $resource) . '/' . $dataset_id . '/variables/harmonizations?' . $params;
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON),
          )
        ),
    ));
    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $dtoObj = '\obiba\mica\DatasetVariablesHarmonizationsDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    } catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the variable by its ID.
   *
   * @param $variable_id
   * @return array|Protobuf\Message|null
   */
  public function get_variable($variable_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/variable/' . $variable_id;
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON),
          )
        ),
    ));
    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $dtoObj = '\obiba\mica\DatasetVariableDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    } catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the facet (=summary from ES) of the variable.
   *
   * @param $variable_id
   * @return array|mixed
   */
  public function get_variable_facet($variable_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/variable/' . $variable_id . '/facet';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON),
          )
        ),
    ));
    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      return json_decode($data);
    } catch (HttpClientException $e) {
      return array();
    }
  }

  /**
   * Get the harmonizations of a dataschema variable.
   *
   * @param $variable_id
   * @return array|Protobuf\Message|null
   */
  public function get_variable_harmonizations($variable_id) {
    $this->setLastResponse(NULL);
    $url = $this->micaUrl . '/variable/' . $variable_id . '/harmonizations';
    $request = new HttpClientRequest($url, array(
      'method' => HttpClientRequest::METHOD_GET,
      'headers' => $this->authorizationHeader(array(
            'Accept' => array(parent::HEADER_JSON),
          )
        ),
    ));
    $client = $this->client();
    try {
      $data = $client->execute($request);
      $this->setLastResponse($client->lastResponse);
      $dtoObj = '\obiba\mica\DatasetVariableHarmonizationDto';
      return $data ? Protobuf::decode($dtoObj, $data) : NULL;
    } catch (HttpClientException $e) {
      return array();
    }
  }

  // TODO @samir see if we need this method anymore
//  public function variables_search($spec_dto_params = NULL, $size = NULL) {
//    $this->setLastResponse(NULL);
//    $size_param = is_null($size) ? parent::SIZE_RESPONSE : $size;
//    $url = $this->micaUrl . '/variables/_search?size=' . $size_param;
//    $url_parm = explode('?', $_SERVER['REQUEST_URI']);
//    if (empty($url_parm[1])) {
//
//      $dto_param = $this->dto_filter_construct(NULL, NULL, 'variable', $spec_dto_params, $size_param);
//      if (empty(json_decode($dto_param)->filteredQuery->filter)) {
//        $url = $this->micaUrl . '/variables/_search?from=' . json_decode($dto_param)->from . '&size=' . $size_param;
//
//        $request = new HttpClientRequest($url, array(
//          'method' => HttpClientRequest::METHOD_GET,
//          'headers' => $this->authorizationHeader(array(
//                'Accept' => array(parent::HEADER_JSON),
//              )
//            ),
//        ));
//      }
//      else {
//        $request = new HttpClientRequest($url, array(
//          'method' => HttpClientRequest::METHOD_POST,
//          'headers' => $this->authorizationHeader(array(
//                'Accept' => array(parent::HEADER_JSON),
//                'Content-Type' => array(parent::HEADER_JSON)
//              )
//            ),
//          'data' => $dto_param,
//        ));
//      }
//    }
//    else {
//      $dto_param = $this->dto_filter_construct($url_parm[1], array('parent' => 'Study'), 'variable', $spec_dto_params, $size_param);
//      if (empty(json_decode($dto_param)->filteredQuery->filter)) {
//        $url = $this->micaUrl . '/variables/_search?from=' . json_decode($dto_param)->from . '&size=' . $size_param;
//
//        $request = new HttpClientRequest($url, array(
//          'method' => HttpClientRequest::METHOD_GET,
//          'headers' => $this->authorizationHeader(array(
//                'Accept' => array(parent::HEADER_JSON),
//              )
//            ),
//        ));
//      }
//      else {
//        $request = new HttpClientRequest($url, array(
//          'method' => HttpClientRequest::METHOD_POST,
//          'headers' => $this->authorizationHeader(array(
//                'Accept' => array(parent::HEADER_JSON),
//                'Content-Type' => array(parent::HEADER_JSON)
//              )
//            ),
//          'data' => $dto_param,
//        ));
//      }
//    }
//    $client = $this->client();
//    try {
//      $data = $client->execute($request);
//      $this->setLastResponse($client->lastResponse);
//      $dto_data_response = json_decode($data);
//      $this->session_facets_builder($dto_data_response);
//
//      //dpm($dto_data_response);
//      $variable_summaries = array();
//      if ($dto_data_response->totalHits > 0 && $size_param > 0) {
//        $variable_summaries = $dto_data_response->{'obiba.mica.DatasetVariableResultDto.result'}->summaries;
//      }
//      $dtoObj = '\obiba\mica\DatasetVariableResolverDto';
//      $response = array();
//      if (!empty($variable_summaries)) {
//        foreach ($variable_summaries as $variable) {
//          $response ['data'][] = Protobuf::decode($dtoObj, json_encode($variable));
//        }
//      }
//      $response['totalHits'] = $dto_data_response->totalHits;
//      if (!empty($dto_data_response->aggs)) {
//        $response['aggs'] = $dto_data_response->aggs;
//      }
//      return $response;
//    } catch (HttpClientException $e) {
//      return array('totalHits' => 0);
//    }
//  }


}