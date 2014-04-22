/**
 * @file
 * JavaScript code for the opening hours admin interface.
 */

(function ($) {
"use strict";

  // When the document is ready, set up our app.
  $(function () {
    Drupal.OpeningHours.adminApp = new Drupal.OpeningHours.AdminRouter({
      container: $('#opening-hours-admin'),
      firstDayOfWeek: Drupal.settings.OpeningHours.firstDayOfWeek,
      nid: Drupal.settings.OpeningHours.nid
    });

    // Start the router history tracking.
    Backbone.history.start();
  });

}(jQuery));

