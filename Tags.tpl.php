<div class="xml-form-elements-tags form-item">
  <label for="<?php print "{$input['#id']}" ?>"><?php print "{$label}" ?></label>
  <input type="text" 
         name="<?php "{$input['#name']}" ?>" 
         id="<?php print "{$input['#id']}" ?>" 
         size="25" 
         value="<?php print "{$input['#value']}" ?>" 
         class="form-text tag-editor">
  <div class="add-tag" style="display:inline-block"><input type="image" src="http://xml_form_api.local/sites/xml_form_api.local/modules/xml_form_elements/images/add.png" name="<?php print $add['#name'] ?>" id="<?php print $add['#id'] ?>" value="add"></div>
  <div class="hidden-tags">
    <?php foreach($tags as $tag): ?>
      <input type="hidden" name="<?php print $tag['#name'] ?>" id="<?php print $tag['#id'] ?>" value="<?php print "{$tag['#value']}" ?>">
    <?php endforeach ?>
  </div>
  <ui class="tag-list">
    <?php foreach($tags as $tag): ?>
    <li title="<?php print "{$tag['#value']}" ?>"><span class="edit-tag"><?php print "{$tag['#value']}" ?></span><span class="remove-tag"></span></li>
    <?php endforeach ?>
  </ui>
</div>