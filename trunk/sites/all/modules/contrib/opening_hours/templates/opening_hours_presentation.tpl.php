<?php
/**
 * @file
 * Template for the opening hours admin interface.
 *
 * Is not really a template in Drupal-sense, mainly a container for the
 * markup necessary to render the opening hours interface.
 */
?>
<script type="text/template" id="oho-day-presentation-template">
  <div class="day <%= even_odd %> clear-block">
    <span class="name"><%= name %></span>
    <span class="times"><%= instances %></span>
  </div>
</script>

<script type="text/template" id="oho-instance-presentation-template">
  <div class="instance">
    <span class="start_time" title="<?php echo t('Opening time'); ?>"><%= start_time %></span> –
    <span class="end_time" title="<?php echo t('Closing time'); ?>"><%= end_time %></span>
  <% if (notice) { %>
    <span class="notice"><%= notice %></span>
  <% } %>
  </div>
</script>

