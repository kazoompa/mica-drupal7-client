<?php
/**
 * @file
 * Obiba Mica Graphics variable file
 */

/**
 * Implements hook_variable_info().
 */
function obiba_mica_graphic_variable_info($options) {

  $variable['graphic_label'] = array(
    'title' => t('Statistics graphic Label'),
    'description' => t('The Statistics graphic section title'),
    'type' => 'string',
    'default' => 'Catalogue Statistics graphic',
  );

  $variable['graphics_blocks'] = array(
    'title' => t('Statistics graphic bocks'),
    'description' => t('Enable the Statistics graphic in some blocks'),
    'type' => 'boolean',
    'default' => TRUE,
  );

  $variable['graphics_blocks_networks'] = array(
    'title' => t('Concerned Networks Statistics'),
    'description' => t('Please select the Concerned networks'),
    'type' => 'options',
    'default' => 'none',
  );

  $variable['graphics_blocks_aggregations'] = array(
    'title' => t('Aggregations statistics'),
    'description' => t('Please select the Aggregations statistics to show'),
    'type' => 'options',
    'default' => 'none',
  );

  $variable[NetworkStatistics::GEO_NBR_STAT.'_css'] = array(
    'title' => t('Specific CSS of the Country of residence geo statistics block'),
    'description' => t('Please select the specific CSS of the Country of residence geo statistics block, multiple classes must be separated by comma \',\'.'),
    'type' => 'string',
    'default' => '',
  );

  $variable[NetworkStatistics::RECRUITMENT_SOURCES.'_css'] = array(
    'title' => t('Specific CSS of the Sources of recruitment statistics block'),
    'description' => t('Please select the Sources of recruitment statistics block, multiple classes must be separated by comma \',\'.'),
    'type' => 'string',
    'default' => '',
  );

  $variable[NetworkStatistics::BIO_SAMPLES.'_css'] = array(
    'title' => t('Specific CSS of the Biological Samples statistics block'),
    'description' => t('Please select the Biological Samples statistics block, multiple classes must be separated by comma \',\'.'),
    'type' => 'string',
    'default' => '',
  );

  $variable[NetworkStatistics::METHODS_DESIGNS.'_css'] = array(
    'title' => t('Specific CSS of the Study design statistics block'),
    'description' => t('Please select the Study design statistics block, multiple classes must be separated by comma \',\'.'),
    'type' => 'string',
    'default' => '',
  );

  $variable[NetworkStatistics::RECRUITMENT_SOURCES.'_css'] = array(
    'title' => t('Specific CSS of the Recruitment target statistics block'),
    'description' => t('Please select the Recruitment target statistics block, multiple classes must be separated by comma \',\'.'),
    'type' => 'string',
    'default' => '',
  );

  $variable[NetworkStatistics::ACCESS.'_css'] = array(
    'title' => t('Specific CSS of the Access Permitted statistics block'),
    'description' => t('Please select the Access Permitted statistics block, multiple classes must be separated by comma \',\'.'),
    'type' => 'string',
    'default' => '',
  );


  $variable['graphics_content'] = array(
    'title' => t('Statistics graphic in networks page '),
    'description' => t('Enable the Statistics graphic in networks page'),
    'type' => 'boolean',
    'default' => TRUE,
  );

  return $variable;
}