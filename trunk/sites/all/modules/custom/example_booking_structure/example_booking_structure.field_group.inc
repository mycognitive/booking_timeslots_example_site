<?php
/**
 * @file
 * example_booking_structure.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function example_booking_structure_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_time_ahead_reservation|node|class|default';
  $field_group->group_name = 'group_time_ahead_reservation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'class';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Maximum time ahead that a reservation can be made',
    'weight' => '10',
    'children' => array(
      0 => 'field_time_ahead_rsrv_unit',
      1 => 'field_time_ahead_rsrv_value',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_time_ahead_reservation|node|class|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_time_ahead_reservation|node|class|form';
  $field_group->group_name = 'group_time_ahead_reservation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'class';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Maximum time ahead that a reservation can be made',
    'weight' => '8',
    'children' => array(
      0 => 'field_time_ahead_rsrv_unit',
      1 => 'field_time_ahead_rsrv_value',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_time_ahead_reservation|node|class|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_time_ahead_reservation|node|facility|default';
  $field_group->group_name = 'group_time_ahead_reservation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'facility';
  $field_group->mode = 'default';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Maximum time ahead that a reservation can be made',
    'weight' => '2',
    'children' => array(
      0 => 'field_time_ahead_rsrv_unit',
      1 => 'field_time_ahead_rsrv_value',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
      ),
    ),
  );
  $export['group_time_ahead_reservation|node|facility|default'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_time_ahead_reservation|node|facility|form';
  $field_group->group_name = 'group_time_ahead_reservation';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'facility';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Maximum time ahead that a reservation can be made',
    'weight' => '4',
    'children' => array(
      0 => 'field_time_ahead_rsrv_unit',
      1 => 'field_time_ahead_rsrv_value',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_time_ahead_reservation|node|facility|form'] = $field_group;

  return $export;
}
