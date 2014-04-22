<?php

/**
 * @file additional routines to add 'Features' exportable support to this
 * module.
 *
 * This set of features is different from most in that it doesn't set local data
 * or settings directly, but enabling it initiates a fetch of remote content,
 * after which, we say the content/feature is enabled
 *
 * @author dman dan@coders.co.nz 2010-11
 */

module_load_include('inc', 'taxonomy_xml', 'taxonomy_xml.process');

/**
 * Implementation of hook_features_export_options().
 *
 * List all config settings currently available for export. This adds each of
 * the configurations to the features UI where they can be chosen for bundling.
 *
 * @return array A keyed array of items, suitable for use with a FormAPI select
 * or checkboxes element.
 *
 **/
function taxonomy_xml_source_features_export_options() {
  $taxonomy_xml_services = taxonomy_xml_services();
  $taxonomy_xml_services += variable_get('taxonomy_xml_imports', array());
  $options = array();
  foreach ($taxonomy_xml_services as $service_id => $service) {
    $options[$service_id] = $service['name'];
  }
  return $options;
}


/**
 * Implementation of hook_features_export().
 *
 * Process the export array for a given component.
 *
 * Normally, we will be adding this as a child in the pipe of
 * content_features_export, so that when a filefield instance is exported, this
 * setting was published along with it.
 */
function taxonomy_xml_source_features_export($data, &$export, $module_name = '') {
  $export['dependencies']['filefield_paths'] = 'taxonomy_xml';
  $taxonomy_xml_services = taxonomy_xml_services();
  $taxonomy_xml_services += variable_get('taxonomy_xml_imports', array());
  foreach ($data as $identifier) {
    if ($taxonomy_xml_services[$identifier]) {
      $export['features']['taxonomy_xml_source'][$identifier] = $identifier;
    }
  }
}


/**
 * Implementation of hook_features_export_render()
 *
 * Return the PHP code that represents a dump of the settings listed as $data
 *
 * This string is compared to the current site status to determine if the
 * feature is 'overridden'. This will not work, as we insert once into the
 * database, but cannot retrieve what was inserted, so our feature will always
 * allegedly be overridden.
 * TODO find a way to tidy this. Count the terms?
 */
function taxonomy_xml_source_features_export_render($module, $data) {
  $code = array();
  $code[] = '  $settings = array();';
  $code[] = '';
  $all_nodes = array_map('check_plain', node_type_get_name());

  // To avoid an imprt being flagged as overridden each time,
  // return a cached copy of the settings that we imported,
  // to tell the system there is no change.
  // This is cheating the method that detects overrides.
  // TODO - do this properly instead.
  $taxonomy_xml_imports = variable_get('taxonomy_xml_imports', array());
  #$taxonomy_xml_imports += taxonomy_xml_services();

  $translatables = array();
  foreach ($data as $item_id) {
    if (isset($taxonomy_xml_imports[$item_id])) {
      $item = $taxonomy_xml_imports[$item_id];
      if (empty($item['nodes'])) {
        $item['nodes'] = array();
      }

      // Check if the node associations are 'wildcard'
      $diff = array_diff((array) $all_nodes, (array) $item['nodes']);
      if (empty($diff)) {
        $item['nodes'] = '*';
      }
      unset($item['module']);

      #$item['exported'] = TRUE;
      $code[] = "  // Exported {$item_id}";
      $export = features_var_export($item, '  ');
      $code[] = "  \$settings['{$item_id}'] = {$export};";
    }
  }

  $code[] = '';
  $code[] = '  return $settings;';
  $code = implode("\n", $code);
  return array('taxonomy_xml_source_default_items' => $code);
}

/**
 * Implementation of hook_features_export_revert().
 */
function taxonomy_xml_source_features_revert($module) {
  taxonomy_xml_source_features_rebuild($module);
}

/**
 * Create/recreate the items based on the data array.
 *
 * ENABLING this feature will initiate a download/reload of the defined data
 * from the service.
 *
 * Implementation of hook_features_export_rebuild().
 */
function taxonomy_xml_source_features_rebuild($module) {
  // Cache a note about what we have imported
  $taxonomy_xml_imports = variable_get('taxonomy_xml_imports', array());

  if ($defaults = features_get_default('taxonomy_xml_source', $module)) {
    foreach ($defaults as $taxonomy_xml_source_id => $settings) {
      // Initiate vocab import!
      drupal_set_message(t('Initiating import of !taxonomy_xml_source_id', array('!taxonomy_xml_source_id' => $taxonomy_xml_source_id)));
      // Assume the target vocab name is named after the source settings.
      // The values we pass through to the import format are copied straight
      // from the settings array the feature provides
      $values = $settings;

      // Create the vocab if neccessary.
      // Any additional values (description etc) for the vocab may be set from
      // the settings array.
      // If it already exists, the placeholder will be the pre-existing vocab
      // by that name. (text name, not machine ID yet)

      // Overload the 'module' value to provide a machine_name.
      // This may be used by features - (NOT features_extra, which does it in its own table)
      if (empty($settings['module'])) {
        // Setting this will help integrate with features?
        $settings['module'] = 'features_' . preg_replace('/[^a-zA-Z0-9]+/', '_', $taxonomy_xml_source_id);
      }
      if (empty($settings['name'])) {
        // Should not happen
        $settings['name'] = $taxonomy_xml_source_id;
      }
      if (empty($settings['nodes'])) {
        $settings['nodes'] = array();
      }
      // We allow a special wildcard value for the 'nodes' array
      if (is_string($settings['nodes']) && $settings['nodes'] == '*') {
        $settings['nodes'] = array_map('check_plain', node_get_types('names'));
      }

      $vocabulary = _taxonomy_xml_get_vocabulary_placeholder($settings['name'], $settings);
      $values['vid'] = $vocabulary->vid;

      // If trying to deploy a taxonomy file with a features module, you can use
      // the token %module in the filepath setting.
      // Replace that now.
      if (isset($values['filepath'])) {
        $values['filepath'] = strtr($values['filepath'], array('!module' => drupal_get_path('module', $module)));
      }

      // Taxonomy_xml features exports can be either a 'service'
      // Using the format seen in lookup_services.inc
      if (isset($settings['servicetype'])) {
        taxonomy_xml_invoke_service_request($settings, $values);
      }
      else {
        // Or the feature may be a macro-like array, containing form values
        // that would be submitted to the import form
        taxonomy_xml_fetch_and_import($values);
      }
      $taxonomy_xml_imports[$taxonomy_xml_source_id] = $settings;
    }
    variable_set('taxonomy_xml_imports', $taxonomy_xml_imports);
  }
  else {
    drupal_set_message(t('No taxonomy sources to install for %module', array('%module' => $module)), 'error');
  }
}
