<?php
/**
 * @file
 * example_booking_structure.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function example_booking_structure_user_default_roles() {
  $roles = array();

  // Exported role: company.
  $roles['company'] = array(
    'name' => 'company',
    'weight' => '3',
  );

  // Exported role: company manager.
  $roles['company manager'] = array(
    'name' => 'company manager',
    'weight' => '5',
  );

  // Exported role: company operator.
  $roles['company operator'] = array(
    'name' => 'company operator',
    'weight' => '6',
  );

  // Exported role: company staff.
  $roles['company staff'] = array(
    'name' => 'company staff',
    'weight' => '7',
  );

  // Exported role: member.
  $roles['member'] = array(
    'name' => 'member',
    'weight' => '4',
  );

  return $roles;
}
