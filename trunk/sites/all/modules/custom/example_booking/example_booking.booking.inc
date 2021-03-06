<?php

require_once 'example_booking.booking.controller.inc';

/**
 * Entity general information
 */
function example_booking_booking_entity_info(&$info) {
  $info['booking'] = array(
    'label'            => t('Booking'),
    'entity class'     => 'Booking',
    'controller class' => 'BookingController',
    'views controller class' => 'EntityDefaultViewsController',
    'base table'       => 'booking',
    'uri callback'     => 'entity_class_uri',
    'label callback'   => 'entity_class_label',
    'fieldable'        => TRUE,
    'entity keys' => array(
      'id' => 'pid'
    ),
    'static cache' => TRUE,
    'bundles' => array(
      'booking'=> array(
        'label' => 'Booking',
        'admin' => array(
          'path'             => 'admin/structure/booking',
          'access arguments' => array('administer bookings'),
        ),
      ),
    ),
    'view modes' => array(
      'full' => array(
        'label'           => t('Full Booking'),
        'custom settings' =>  FALSE,
      ),
    ),
    'admin ui' => array(
      'path'             => 'admin/structure/booking',
      'file'             => 'example_booking.booking.controller.inc',
      'controller class' => 'BookingUIController',
    ),
      
  );
  
  // Support entity cache module.
  if (module_exists('entitycache')) {
    $info['booking']['field cache']  = FALSE;
    $info['booking']['entity cache'] = TRUE;
  }
}

/**
 * Adds custom fields into the entity.
 * @NOTE (LT): I'm not sure how does it work.
 */
function example_booking_booking_field_extra_fields(&$return) {
  $return['booking']['booking'] = array(
    'form' => array(
      'created' => array(
        'weight'      => 0,
        'label'       => t('Created'),
        'description' => t('Creation date'),
      ),
      'slot_time' => array(
        'weight'      => 0,
        'label'       => t('Date'),
        'description' => t('Date and time of the slot being reserved'),
      ),
      'view_link' => array(
        'label'       => t('View link'),
        'description' => t('View link'),
      ),
      'edit_link' => array(
        'label'       => t('Edit link'),
        'description' => t('Edit link'),
      ),
      'remove_link' => array(
        'label'       => t('Remove link'),
        'description' => t('Remove link'),
      ),
    ),
    'display' => array(
      'created' => array(
        'weight'      => 0,
        'label'       => t('Created'),
        'description' => t('Creation date'),
      ),
      'slot_time' => array(
        'weight'      => 0,
        'label'       => t('Date'),
        'description' => t('Date and time of the slot being reserved'),
      ),
    ),
  );
}

/**
 * Implements hook_entity_property_info_alter().
 */
