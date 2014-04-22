// Displays a summary of the opening hours setting on the vertical tab on the node type form.
(function ($) {
"use strict";

Drupal.behaviors.commentFieldsetSummaries = {
  attach: function (context) {
    // Provide the summary for the node type form.
    $('fieldset.opening-hours-node-type-settings-form', context).drupalSetSummary(function(context) {
      // Default opening-hours setting.
      if ($(".form-item-opening-hours-enabled input:checked", context).length > 0) {
        return Drupal.t('Enabled');
      }
      else {
        return Drupal.t('Disabled');
      }
    });
  }
};

})(jQuery);
