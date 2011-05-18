/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * A grid that can be used to enter information in a form. 
 * Requires some extra logic to be populated and submitted.
 */
Ext.define('Form.Grid', {
    extend: 'Ext.grid.Panel',
    alias: 'widget.formgrid',
    requires: [
    'Ext.grid.plugin.CellEditing',
    'Ext.form.field.Text',
    'Ext.toolbar.TextItem'
    ],
    initComponent: function() {
        this.editing = Ext.create('Ext.grid.plugin.CellEditing');
        Ext.apply(this, {
            iconCls: 'icon-grid',
            frame: true,
            plugins: [this.editing],
            dockedItems: [{
                xtype: 'toolbar',
                items: [{
                    iconCls: 'icon-add',
                    text: 'Add',
                    scope: this,
                    handler: this.onAddClick
                }, {
                    iconCls: 'icon-delete',
                    text: 'Delete',
                    disabled: true,
                    itemId: 'delete',
                    scope: this,
                    handler: this.onDeleteClick
                }]
            }]
        });
        this.callParent();
        this.getSelectionModel().on('selectionchange', this.onSelectChange, this);
    },
    onSelectChange: function(selModel, selections){
        this.down('#delete').setDisabled(selections.length === 0);
    },
    onDeleteClick: function(){
        var selection = this.getView().getSelectionModel().getSelection()[0];
        if (selection) {
            this.store.remove(selection);
        }
    },
    onAddClick: function(){
        var rec = Ext.ModelManager.create(this.modelInitTmpl, this.store.model.modelName);
        var edit = this.editing;
        edit.cancelEdit();
        this.store.insert(0, rec);
        edit.startEditByPosition({
            row: 0,
            column: 0
        });
    }
});