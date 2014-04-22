<?php
/**
 * @file
 * example_booking_structure.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function example_booking_structure_taxonomy_default_vocabularies() {
  return array(
    'classes_types' => array(
      'name' => 'Classes Types',
      'machine_name' => 'classes_types',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'duration' => array(
      'name' => 'Duration',
      'machine_name' => 'duration',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'sports_categories' => array(
      'name' => 'Sports Categories',
      'machine_name' => 'sports_categories',
      'description' => 'Sports Categories',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
    'type' => array(
      'name' => 'Type',
      'machine_name' => 'type',
      'description' => '',
      'hierarchy' => '0',
      'module' => 'taxonomy',
      'weight' => '0',
    ),
  );
}