function example_booking_booking_entity_property_info_alter(&$info) {
  $properties = &$info['booking']['properties'];
  $properties['created'] = array(
    'label' => t("Date created"),
    'type' => 'date',
    'description' => t("The date the node was posted."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer nodes',
    'schema field' => 'created',
  );
  $properties['uid'] = array(
    'label' => t("User"),
    'type' => 'user',
    'description' => t("Id of the user who booked the slot."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer nodes',
    'schema field' => 'uid',
  );
  $properties['slot_time'] = array(
    'label' => t("Date"),
    'type' => 'text',
    'description' => t("Date and time of the slot being reserved."),
    'setter callback' => 'entity_property_verbatim_set',
    'setter permission' => 'administer nodes',
    'schema field' => 'slot_time',
    'computed' => TRUE,
  );
  
  // Links:
  $properties['view_link'] = array(
    'label' => t('View link'),
    'type' => 'text',
    'description' => t('Provide a simple link to view the entity.'),
    'getter callback' => 'example_booking_field_booking_link_view',
    'computed' => TRUE,
  );
  $properties['edit_link'] = array(
    'label' => t('Edit link'),
    'type' => 'text',
    'description' => t('Provide a simple link to edit the entity.'),
    'getter callback' => 'example_booking_field_booking_link_edit',
    'computed' => TRUE,
  );
  $properties['delete_link'] = array(
    'label' => t('Delete link'),
    'type' => 'text',
    'description' => t('Provide a simple link to delete the entity.'),
    'getter callback' => 'example_booking_field_booking_link_delete',
    'computed' => TRUE,
  );  
  
}

function example_booking_field_booking_link_view($x) {
  dsm ($x);
}

function example_booking_field_booking_link_edit($x) {
  dsm ($x);
}

function example_booking_field_booking_link_delete($x) {
  dsm ($x);
}


/**
 * Title of the /admin/structure/booking page.
 */
function example_booking_booking_info() {
  return 'Bookings';
}

/**
 * Implements the uri callback.
 */
function example_booking_booking_uri($entity) {
  return array(
    'path' => 'booking/' . $entity -> id,
  );
}

function example_booking_booking_page_title($entity) {
  return $entity -> pid;
}

/**
 * Page that displays the entity.
 */
function example_booking_booking_page_view($entity, $view_mode = 'full') {
  // Our entity type, for convenience.
  $entity_type = 'booking';
  
  // Start setting up the content.
  $entity->content = array(
    '#view_mode' => $view_mode,
  );
  
  // Build fields content - this is where the Field API really comes in to play.
  // field_attach_prepare_view() lets the fields load any data they need
  // before viewing.
  field_attach_prepare_view($entity_type, array($entity->pid => $entity), $view_mode);
  
  // We call entity_prepare_view() so it can invoke hook_entity_prepare_view()
  // for us.
  entity_prepare_view($entity_type, array($entity->pid => $entity));
  
  
  // Now field_attach_view() generates the content for the fields.
  $entity->content += field_attach_view($entity_type, $entity, $view_mode);

  return $entity -> content;
}
  
/**
 * Form of entity edit.
 */
function example_booking_booking_form_edit($form, &$form_state, $entity) {

  /*
  $form['created'] = array(
    '#type'          => 'value',
    '#date_format'   => 'Y-m-d H:i',
    '#default_value' => $datetime,
  );
  */

  $form['slot_time'] = array(
    '#title'         => t('Booking date'),
    '#type'          => module_exists('date_popup') ? 'date_popup' : 'date_select',
    '#date_format'   => 'Y-m-d H:i',
    '#default_value' => $entity->slot_time,
  );
  
  $form['booking'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );
  
  field_attach_form('booking', $entity, $form, $form_state);
  
  
  $secondary_to_primary_name   = variable_get('example_booking_2nd_to_1st_ref_field');
  $tertiary_to_secondary_name  = variable_get('example_booking_3rd_to_2nd_ref_field');
  
  $form[$secondary_to_primary_name][$form[$secondary_to_primary_name]['#language']]['#attributes'] = array('disabled' => 'disabled');
  $form[$tertiary_to_secondary_name][$form[$tertiary_to_secondary_name]['#language']]['#attributes'] = array('disabled' => 'disabled');
  $form['field_bt_facility'][$form['field_bt_facility']['#language']]['#attributes'] = array( 'disabled' => 'disabled' );
  $form['field_bt_number_of_players'][$form['field_bt_number_of_players']['#language']][0]['value']['#attributes'] = array( 'disabled' => 'disabled' );
  
  // Fix for broken autocomplete path when editing the entity 
  $user_autocomplete_path = &$form['field_user'][$form['field_user']['#language']][0]['target_id']['#autocomplete_path'];
  $user_autocomplete_path = str_replace (arg(1), 'NULL', $user_autocomplete_path);
  
  $form['submit'] = array(
    '#type'    => 'submit',
    '#value'   => t('Save'),
    '#submit' => array('example_booking_booking_edit_save'),
    '#weight'  => 1000,
  );
  
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('example_booking_booking_edit_delete'),
    '#weight' => 1001,
  );

  return $form;
}

/**
 * Form of booking deletion.
 */
function example_booking_booking_form_remove($form, &$form_state, $entity) {
  $form['confirmation'] = array(
    '#type'          => 'item',
    '#title'         => t('Are you sure you want to delete this booking?'),
    '#markup'         => '',
    '#weight'        => 1,
  );
  
  $form['booking'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );
  
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete'),
    '#submit' => array('example_booking_booking_edit_delete'),
    '#weight' => 200,
  );

  return $form;
}

function example_booking_booking_edit_delete($form, &$form_state) {
  $entity = &$form_state['values']['booking'];  
  example_booking_booking_delete($entity);
  $form_state['redirect'] = "admin/bookings";
}

