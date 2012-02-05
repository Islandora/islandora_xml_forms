<div class="xml-form-elements-pages">
  <!-- Headers -->
  <ul>
    <li class="ui-corner-all">
      <a href='#one'>Step 1</a>
    </li>
    <li class="ui-corner-all">
      <a href='#two'>Step 2</a>
    </li>
  </ul>
  <!-- Content -->
  <div id="one" class="ui-corner-all">
    <h2>Step One</h2>
    <div>Please Fill in all the required fields before continuing to Step 2.</div>
    <div><fieldset><legend>Authorized Form of Name</legend><fieldset><div class="form-item" id="edit-authority-name-auth-given-wrapper">
            <label for="edit-authority-name-auth-given">Given Name: </label>
            <input type="text" maxlength="128" name="authority[name][auth_given]" id="edit-authority-name-auth-given" size="60" value="" class="form-text">
          </div>
          <div class="form-item" id="edit-authority-name-auth-family-wrapper">
            <label for="edit-authority-name-auth-family">Family Name: </label>
            <input type="text" maxlength="128" name="authority[name][auth_family]" id="edit-authority-name-auth-family" size="60" value="" class="form-text">
          </div>
          <div class="form-item" id="edit-authority-name-auth-date-wrapper">
            <label for="edit-authority-name-auth-date">Date(s): </label>
            <input type="text" maxlength="128" name="authority[name][auth_date]" id="edit-authority-name-auth-date" size="60" value="" class="form-text">
            <div class="description">If known, list birth and death dates in the format YYYY-YYYY</div>
          </div>
        </fieldset>
        <div class="form-item" id="edit-authority-occupation-wrapper">
          <label for="edit-authority-occupation">Occupation: </label>
          <input type="text" maxlength="128" name="authority[occupation]" id="edit-authority-occupation" size="60" value="" class="form-text">
        </div>
        <div class="form-item" id="edit-authority-geographic-wrapper">
          <label for="edit-authority-geographic">Geographic: </label>
          <input type="text" maxlength="128" name="authority[geographic]" id="edit-authority-geographic" size="60" value="" class="form-text">
        </div>
        <fieldset><legend>Address Information</legend><div class="form-item" id="edit-authority-hierarchicalGeographic-county-wrapper">
            <label for="edit-authority-hierarchicalGeographic-county">County: </label>
            <select name="authority[hierarchicalGeographic][county]" class="form-select" id="edit-authority-hierarchicalGeographic-county"><option value="" selected="selected">Select</option><option value="Prince">Prince</option><option value="Queens">Queens</option><option value="Kings">Kings</option></select>
          </div>
          <div class="form-item" id="edit-authority-hierarchicalGeographic-region-wrapper">
            <label for="edit-authority-hierarchicalGeographic-region">Region/Electoral District: </label>
            <input type="text" maxlength="128" name="authority[hierarchicalGeographic][region]" id="edit-authority-hierarchicalGeographic-region" size="60" value="" class="form-text">
            <div class="description">this could hold the electoral district</div>
          </div>
          <div class="form-item" id="edit-authority-hierarchicalGeographic-city-wrapper">
            <label for="edit-authority-hierarchicalGeographic-city">City: </label>
            <input type="text" maxlength="128" name="authority[hierarchicalGeographic][city]" id="edit-authority-hierarchicalGeographic-city" size="60" value="" class="form-text">
          </div>
          <div class="form-item" id="edit-authority-hierarchicalGeographic-citySection-wrapper">
            <label for="edit-authority-hierarchicalGeographic-citySection">Street Address: </label>
            <input type="text" maxlength="128" name="authority[hierarchicalGeographic][citySection]" id="edit-authority-hierarchicalGeographic-citySection" size="60" value="" class="form-text">
          </div>
        </fieldset>
        <div class="clear-block" id="ahah-00000000688dd4d50000000026bb245b-wrapper"><div class="form-item" id="00000000688dd4d50000000026bb245b-wrapper">
            <label for="00000000688dd4d50000000026bb245b">Keywords: </label>
            <div class="xml-form-elements-tags">
              <input type="text" name="authority[topics][0]" id="00000000688dd4d50000000026bb245b" size="60" value="" class="form-text tag-editor">
              <input type="image" src="/sites/formbuilder.local/modules/islandora_xml_forms/elements/images/add.png" name="op" id="00000000688dd4d50000000026bb245b-add" value="add" class="ahah-processed">
              <div class="hidden-controls">
                <input type="submit" name="op" id="00000000688dd4d70000000026bb245b-edit" value="" style="display:none" class="form-submit ahah-processed">
                <input type="submit" name="op" id="00000000688dd4d70000000026bb245b-remove" value="" style="display:none" class="form-submit ahah-processed">
              </div>
              <div class="hidden-tags">
              </div>
              <span class="tag-list">
              </span>
            </div>
          </div>
        </div></fieldset>
      <div class="clear-block" id="00000000688dd4be0000000026bb245b"><div class="form-item" id="edit-variantPage-wrapper">
          <label for="edit-variantPage">Alternate Name(s): </label>
          <div class="xml-form-elements-tabs xml-form-elements-tabs-collapsible  ui-tabs ui-widget ui-widget-content ui-corner-all ui-tabs-collapsible">
            <!-- Header  -->
            <ul class="ui-tabs-nav ui-helper-reset ui-helper-clearfix ui-widget-header ui-corner-all">
              <!-- First Tab Panel -->
              <li class="tool_tip_trigger ui-state-default ui-corner-top ui-tabs-selected ui-state-active">
                <a href="#00000000688dd4b00000000026bb245b">
                  <span style="float:left; vertical-align: middle; text-align: center;">1</span>
                  <span class="expand-tabpanel-icon ui-icon ui-icon-circle-triangle-e" style="float: right; vertical-align: middle; margin-left:0.2em;"></span>
                </a>
                <span class="tool_tip">Empty</span>
              </li>
              <!-- All other Tab Panels -->
            </ul>
            <!-- Content  -->
            <div id="00000000688dd4b00000000026bb245b" class="ui-tabs-panel ui-widget-content ui-corner-bottom"><fieldset><div class="form-item" id="edit-variantPage-variant-name-variant-variant-given-wrapper">
                  <label for="edit-variantPage-variant-name-variant-variant-given">Given Name: </label>
                  <input type="text" maxlength="128" name="variantPage[variant][name_variant][variant_given]" id="edit-variantPage-variant-name-variant-variant-given" size="60" value="" class="form-text">
                </div>
                <div class="form-item" id="edit-variantPage-variant-name-variant-variant-family-wrapper">
                  <label for="edit-variantPage-variant-name-variant-variant-family">Family Name: </label>
                  <input type="text" maxlength="128" name="variantPage[variant][name_variant][variant_family]" id="edit-variantPage-variant-name-variant-variant-family" size="60" value="" class="form-text">
                </div>
              </fieldset>
              <div class="ui-tabpanel-add-button"><input type="submit" name="op" id="00000000688dd4b00000000026bb245b-add" value="Add" class="form-submit ahah-processed">
              </div><div class="ui-tabpanel-delete-button"><input type="submit" name="op" id="00000000688dd4b00000000026bb245b-remove" value="Delete" style="display:none" class="form-submit ahah-processed">
              </div></div></div>
        </div>
      </div><div class="clear-block" id="00000000688dd45c0000000026bb245b"><div class="form-item" id="edit-affiliation-wrapper">
          <label for="edit-affiliation">Affiliation(s): </label>
          <div class="xml-form-elements-tabs xml-form-elements-tabs-collapsible  ui-tabs ui-widget ui-widget-content ui-corner-all ui-tabs-collapsible">
            <!-- Header  -->
            <ul class="ui-tabs-nav ui-helper-reset ui-helper-clearfix ui-widget-header ui-corner-all">
              <!-- First Tab Panel -->
              <li class="tool_tip_trigger ui-state-default ui-corner-top ui-tabs-selected ui-state-active">
                <a href="#00000000688dd45e0000000026bb245b">
                  <span style="float:left; vertical-align: middle; text-align: center;">Party Affiliation Information</span>
                  <span class="expand-tabpanel-icon ui-icon ui-icon-circle-triangle-e" style="float: right; vertical-align: middle; margin-left:0.2em;"></span>
                </a>
                <span class="tool_tip">Empty</span>
              </li>
              <!-- All other Tab Panels -->
            </ul>
            <!-- Content  -->
            <div id="00000000688dd45e0000000026bb245b" class="ui-tabs-panel ui-widget-content ui-corner-bottom"><div class="form-item" id="edit-affiliation-affiliation-0-organization-wrapper">
                <label for="edit-affiliation-affiliation-0-organization">Organization: </label>
                <input type="text" maxlength="128" name="affiliation[affiliation_0][organization]" id="edit-affiliation-affiliation-0-organization" size="60" value="" class="form-text">
              </div>
              <div class="form-item" id="edit-affiliation-affiliation-0-position-wrapper">
                <label for="edit-affiliation-affiliation-0-position">Role in Govt/Party: </label>
                <input type="text" maxlength="128" name="affiliation[affiliation_0][position]" id="edit-affiliation-affiliation-0-position" size="60" value="" class="form-text">
              </div>
              <div class="form-item" id="edit-affiliation-affiliation-0-dateValid-start-wrapper">
                <label for="edit-affiliation-affiliation-0-dateValid-start">Start of Term: </label>
                <input type="text" maxlength="128" name="affiliation[affiliation_0][dateValid-start]" id="edit-affiliation-affiliation-0-dateValid-start" size="60" value="" class="form-text">
              </div>
              <div class="form-item" id="edit-affiliation-affiliation-0-dateValid-end-wrapper">
                <label for="edit-affiliation-affiliation-0-dateValid-end">End of Term: </label>
                <input type="text" maxlength="128" name="affiliation[affiliation_0][dateValid-end]" id="edit-affiliation-affiliation-0-dateValid-end" size="60" value="" class="form-text">
              </div>
              <div class="ui-tabpanel-delete-button"><input type="submit" name="op" id="00000000688dd45e0000000026bb245b-remove" value="Delete" style="display:none" class="form-submit ahah-processed">
              </div><div class="ui-tabpanel-add-button"><input type="submit" name="op" id="00000000688dd45e0000000026bb245b-add" value="Add" class="form-submit ahah-processed">
              </div></div></div>
        </div>
      </div><div class="form-item" id="edit-note-wrapper">
        <label for="edit-note">Biographical Note: </label>
        <div class="resizable-textarea"><span><textarea cols="60" rows="5" name="note" id="edit-note" class="form-textarea resizable textarea-processed"></textarea><div class="grippie" style="margin-right: -6px; "></div></span></div>
      </div>
      <div class="form-item" id="edit-url-wrapper">
        <label for="edit-url">URL: </label>
        <input type="text" maxlength="128" name="url" id="edit-url" size="60" value="" class="form-text">
        <div class="description">Link to personal or institutional page.</div>
      </div>
      <input type="submit" name="op" id="edit-submit" value="Submit" class="form-submit">
      <input type="hidden" name="form_build_id" id="form-818c512ecd493e5d4c413852cd0e841b" value="form-818c512ecd493e5d4c413852cd0e841b">
      <input type="hidden" name="form_token" id="edit-xml-form-builder-preview-form-token" value="61e29a4ccc9b44f97f1cb2340a531821">
      <input type="hidden" name="form_id" id="edit-xml-form-builder-preview" value="xml_form_builder_preview">

    </div>
  </div>
  <div id="two" class="ui-corner-all">
    Content 2
  </div>
</div>