/**
 * @file
 * Backbone collections for managing opening hours instances.
 */

(function ($) {
"use strict";

/**
 * A collection of opening hours instances.
 */
Drupal.OpeningHours.Instances = Backbone.Collection.extend({
  model: Drupal.OpeningHours.Instance,

  // Drupal.settings is not available when this file is loaded, so we
  // need a callback for the url.
  url: function () {
    return Drupal.settings.basePath + 'opening_hours/instances';
  }
});

}(jQuery));