function example_booking_booking_edit_save($form, &$form_state) {
  $entity = $form_state['values']['booking'];  

  if (user_access('administer bookings')) {
    $entity->uid = filter_var($form['field_user'][$form['field_user']['#language']][0]['target_id']['#value'], FILTER_SANITIZE_NUMBER_INT);
  }
  
  $entity->players = $form_state['values']['field_bt_number_of_players']['und'][0]['value'];
  $entity->slot_time = $form_state['values']['slot_time'];

  global $user;
  
  field_attach_submit('booking', $entity, $form, $form_state);
  $booking = example_booking_booking_save($entity);
  
  return $form;
}
  
/**
 * Form of entity creation.
 */
function example_booking_booking_form($form, &$form_state, $entity, $input) {

  list($primary_id, $secondary_id, $tertiary_id, $cid, $datetime, $duration) = $input;
  /*
  $form['created'] = array(
    '#type'          => 'value',
    '#date_format'   => 'Y-m-d H:i',
    '#default_value' => $datetime,
  );
  */
  
  //var_dump($datetime);
  
  $data = example_booking_get_form_data_matrix($tertiary_id ? $tertiary_id : $secondary_id, $datetime);
  
  $price_data = json_decode($data['affected_settings'] -> notice);
  
  
  $price_key = 'members';
  $price = $price_data -> price -> type == 'regular' ? $price_data -> price -> regular -> $price_key : $price_data -> price -> non_regular -> { $duration } -> $price_key;
  //dsm($price_data);
  //dsm($price);
  
  $form['book_price'] = array(
    '#type'          => 'item',
    '#title'         => t('Booking price: '),
    '#markup'         => '$' . $price, 
    '#weight'        => 1,
  );
  
  $form['duration'] = array(
    '#type'          => 'item',
    '#title'         => t('Duration: '),
    '#markup'         => $duration . t(' minutes'),
    '#weight'        => 1,
  );
  
  $form['slot_time'] = array(
    '#title'         => t('Booking date'),
    '#type'          => module_exists('date_popup') ? 'date_popup' : 'date_select',
    '#date_format'   => 'Y-m-d H:i',
    '#default_value' => $datetime,
    '#attributes' => array( 'disabled' => 'disabled' )
  );
  

  
  $form['booking'] = array(
    '#type' => 'value',
    '#value' => $entity,
  );
  
  field_attach_form('booking', $entity, $form, $form_state);
  //dsm($form);
  // selecting fid / disabling changing
  
  global $user;
  
  if (user_access('administer bookings')) {
    if (!$form_state['input']['field_user']['und'])
      $form['field_user'][$form['field_user']['#language']][0]['target_id']['#value'] = $user -> name . ' (' . $user -> uid . ')';
  }
  else
  {
    $form['field_user'][$form['field_user']['#language']]['#access'] = false;
  }
  
  $secondary_to_primary_name   = variable_get('example_booking_2nd_to_1st_ref_field');
  $tertiary_to_secondary_name  = variable_get('example_booking_3rd_to_2nd_ref_field');
  
  
  $form[$secondary_to_primary_name][$form[$secondary_to_primary_name]['#language']]['#value'] = $primary_id;
  $form[$secondary_to_primary_name][$form[$secondary_to_primary_name]['#language']]['#attributes'] = array('disabled' => 'disabled');
  
  $form[$tertiary_to_secondary_name][$form[$tertiary_to_secondary_name]['#language']]['#value'] = $tertiary_id;
  $form[$tertiary_to_secondary_name][$form[$tertiary_to_secondary_name]['#language']]['#attributes'] = array('disabled' => 'disabled');
  
  $form['field_bt_facility'][$form['field_bt_facility']['#language']]['#default_value'] = $secondary_id;
  $form['field_bt_facility'][$form['field_bt_facility']['#language']]['#attributes'] = array( 'disabled' => 'disabled' );

  if (is_numeric($tertiary_id)) {
    $form['field_bt_facility'][$form['field_bt_facility']['#language']]['#access'] = FALSE;
  } else {
    $form[$tertiary_to_secondary_name][$form[$tertiary_to_secondary_name]['#language']]['#access'] = FALSE;
  }


  $form['field_bt_number_of_players'][LANGUAGE_NONE][0]['value']['#description'] = t('Maximum of !num player(s).', array('!num' => $data['capacity']));
  
  $form['submit'] = array(
    '#type'    => 'submit',
    '#value'   => t('Create'),
    '#weight'  => 1000,
  );

  return $form;
}

/**
 * Validation for the entity creation page.
 */
