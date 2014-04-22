<?php
/**
 * @file
 * Page callbacks for the opening hours module.
 */

/**
 * Page for editing the opening hours for a specific node.
 */
function opening_hours_node_edit_page($node) {
  drupal_set_title(t('Opening hours for @title', array('@title' => $node->title)));

  drupal_add_css(drupal_get_path('module', 'jquery_ui') . '/jquery.ui/themes/default/ui.all.css');
  drupal_add_css(drupal_get_path('module', 'opening_hours') . '/css/opening_hours.admin.css');
  opening_hours_add_js('admin', $node->nid);

  return theme('opening_hours_admin');
}

/**
 * The CRUD API for communication with Backbone.
 */
function opening_hours_crud_api_page() {
  $output = array();

  switch ($_SERVER['REQUEST_METHOD']) {
    case 'POST':
      if (!user_access('edit opening hours for content')) {
        header('HTTP/1.1 403 Forbidden');
        exit('Client does not have permissions to edit opening hours.');
      }

      $instance = _opening_hours_get_instance_from_request();

      // We can hardly save an instance that doesn't have the required data.
      if (!$instance) {
        header('HTTP/1.1 400 Bad Request');
        exit('Required fields were missing or invalid.');
      }

      if (drupal_write_record('opening_hours', $instance) === SAVED_NEW) {
        header('HTTP/1.1 201 Created');
        header('Location: ' . url('opening_hours/instances/' .  $instance->instance_id));
        $output[] = $instance;

        if (!empty($instance->repeat_rule)) {
          opening_hours_repeat_instance_propagate($instance);
        }

        // Clear the cached array storing which nodes have opening hours.
        cache_clear_all('opening_hours_present_on_node', 'cache');
      }

      break;
    case 'GET':
      // The nid parameter might be an array. If it's a single value, it
      // should still pass these filters with no trouble.
      // In any case, we want to elimitate bogus values, to see if
      // there's anything left to fetch.
      $nids = explode(',', $_REQUEST['nid']);
      array_filter($nids, 'is_numeric');
      array_filter($nids);

      if (!empty($_REQUEST['from_date']) && !empty($_REQUEST['to_date']) && !empty($nids)) {
        $output = opening_hours_instance_load_multiple($nids, $_REQUEST['from_date'], $_REQUEST['to_date']);
      }

      // Filter out instances on blocked days.
      // array_values is necessary, since filtered values causes holes
      // in the key order, which in turns causes json_encode to return a
      // JavaScript object instead of an array in some cases.
      $output = array_values(array_filter($output, '_opening_hours_exclude_blocked'));
  }

  return drupal_json_output($output);
}

/**
 * The CRUD API for a specific instance ID.
 */
function opening_hours_instance_id_api_page($instance) {
  $output = array();

  switch ($_SERVER['REQUEST_METHOD']) {
    case 'GET':
      $output[] = $instance;
      break;
    case 'PUT':
      $updated_instance = _opening_hours_get_instance_from_request();

      // We can hardly save an instance that doesn't have the required data.
      if (!$updated_instance) {
        header('HTTP/1.1 400 Bad Request');
        exit('Required fields were missing or invalid.');
      }

      // Check that instance_id matches up.
      if ($updated_instance->instance_id != $instance->instance_id) {
        header('HTTP/1.1 400 Bad Request');
        exit('Instance ID mismatch.');
      }

      $output[] = _opening_hours_instance_update($updated_instance, $instance);
      break;
    case 'DELETE':
      _opening_hours_instance_delete($instance);
      break;
  }

  return drupal_json_output($output);
}

/**
 * Helper function to exclude blocked days from lookups.
 */
function _opening_hours_exclude_blocked($instance) {
  return !in_array($instance->date, variable_get('opening_hours_blocked_days', array()));
}


/**
 * Helper function to get and sanitise instance from request input.
 */
function _opening_hours_get_instance_from_request() {
  $instance = json_decode(file_get_contents("php://input"));

  // At the very least, we must have nid, date, start_time and end_time set.
  if (empty($instance->nid) || empty($instance->date) ||
      empty($instance->start_time) || empty($instance->end_time)) {
    return;
  }

  return $instance;
}

/**
 * Helper function to update an existing instance.
 *
 * Also updates propagated copies, if applicable.
 *
 * @param stdClass $instance
 *   The updated instance object.
 * @param stdClass $original
 *   Original instance data as currently stored in the database.
 *
 * @return stdClass
 *   Returns $instance.
 */
