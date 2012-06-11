<?php
/**
 * @file
 * Template for the Pages Element.
 */
?>
<div id="<?php print $element['#id'] ?>" class="xml-form-elements-pages clear-block">
  <!-- Headers -->
  <ul>
    <?php foreach (element_children($element) as $key) : ?>
      <li class="ui-corner-all">
        <a href='#<?php print $element[$key]['#id']; ?>-wrapper'><?php print $element[$key]['#title']; ?></a>
      </li>
    <?php endforeach; ?>
  </ul>
  <!-- Content -->
  <?php print $content ?>
</div>