function example_booking_booking_form_validate($form, &$form_state) {
  
  $entity = $form_state['values']['booking'];
  
  $tertiary_to_secondary_name  = variable_get('example_booking_3rd_to_2nd_ref_field');
  
  $fid = $form_state['values'][$tertiary_to_secondary_name]['und'][0]['nid'];
  
  if (!$fid)
    $fid = $form_state['values']['field_bt_facility']['und'][0]['nid'];

  $players = $form_state['values']['field_bt_number_of_players']['und'][0]['value'];
  
  // custom validation for booking slots
  $data = example_booking_get_form_data_matrix($fid, $entity->slot_time);
  
  if($data['capacity'] < $players)
  {
    form_set_error('field_bt_number_of_players', t('There is not enough slots to book that many players.'));
  }
  
  field_attach_form_validate('booking', $entity, $form, $form_state);
}

/**
 * Helper function to get booking data from database
 */
function example_booking_get_form_data_matrix($fid, $slot_time) {
  $start_time = substr($slot_time, 0, 10) . " 00:00:00";
  $end_time = substr($slot_time, 0, 10) . " 23:59:59";
  $book_time = substr($slot_time, 11, 5); // HH:MM
  
  module_load_include('inc', 'example_booking', 'example_booking.theme');

  $primary_name                = variable_get('example_booking_1st_content_type');
  $secondary_name              = variable_get('example_booking_2nd_content_type');
  $secondary_to_primary_name   = variable_get('example_booking_2nd_to_1st_ref_field');
  $tertiary_name               = variable_get('example_booking_3rd_content_type');
  $tertiary_to_secondary_name  = variable_get('example_booking_3rd_to_2nd_ref_field');
  $tertiary_to_quaternary_name = variable_get('example_booking_3rd_to_4th_ref_field');
  $tertiary_capacity_name      = variable_get('example_booking_3rd_capacity_field');  
  
  $node = node_load($fid);
  
  if ($node -> type == $secondary_name) {
    $vid = $node -> $secondary_to_primary_name;
    $vid = $vid[LANGUAGE_NONE][0]['nid'];
    $fid = $node -> vid;
    $cid = null;
  }
  elseif ($node -> type == $tertiary_name) {
    $fid = $node -> $tertiary_to_secondary_name;
    $fid = $fid[LANGUAGE_NONE][0]['nid'];
    
    $facility = node_load ($fid);
    
    $vid = $facility -> $secondary_to_primary_name;
    $vid = $vid[LANGUAGE_NONE][0]['nid'];
    $cid = $node -> vid;
  }
  
  
  $data_matrix = example_booking_get_availability_data_matrix($vid, $fid, $cid, 'full', $start_time, $end_time, NULL, NULL, 15);
  //dsm($data_matrix);
  return $data_matrix['data'][0][$book_time];
}

/**
 * Submission handler for the entity creation page.
 */
function example_booking_booking_form_submit($form, &$form_state) {
  $entity = $form_state['values']['booking'];  
  //var_dump($form_state['values']); exit;
  
  // the value of players is stored in separate table, so copy the value into entity table as well.
  $entity->players = $form_state['values']['field_bt_number_of_players']['und'][0]['value'];
  
  global $user;
  
  $uid = $user -> uid;
  
  if (user_access('administer bookings')) {
    $uid = $form_state['values']['field_user'][LANGUAGE_NONE][0]['target_id'];
  }
  
  if (!$entity->uid)
    $entity->uid = $uid;
  
  $primary_name                = variable_get('example_booking_1st_content_type');
  $secondary_name              = variable_get('example_booking_2nd_content_type');
  $secondary_to_primary_name   = variable_get('example_booking_2nd_to_1st_ref_field');
  $tertiary_name               = variable_get('example_booking_3rd_content_type');
  $tertiary_to_secondary_name  = variable_get('example_booking_3rd_to_2nd_ref_field');
  $tertiary_to_quaternary_name = variable_get('example_booking_3rd_to_4th_ref_field');
  $tertiary_capacity_name      = variable_get('example_booking_3rd_capacity_field');  
  
  
  $booked_node = node_load($form_state['values']['field_bt_facility']['und'][0]['nid']);
  
  if ($booked_node -> type == $secondary_name)
  {
    $field                = $booked_node -> $secondary_to_primary_name;
    $entity->primary_id   = $field[LANGUAGE_NONE][0]['nid'];
    $entity->secondary_id = $booked_node -> vid;
  } elseif ($booked_node -> type == $tertiary_name) {
    $field                = $booked_node -> $tertiary_to_secondary_name;
    $fid_node = node_load($field[LANGUAGE_NONE][0]['nid']);
    $field                = $fid_node -> $secondary_to_primary_name;
    $entity->primary_id   = $field[LANGUAGE_NONE][0]['nid'];
    $entity->secondary_id = $fid_node -> vid;
    $entity->tertiary_id  = $booked_node -> vid;
  }
  
  field_attach_submit('booking', $entity, $form, $form_state);
  $booking = example_booking_booking_save($entity);
  $form_state['redirect'] = "booking/$booking->pid";
}

