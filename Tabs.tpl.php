<?php
$classes = ($collapsible) ? 'xml-form-elements-tabs-collapsible ' : '';
$classes .= ( $collapsed) ? 'xml-form-elements-tabs-collapsed' : '';
?>
<div class="xml-form-elements-tabs <?php print $classes ?>">
  <!-- Header  -->
  <ul>
    <!-- First Tab Panel -->
    <li>
      <a href='<?php print "#$link" ?>'>
        <span style='float:left; vertical-align: middle; text-align: center;'><?php print $title ?></span>
        <?php if ($collapsible): ?>
          <span class='expand-tabpanel-icon ui-icon ui-icon-circle-triangle-e' style='float: right; vertical-align: middle; margin-left:0.2em;'></span>
        <?php else: ?>
          <span class='expand-tabpanel-icon ui-icon ui-icon-circle-triangle-s' style='float: right; vertical-align: middle; margin-left:0.2em;'></span>
        <?php endif; ?>
      </a>
    </li>
    <!-- All other Tab Panels -->
    <?php if (isset($tabpanels)): ?>
      <?php foreach ($tabpanels as $title => $link): ?>
        <li>
          <a href='<?php print "#$link" ?>'><?php print $title ?></a>
          <span class='ui-icon ui-icon-close' style='float: left; margin: 0.4em 0.2em 0 0; cursor: pointer;'></span>
        </li>
      <?php endforeach; ?>
    <?php endif; ?>
  </ul>
  <!-- Content  -->
  <?php print $content ?>
</div>