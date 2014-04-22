/**
 * @file
 * JavaScript code for presenting opening hours.
 */

(function ($) {
  "use strict";

  // Global container for Opening Hours data.
  Drupal.OpeningHours.dataStore = {};

  // Get instance data from the server via AJAX.
  Drupal.OpeningHours.getInstances = function (options) {
    var nids = options.nids;

    // If just a single nid was given, wrap it in an array, so we can
    // treat it uniformly below.
    if (!nids) {
      nids = [options.nid];
    }

    $.ajax({
      data: {
        from_date: options.week.dates[0].getISODate(),
        to_date: options.week.dates[6].getISODate(),
        nid: nids.join()
      },
      dataType: 'json',
      success: function (data) {
        // Set up an empty dataStore array for each day in the week for each nid.
        _.each(nids, function (nid) {

          // Make sure we have an object ready for each nid.
          if (!Drupal.OpeningHours.dataStore[nid]) {
            Drupal.OpeningHours.dataStore[nid] = {};
          }

          _.each(options.week.dates, function (date) {
            Drupal.OpeningHours.dataStore[nid][date.getISODate()] = [];
          });
        });

        // Store the received data in the dataStore.
        _.each(data, function (instance) {
          Drupal.OpeningHours.dataStore[instance.nid][instance.date].push(instance);
        });

        if (options.success) {
          options.success();
        }
      },
      url: Drupal.settings.basePath + 'opening_hours/instances'
    });
  };

  // View prototype to manage each week presentation’s state.
  Drupal.OpeningHours.WeekPresentationView = function (options) {
    var self = this;

    self.constructor = function () {
      self.el = $(options.el);
      self.nid = options.nid;
      self.options = options;
      self.week = options.week;

      // Set up binding for navigation.
      self.el.find('.prev').click(self.goToPreviousWeek);
      self.el.find('.next').click(self.goToNextWeek);

      return self;
    };

    // Get data for the current week.
    self.getData = function (callback) {
      var from_date = self.week.dates[0].getISODate(),
          to_date = self.week.dates[6].getISODate();

      // If the data is already in the data store, invoke the callback
      // immediately and return.
      if (Drupal.OpeningHours.dataStore[self.nid] &&
          _.isArray(Drupal.OpeningHours.dataStore[self.nid][from_date]) &&
          _.isArray(Drupal.OpeningHours.dataStore[self.nid][to_date])) {
        callback();
        return self;
      }

      Drupal.OpeningHours.getInstances({
        from_date: from_date,
        nid: self.nid,
        to_date: to_date,
        week: self.week,
        success: callback
      });

      return self;
    };

    self.goToPreviousWeek = function (event) {
      var date = new Date(self.week.dates[0].getTime());

      if ($(this).hasClass('disabled')) {
        return false;
      }

      // Subtract seven days to get the first date of the previous week,
      // and use the router to navigate back to that.
      date.setDate(date.getDate() - 7);

      // Change the date of the view and re-render it.
      self.week = new Drupal.OpeningHours.Week(date.getISODate(), self.week.firstDayOfWeek);
      self.render();

      event.preventDefault();
    };

    self.goToNextWeek = function (event) {
      var date = new Date(self.week.dates[6].getTime());

      // Add one day to the last date of the previous week,
      // and use the router to navigate back to that.
      date.setDate(date.getDate() + 1);

      // Change the date of the view and re-render it.
      self.week = new Drupal.OpeningHours.Week(date.getISODate(), self.week.firstDayOfWeek);
      self.render();

      event.preventDefault();
    };

    // Render the week as soon as data is available.
    self.render = function () {
      // Fade out the element while we're waiting on data.
      self.el.fadeOut('fast');
      // Wait till we have data available before rendering.
      self.getData(function (data) {
        var daysContainer = self.el.find('.days');

        // Clean out previously rendered week, if any.
        daysContainer.empty();

        // Fill in the header.
        self.el.find('.from_date').text($.datepicker.formatDate('d/m', self.week.dates[0]));
        self.el.find('.to_date').text($.datepicker.formatDate('d/m', self.week.dates[6]));
        // Use the middle day of the week to get the week number, to
        // dodge off-by-one-errors in older versions of jQuery UI.
        self.el.find('.week_num').text($.datepicker.iso8601Week(self.week.dates[3]));

        // If we're at the current week, disable the back arrow.
        if (self.week.isCurrentWeek()) {
          self.el.find('.prev').addClass('disabled');
        } else {
          self.el.find('.prev').removeClass('disabled');
        }

        // Helper variables to add even/odd classes to rows.
        var flip = { 'even' : 'odd', 'odd' : 'even' };
        var even_odd = 'even';

        // Render each day.
        _.each(self.week.dates, function (date) {
          var dateStr = date.getISODate(),
              renderedInstances = [];

          // Render each instance for this day.
          _.each(Drupal.OpeningHours.dataStore[self.nid][dateStr], function (instance) {
            renderedInstances.push(self.options.instanceTemplate({
              start_time: instance.start_time,
              end_time: instance.end_time,
              notice: instance.notice || ''
            }));
          });

          // Render the day container with the instances in it (or a
          // simple "closed" notice.
          daysContainer.append(self.options.dayTemplate({
            name: $.datepicker.formatDate('DD', date),
            instances: renderedInstances.join("") || Drupal.t('closed'),
            even_odd: even_odd
          }));

          // Flip the even/odd value every time an instance is added.
          even_odd = flip[even_odd];
        });

        // Convert all notices to Tipsy tooltips.
        self.el.find('.notice').each(function () {
          var $notice = $(this),
              message = $notice.text();

          if (message) {
            // Replace the notice with a star, and add a class for
            // styling that.
            $notice.text('*');
            $notice.addClass('notice-star');

            $notice.parent('.instance').attr('title', message).tipsy({
              fade: true
            });
          }
          else {
            $notice.remove();
          }
        });

        // Fade back in when we're done rendering.
        self.el.fadeIn('fast');

        self.el.removeClass('placeholder');
      });

      return self;
    };

    return self.constructor();
  };

  // When the document is ready, set up our app.
  $(function () {
    var curDate = new Date().getISODate(),
        dayTemplate = _.template($('#oho-day-presentation-template').html()),
        instanceTemplate = _.template($('#oho-instance-presentation-template').html()),
        nids = [],
        presentationViews = [],
        week = new Drupal.OpeningHours.Week(curDate, Drupal.settings.OpeningHours.firstDayOfWeek);

    // Set up WeekPresentationView instances for each presentation
    // present on the page.
    $('.opening-hours-week').each(function () {
      var view = new Drupal.OpeningHours.WeekPresentationView({
        date: curDate,
        dayTemplate: dayTemplate,
        el: this,
        firstDayOfWeek: Drupal.settings.OpeningHours.firstDayOfWeek,
        instanceTemplate: instanceTemplate,
        nid: parseInt($(this).attr('data-nid'), 10),
        week: week
      });

      // Add the view to our list, but do not render it yet, as we want
      // to fetch data for all of them in bulk.
      presentationViews.push(view);

      // Save the view instance for later reference.
      $.data(this, 'weekPresentationViewInstance', view);
    });

    if (presentationViews) {
      // Now we have generated our presentationViews, we want to get data
      // for all of them, and render them.
      // This extra step is necesarry to avoid each view making its own
      // AJAX-request for data, which would be harmful when viewing a list
      // of libraries.
      Drupal.OpeningHours.getInstances({
        nids: _.pluck(presentationViews, 'nid'),
        week: presentationViews[0].week,
        success: function () {
          _.invoke(presentationViews, 'render');

          // Let anyone who cares know that we're done loading and ready for business.
          $(window).trigger('OpeningHoursLoaded');
        }
      });
    }

  });
}(jQuery));