function example_booking_booking_form_add($input) {
  is_array($input) ?: drupal_not_found(); // Page not found on wrong booking
  $entity = entity_get_controller('booking')->create();
  
  // since slot_time is separated from entity itself it needs to be assigned from the form
  list($primary_id, $secondary_id, $tertiary_id, $cid, $datetime, $duration) = $input;
  //var_dump($fid, $cid, $datetime, $entity);
  $entity->slot_time = $datetime;
  $entity->cid = $cid;
  $entity->primary_id = $primary_id;
  $entity->secondary_id = $secondary_id;
  $entity->tertiary_id = $tertiary_id;
  $entity->duration = $duration;
  
  return drupal_get_form('example_booking_booking_form', $entity, $input);
}

/**
 * Provides a list of existing entities and the ability to add more. Tabs
 * provide field and display management.
 */
function example_booking_booking_admin_page() {
  $content = array();
  $content[] = array(
    '#type' => 'item',
    '#markup' => t('Administration page for bookings.')
  );

  $content[] = array(
    '#type' => 'item',
    '#markup' => l(t('Add booking'), 'booking/add'),
  );

  $content['table'] = example_booking_booking_list_entities();

  return $content;
}

function example_booking_booking_list_entities() {
  return array();
}

/**
 * Entity management pages paths.
 */
function example_booking_booking_menu(&$items) {

  $items['admin/structure/booking'] = array(
    'title'            => 'Bookings',
    'description'      => 'Manage bookings',
    'page callback'    => 'example_booking_booking_admin_page',
    'access arguments' => array('administer bookings'),
  );

  $items['booking/add/%booking_b64json'] = array(
    'title'            => 'New booking',
    'page callback'    => 'example_booking_booking_form_add',
    'page arguments'   => array(2),
    'access arguments' => array('create booking entities'),
    'file'             => 'example_booking.booking.inc',
  );

  // This will enforce using of booking_load to be used as 1st argument replacement callback
  $items['booking/%booking'] = array(
    'title callback'   => 'example_booking_booking_page_title',
    'title arguments'  => array(1),
    'page callback'    => 'example_booking_booking_page_view',
    'page arguments'   => array(1),
    'access arguments' => array('view bookings'),
    'file'             => 'example_booking.booking.inc',
    'type'             => MENU_CALLBACK
  );

  $items['booking/%booking/view'] = array(
    'title'            => 'View',
    'type'             => MENU_DEFAULT_LOCAL_TASK,
    'weight'           => -10,
  );
  
  $items['booking/%booking/remove'] = array(
    'title'   => 'Delete booking',
    'page callback'    => array('drupal_get_form'),
    'page arguments'   => array('example_booking_booking_form_remove', 1),
    'access arguments' => array('administer bookings'),
    'file'             => 'example_booking.booking.inc',
    'context'          => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
  );
  
/*
  $items[$booking_uri . '/delete'] = array(
    'title'            => 'Delete booking',
    'title callback'   => 'example_booking_label_booking',
    'title arguments'  => array($booking_uri_argument_position),
    'page callback'    => 'drupal_get_form',
    'page arguments'   => array('example_booking_delete_form_booking', $booking_uri_argument_position),
    'access callback'  => 'entity_access',
    'access arguments' => array('edit', 'example_booking_booking', $booking_uri_argument_position),
    'file'             => 'example_booking_booking.inc',
  );
*/
  
  $items['booking/%booking/edit'] = array(
    'title'            => 'Edit',
    'page callback'    => array('drupal_get_form'),
    'page arguments'   => array('example_booking_booking_form_edit', 1),
    'access arguments' => array('edit booking entities'),
    'file'             => 'example_booking.booking.inc',
    'type'             => MENU_LOCAL_TASK,
    'context'          => MENU_CONTEXT_PAGE | MENU_CONTEXT_INLINE,
  );

}

