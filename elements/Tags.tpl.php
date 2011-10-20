<div class="xml-form-elements-tags">
  <input type="text" 
         name="<?php print "{$input['#name']}" ?>" 
         id="<?php print "{$input['#id']}" ?>" 
         size="<?php print "{$variables[0]['#size']}" ?>" 
         value="<?php print "{$input['#value']}" ?>" 
         class="form-text tag-editor">
  <div class="add-tag" style="display:inline-block"><input type="image" src="<?php print $image_path ?>/add.png" name="<?php print $add['#name'] ?>" id="<?php print $add['#id'] ?>" value="add"></div>
  <div class="hidden-controls">
    <?php print $edit['#children']; ?>
    <?php print $remove['#children']; ?>
  </div>
  <div class="hidden-tags">
    <?php foreach ($tags as $tag): ?>
      <input type="hidden" name="<?php print $tag['#name'] ?>" id="<?php print $tag['#id'] ?>" class="form-tag" value="<?php print "{$tag['#value']}" ?>">
    <?php endforeach ?>
  </div>
  <ui class="tag-list">
    <?php foreach ($tags as $tag): ?>
      <li title="<?php print "{$tag['#value']}" ?>">
        <span class="edit-tag" onclick="$('#<?php print $edit[$tag['#hash']]['#id'] ?>').trigger('mousedown'); return false;"><?php print "{$tag['#value']}" ?></span>
        <span class="remove-tag" onclick="$('#<?php print $remove[$tag['#hash']]['#id'] ?>').trigger('mousedown'); return false;"></span>
      </li>
    <?php endforeach ?>
  </ui>
</div>