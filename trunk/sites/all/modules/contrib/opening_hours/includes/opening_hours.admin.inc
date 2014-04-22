<?php
/**
 * @file
 * Administration page for opening hours.
 */

/**
 * Main admin overview page.
 */
function opening_hours_admin_settings_page() {
  $page = array();

  // Blocked days section.
  $page['blocked_title'] = array(
    '#tag' => 'h3',
    '#type' => 'html_tag',
    '#value' => t('Blocked days'),
  );

  $page['blocked_description'] = array(
    '#tag' => 'p',
    '#type' => 'html_tag',
    '#value' => t('Blocked days are days where no opening hours are
    allowed, ie. everythings appears to be closed. You can use this
    functionality to enforce closing days for a large chain of shops or
    similar organisations, to avoid having to enter closing data on
    dozens of pages.'),
    '#attributes' => array('class' => 'description'),
  );

  $items = array();
  $blocked_days = variable_get('opening_hours_blocked_days', array());

  foreach ($blocked_days as $date) {
    $items[] = $date . ' ' . l(t('Delete'), 'admin/content/opening_hours/blocked_day/' . $date .  '/delete');
  }

  if (!empty($items)) {
    $page['blocked_day_list'] = array(
      '#items' => $items,
      '#theme' => 'item_list',
    );
  }
  else {
    $output[] = '<p>' . t('No blocked days found.') . '</p>';
  }

  $page['block_add_new'] = array(
    '#markup' => l(t('Add new blocked day'), 'admin/content/opening_hours/blocked_day/add'),
  );

  return $page;
}

/**
 * Add blocked day form.
 */
function opening_hours_admin_blocked_day_add_form($form, $form_state) {
  $form['date'] = array(
    '#type' => 'date_popup',
    '#title' => t('Date'),
    '#date_format' => 'Y-m-d',
    '#date_year_range' => '-0:+10',
  );

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save blocked day'),
  );

  return $form;
}

/**
 * Validation handler for blocked day add form.
 */
function opening_hours_admin_blocked_day_add_form_validate(&$form, &$form_state) {
  $matches = array();
  $date = trim($form_state['values']['date']);

  if (!preg_match('/^(20\d\d-[01]\d-[0-3]\d)/', $date, $matches)) {
    form_set_error('date', t('Invalid date. Must be in Y-m-d format, eg.  2011-12-28'));
  }

  // Store the sanitized value as the date.
  $form_state['values']['date'] = $matches[1];
}

/**
 * Submit handler for blocked day add form.
 */
function opening_hours_admin_blocked_day_add_form_submit(&$form, &$form_state) {
  $blocked_days = variable_get('opening_hours_blocked_days', array());
  $blocked_days[] = $form_state['values']['date'];

  sort($blocked_days);
  variable_set('opening_hours_blocked_days', $blocked_days);

  drupal_set_message(t('Blocked day %date added.', array(
    '%date' => $form_state['values']['date'],
  )));

  $form_state['redirect'] = 'admin/content/opening_hours';
}

/**
 * Delete blocked day confirmation page.
 */
function opening_hours_admin_blocked_day_delete_form($form, $form_state, $date) {
  $form['date'] = array(
    '#type' => 'value',
    '#value' => $date,
  );

  return confirm_form($form, t('Delete blocked day @date?', array('@date' => $date)), 'admin/content/opening_hours');
}

/**
 * Submit handler for delete form.
 */
function opening_hours_admin_blocked_day_delete_form_submit(&$form, &$form_state) {
  $blocked_days = variable_get('opening_hours_blocked_days', array());
  $index = array_search($form_state['values']['date'], $blocked_days);

  unset($blocked_days[$index]);
  sort($blocked_days);
  variable_set('opening_hours_blocked_days', $blocked_days);

  drupal_set_message(t('Blocked day @date deleted.', array(
    '@date' => $form_state['values']['date'],
  )));

  $form_state['redirect'] = 'admin/content/opening_hours';
}