function _opening_hours_instance_update($instance, $original) {
  // Handling of root instances vs. propagated copies is very different.
  if (empty($original->original_instance_id)) {
    // If instance used to repeat, and it's repeat settings have changed,
    // we need to clean out possibly outdated propagated instances.
    if (!empty($original->repeat_rule) && (
          $instance->repeat_rule != $original->repeat_rule ||
          $instance->repeat_end_date != $original->repeat_end_date)) {

      // If the event no longer repeats, or the repeat rule changed,
      // wipe all propagated copies.
      if (empty($instance->repeat_rule) || $instance->repeat_rule != $original->repeat_rule) {
        db_query("
          DELETE FROM {opening_hours}
          WHERE original_instance_id = :id
        ", array(
          ':id' => $instance->instance_id,
        ));
      }
      // Otherwise, just wipe all instances after the new end date.
      else {
        db_query("
          DELETE FROM {opening_hours}
          WHERE original_instance_id = :id
          AND date > :end_date
        ", array(
          ':id' => $instance->instance_id,
          ':end_date' => $instance->repeat_end_date,
        ));
      }
    }
  }
  else {
    // There are three different scenarios here. Applying changes to the
    // entire chain of propagated events, to the actual instance and any
    // subsequent versions, or just the one instance.
    if ($_REQUEST['propagateChanges'] == 'all') {
      // TODO: Implement this.
    }
    // If we only want to instances subsequent to the current, we'll
    // make the current a root instance and stop the propagation of the
    // original root instance.
    elseif ($_REQUEST['propagateChanges'] == 'future') {
      db_query("
        UPDATE {opening_hours} SET original_instance_id = NULL
        WHERE instance_id = :id
      ", array(':id' => $instance->instance_id));

      db_query("
        UPDATE {opening_hours} SET original_instance_id = :id
        WHERE original_instance_id = :prev_id AND date > :date
      ", array(
        ':id' => $instance->instance_id,
        ':prev_id' => $instance->original_instance_id,
        ':date' => $instance->date,
      ));

      // Stop propagating the original instance, since otherwise it
      // re-propagate on next cron run, nullifying these changes.
      opening_hours_repeat_stop_propagation($instance->original_instance_id);

      unset($instance->original_instance_id);

    }
    // If editing just the single propagated copy, we just set the
    // customised flag so it won't be affected by other changes.
    else {
      $instance->customised = 1;
    }
  }

  // drupal_write_record will try to write an empty string to the repeat
  // end date if it has an empty-ish value, which will fail, so be sure
  // to unset it first in this case.
  if (empty($instance->repeat_end_date)) {
    unset($instance->repeat_end_date);
  }

  // Once that's all done, first update the original instance, and then
  // any propagated copies.
  drupal_write_record('opening_hours', $instance, array('instance_id'));

  $query = db_query("
    UPDATE {opening_hours}
    SET start_time = :start, end_time = :end, notice = :notice
    WHERE customised = 0 AND original_instance_id = :id
  ", array(
    ':start' => $instance->start_time,
    ':end' => $instance->end_time,
    ':notice' => $instance->notice,
    ':id' => $instance->instance_id,
  ));

  // Propagate changes if instance has a repeat rule.
  if (!empty($instance->repeat_rule)) {
    opening_hours_repeat_instance_propagate($instance);
  }

  return $instance;
}

/**
 * Helper function to delete an existing instance.
 *
 * Also deletes propagated copies, if applicable.
 *
 * @param stdClass $instance
 *   The update instance object.
 */
function _opening_hours_instance_delete($instance) {
  // If we want to propagate to all copies, everything is deleted.
  if ($_REQUEST['propagateChanges'] == 'all') {
    $query = db_query("
      DELETE FROM {opening_hours}
      WHERE instance_id = :id OR original_instance_id = :id2
    ", array(
      ':id' => $instance->original_instance_id,
      ':id2' => $instance->original_instance_id,
    ));
  }
  // Delete only future instances, this one included.
  elseif ($_REQUEST['propagateChanges'] == 'future') {
    $query = db_query("
      DELETE FROM {opening_hours}
      WHERE original_instance_id = :id AND date >= :date
    ", array(
      ':id' => $instance->original_instance_id,
      ':date' => $instance->date,
    ));

    // Stop propagating the original instance, since otherwise it
    // re-propagate on next cron run, nullifying this deletion.
    opening_hours_repeat_stop_propagation($instance->original_instance_id);
  }
  else {
    $query = db_query("
      DELETE FROM {opening_hours}
      WHERE instance_id = :id OR original_instance_id = :id2
    ", array(
      ':id' => $instance->instance_id,
      ':id2' => $instance->instance_id,
    ));
  }
}
