<?php
/**
 * @file
 * Template for rendering opening hours week.
 */

if (!empty($preface)) {
  print $preface;
}
?>
<div class="opening-hours-week placeholder" data-nid="<?php print $node->nid; ?>">
  <div class="header">
    <a class="prev" href="#prev">←</a>
    <?php print t('Week'); ?>
    <span class="week_num"></span> –
    <span class="from_date"></span> –
    <span class="to_date"></span>
    <a class="next" href="#next">→</a>
  </div>
  <div class="days"></div>
</div>

