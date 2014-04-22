/**
 * @file
 * Backbone models for the opening hours admin interface.
 */

(function ($) {
"use strict";

/**
 * A single opening hours instance, ie. an open period on a specific date.
 */
Drupal.OpeningHours.Instance = Backbone.Model.extend({
  urlRoot: '/opening_hours/instances',

  // Make sure our required attributes are present.
  defaults: {
    nid: null,
    date: null,
    start_time: '',
    end_time: '',
    notice: ''
  },

  initialize: function () {
    // Drupal.settings is not available when this file is loaded, so we
    // need to set re-set urlRoot now when it should be.
    this.urlRoot = Drupal.settings.basePath + 'opening_hours/instances';
  },

  url: function () {
    // Use the standard Backbone URL logic.
    var url = Backbone.Model.prototype.url.call(this);

    // Set a query parameters with our propagateChanges option.
    if (this.get('propagateChanges')) {
      url = url + '?propagateChanges=' + this.get('propagateChanges');
    }

    return url;
  }
});

}(jQuery));