/**
 * Menu callback; loads a json object encoded by base64
 */
function booking_b64json_load($arg) {
  return json_decode(base64_decode($arg));
}

function example_booking_booking_permission(&$permissions){
  $permissions['administer bookings']       = array('title' => t('Administer bookings'), 'restrict access' => TRUE);
  $permissions['view bookings']             = array('title' => t('View bookings'));
  $permissions['create booking entities']   = array('title' => t('Create booking'));
  $permissions['edit any booking entity']   = array('title' => t('Edit any booking'));
  $permissions['edit own booking entities'] = array('title' => t('Edit own bookings'));
  $permissions['book past dates']           = array('title' => t('Enable booking of past dates'));
  return $permissions;
}

/**
 * Access handler for the entity.
 */
function example_booking_booking_access($op, $task, $account = NULL, $entity_type = NULL) {
  global $user;

  if (!isset($account)) {
    $account = $user;
  }
  switch ($op) {
    case 'create':
      return user_access('administer bookings', $account)
          || user_access('create booking entities', $account);
    case 'view':
      return user_access('administer bookings', $account)
          || user_access('view bookings', $account);
    case 'edit':
      return user_access('administer bookings')
          || user_access('edit any booking entity')
          || (user_access('edit own booking entities') && ($task -> uid == $account -> uid));
  }
}

/**
 * Fetch a basic object.
 *
 * This function ends up being a shim between the menu system and
 * example_booking_booking_load_multiple().
 *
 * This function gets its name from the menu system's wildcard
 * naming conventions. For example, /path/%wildcard would end
 * up calling wildcard_load(%wildcard value). In our case defining
 * the path: examples/entity_example/basic/%entity_example_basic in
 * hook_menu() tells Drupal to call entity_example_basic_load().
 *
 * @param $basic_id
 *   Integer specifying the basic entity id.
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   A fully-loaded $basic object or FALSE if it cannot be loaded.
 *
 * @see entity_example_basic_load_multiple()
 * @see entity_example_menu()
 */
function booking_load($basic_id = NULL, $reset = FALSE) {
  $basic_ids = (isset($basic_id) ? array($basic_id) : array());
  $basic = example_booking_booking_load_multiple($basic_ids, array(), $reset);
  return $basic ? reset($basic) : FALSE;
}

/**
 * Loads a single booking.
 *
 * @param $pid
 *   Integer specifying the entity id.
 * @param $reset
 *   A boolean indicating that the internal cache should be reset.
 * @return
 *   A fully-loaded $basic object or FALSE if it cannot be loaded.
 *
 * @see entity_example_basic_load_multiple()
 * @see entity_example_menu()
 */
/*
function example_booking_booking_load($pid = NULL, $reset = FALSE) {
  $ids = (isset($pid) ? array($pid) : array());
  $res = example_booking_booking_load_multiple($ids, array(), $reset);
  return $res ? reset($res) : FALSE;
}
*/

/**
 * Loads multiple basic entities.
 *
 * We only need to pass this request along to entity_load(), which
 * will in turn call the load() method of our entity controller class.
 *
 * @see entity_load()
 * @see EntityFieldQuery
 *
 * @param $pids
 *   An array of booking IDs.
 * @param $conditions
 *   (deprecated) An associative array of conditions on the {node}
 *   table, where the keys are the database fields and the values are the
 *   values those fields must have. Instead, it is preferable to use
 *   EntityFieldQuery to retrieve a list of entity IDs loadable by
 *   this function.
 * @param $reset
 *   Whether to reset the internal node_load cache.
 *
 * @return
 *   An array of node objects indexed by ids.
 */
function example_booking_booking_load_multiple($pids = array(), $conditions = array(), $reset = FALSE) {
  return entity_load('booking', $pids, $conditions, $reset);
}

/**
* Saves booking entity.
*/
function example_booking_booking_save($entity) {
  entity_save('booking', $entity);
  return $entity;
}

/**
* Deletes a single booking.
*/
function example_booking_booking_delete($id) {
  entity_delete('booking', entity_id('booking' ,$id));
}

/**
* Deletes multiple bookings.
*/
function example_booking_booking_delete_multiple($ids) {
  entity_delete_multiple('booking', $ids);
}
