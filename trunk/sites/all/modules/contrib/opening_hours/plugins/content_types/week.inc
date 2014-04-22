<?php
/**
 * @file
 * CTools plugin providing a Panels content type for displaying opening hours.
 */

/**
 * Array describing the plugin to CTools/Panels.
 */
$plugin = array(
  'title' => t('Opening hours by week'),
  'description' => t('Shows opening hours for the current week for the node in question with arrows for displaying future weeks as well.'),
  'single' => TRUE,
  'content_types' => array('list'),
  'required context' => new ctools_context_required(t('Node'), 'node'),
  'category' => t('Node'),
);

/**
 * Render the block
 */
function opening_hours_week_content_type_render($subtype, $conf, $panel_args, $context) {
  $node = isset($context->data) ? $context->data : NULL;

  $block = new stdClass();

  if (empty($conf['hide_if_empty']) || opening_hours_present_on_node($node->nid)) {
    $block->title = t('Opening hours');

    $block->content = theme('opening_hours_week', array('node' => $node));
  }

  return $block;
}

/**
 * Settings form for the content type.
 */
function opening_hours_week_content_type_edit_form($form, &$form_state) {
  $form['hide_if_empty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Hide if there is no opening hours data for this node'),
    '#default_value' => !(empty($form_state['conf']['hide_if_empty'])) ?  $form_state['conf']['hide_if_empty'] : NULL,
  );

  return $form;
}

/**
 * Submit handler for the admin form
 */
function opening_hours_week_content_type_edit_form_submit($form, &$form_state) {
  $form_state['conf']['hide_if_empty'] = $form_state['values']['hide_if_empty'];
}
