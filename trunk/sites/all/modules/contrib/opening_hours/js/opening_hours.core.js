/**
 * @file
 * Contains the base Drupal.OpeningHours object with utility functions.
 */

(function ($) {
  "use strict";

  Drupal.OpeningHours = {};

  // Sunday is either the zeroeth or seventh day of the week, depending
  // oun the current locale.
  var weekdays = ['sun', 'mon', 'tue', 'wed', 'thu', 'fri', 'sat', 'sun'];

  // Crude validation for the date input.
  var dateValidator = /^\d\d\d\d-[01]\d-[0-3]\d$/;

  Drupal.OpeningHours.Week = function (dateStr, firstDayOfWeek) {
    var self = this;

    self.constructor = function () {
      self.dateStr = dateStr || new Date().getISODate();
      self.firstDayOfWeek = firstDayOfWeek;
      self.weekDays = self.orderedWeekDays(self.firstDayOfWeek);
      self.dates = self.getDates();
    };

    // Get the zero-based day number in the correct order, based on the
    // first day of week set on the object.
    self.getDayNumberOrder = function () {
      var counter = self.firstDayOfWeek,
          order = [];

      while (order.length < 7) {
        order.push(counter % 7);
        counter = counter + 1;
      }

      return order;
    };

    // Find all the dates our week spans over.
    self.getDates = function () {
      var date = Drupal.OpeningHours.parseDate(self.dateStr),
          dates = [],
          dayOrder = self.getDayNumberOrder(),
          dayOffset, tempDate,
          todaysDayNumber = date.getDay();

      // Determine how far back in time the first day of the week was.
      // This will enable us to find the correct date of the first day
      // of the specified week, so we can generate the whole week’s date
      // based on that.
      dayOffset = dayOrder.indexOf(todaysDayNumber);

      while (dates.length < 7) {
        // Start with the given date.
        tempDate = Drupal.OpeningHours.parseDate(self.dateStr);

        // Subtract the dayOffset to jump back to the day we want,
        // starting at the first day of the week based on the
        // calculations above.
        tempDate.setDate(tempDate.getDate() - dayOffset);

        dates.push(tempDate);

        // Reduce the offset by one to proceed to the next day.
        dayOffset -= 1;
      }

      return dates;
    };

    // Determine if this is the current week.
    self.isCurrentWeek = function () {
      var today = new Date().getISODate(),
          matches = _.filter(self.dates, function (candidate) {
            return candidate.getISODate() === today;
          });

      return matches.length > 0;
    };

    /**
     * Get short weekday name in order.
     */
    self.orderedWeekDays = function () {
      var counter = self.firstDayOfWeek,
          order = [];

      while (order.length < 7) {
        order.push(weekdays[counter % 7]);
        counter = counter + 1;
      }

      return order;
    };

    self.constructor();
  };

  /**
   * Format a date.
   */
  Drupal.OpeningHours.formatDate = function (date, format) {
    // Default format.
    if (!format) {
      format = 'd. MM yy';
    }

    return $.datepicker.formatDate(format, date);
  };

  /**
   * Format a date range.
   *
   * Filters the range down to the significant parts, so only the things
   * that are different are shown in the first part of the range.
   */
  Drupal.OpeningHours.formatDateRange = function (date1, date2) {
    var d1 = {
          year: date1.getFullYear(),
          month: date1.getMonth(),
          day: date1.getDate()
        },
        d2 = {
          year: date2.getFullYear(),
          month: date2.getMonth(),
          day: date2.getDate()
        };

    // If it's the same date, just return that.
    if (d1.year === d2.year && d1.month === d2.month && d1.day === d2.day) {
      return [Drupal.OpeningHours.formatDate(date1)];
    }
    // If it's the same month, return a simple day-range.
    else if (d1.year === d2.year && d1.month === d2.month) {
      return [
        $.datepicker.formatDate('d.', date1, Drupal.settings.OpeningHours.formatDate),
        Drupal.OpeningHours.formatDate(date2)
      ];
    }
    // If it's the same year, return a day-month range.
    else if (d1.year === d2.year) {
      return [
        $.datepicker.formatDate('d. MM', date1, Drupal.settings.OpeningHours.formatDate),
        Drupal.OpeningHours.formatDate(date2)
      ];
    }
    // Otherwise, return the full date range.
    else {
      return [
        Drupal.OpeningHours.formatDate(date1),
        Drupal.OpeningHours.formatDate(date2)
      ];
    }
  };

  // Parse a date in ISO 8601 format.
  Drupal.OpeningHours.parseDate = function (dateStr) {
    if (dateValidator.test(dateStr)) {
      var parts = dateStr.split('-');

      // Set the date from the parts. Remember, months are zero-based,
      // so subtract 1 from them.
      return new Date(parts[0], parts[1]-1, parts[2]);
    } else {
      throw 'Input to Drupal.OpeningHours.parseDate was not well-formed. It should be in ISO 8601 format, eg. 2011-11-28';
    }
  };

  // Configure the datepicker when the document is ready.
  $(function () {
    $.datepicker.setDefaults({
      changeMonth: true,
      changeYear: true,
      dayNames: Drupal.settings.OpeningHours.formatDate.dayNames,
      dateFormat: 'yy-mm-dd',
      firstDay: Drupal.settings.OpeningHours.firstDayOfWeek,
      monthNames: Drupal.settings.OpeningHours.formatDate.monthNames
    });
  });
}(jQuery));
