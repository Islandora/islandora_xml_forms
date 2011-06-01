<div class="xml-form-elements-tags form-item">
  <label for="<?php print "{$input['id']}" ?>"><?php print "{$label}" ?></label>
  <input type="text" 
         name="<?php "{$input['name']}" ?>" 
         id="<?php print "{$input['id']}" ?>" 
         size="25" 
         value="<?php print "{$input['value']}" ?>" 
         class="form-text listElement list">
  </input>
  <div>
  </div>
  <ui class="tagEditor">
    <li title="Remove tag">remove</li>
  </ui>
</div>