<?php
/**
 * @file
 * example_booking_structure.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function example_booking_structure_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function example_booking_structure_node_info() {
  $items = array(
    'class' => array(
      'name' => t('Classes'),
      'base' => 'node_content',
      'description' => t('use this content type to add new classes to a facility that belongs to a venue (venue->sports->facility->class)'),
      'has_title' => '1',
      'title_label' => t('Class Name'),
      'help' => '',
    ),
    'facility' => array(
      'name' => t('Facilities'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Facility'),
      'help' => '',
    ),
    'instructors' => array(
      'name' => t('Instructors'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Instructor Name'),
      'help' => '',
    ),
    'venue' => array(
      'name' => t('Venue'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Venue Profile Name'),
      'help' => '',
    ),
  );
  return $items;
}
