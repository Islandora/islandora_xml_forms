<?php

/**
 * @file
 *
 * @TODO: needs documentation + list of all available variables
 */
?>
<div id="content-model-form-main">
  <div id="content-model-form-table">
    <table>
      <tr>
        <th><?php print t('Content model'); ?></th>
        <th><?php print t('Type'); ?></th>
        <th><?php print t('Datastream ID'); ?></th>
        <th><?php print t('Title field'); ?></th>
        <th><?php print t('Transform'); ?></th>
        <th><?php print t('Has template'); ?></th>
        <th><?php print t('Operations'); ?></th>
      </tr>
      <?php foreach ($associations as $association) : ?>
      <tr>
        <td><?php print $association['content_model'] ?></td>
        <td><?php print ($association['type'] == 'hook') ? t('Built-in') : t('Custom') ?></td>
        <td><?php print $association['dsid'] ?></td>
        <td><?php print $association['title_field'] ?></td>
        <td><?php print $association['transform'] ?></td>
        <td><?php print ($association['template']) ? t('Yes') : t('No') ?></td>
        <td>
          <?php if($association['type'] == 'hook'): ?>
            <?php if($association['enabled']): ?>
              <?php print l(t("Disable"), "admin/islandora/xmlform/{$association['form_name']}/disassociate/{$association['id']}") ?>
            <?php else: ?>
              <?php print l(t("Enable"), "admin/islandora/xmlform/{$association['form_name']}/associate/{$association['id']}") ?>
            <?php endif; ?>
          <?php else: ?>
            <?php print l(t("Delete"), "admin/islandora/xmlform/{$association['form_name']}/disassociate/{$association['id']}") ?>
          <?php endif; ?>
        </td>
      </tr>
      <?php endforeach; ?>
    </table>
  </div>
</div>
