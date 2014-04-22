/**
 * @file
 * JavaScript code for the opening hours admin interface.
 */

// We create the routers after document.ready to ensure their templates
// are available.
jQuery(function($) {
"use strict";

Drupal.OpeningHours.AdminRouter = Backbone.Router.extend({
  routes: {
    '': 'today',
    'date/:date': 'specificDate'
  },

  initialize: function (options) {
    this.container = options.container;
    this.firstDayOfWeek = options.firstDayOfWeek;
    this.nid = options.nid;
    this.weekDayNames = options.weekDayNames;

    // Instantiate our main admin view.
    this.adminMainView = new Drupal.OpeningHours.AdminMainView({
      firstDayOfWeek: this.firstDayOfWeek,
      nid: this.nid
    });
  },

  today: function () {
    return this.specificDate(new Date().getISODate());
  },

  specificDate: function (dateStr) {
    var collections = [],
        nid = this.nid,
        self = this,
        week = new Drupal.OpeningHours.Week(dateStr, this.firstDayOfWeek);

    var col = new Drupal.OpeningHours.Instances({});

    col.fetch({
      data: {
        from_date: week.dates[0].getISODate(),
        to_date: week.dates[6].getISODate(),
        nid: nid
      },
      error: function (collection, response) {
        // Do something.
      },
      success: function (collection, response) {
        self.container.empty();
        self.container.html(self.adminMainView.render({
          dayInstances: collection.groupBy(function (instance) {
            return instance.get('date');
          }),
          week: week
        }).el);
      }
    });
  }
});

});
