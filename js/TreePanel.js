/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

Ext.formbuilder.createTreePanel = function() {
    return Ext.create('Ext.tree.Panel', {
        viewConfig: {
            plugins: {
                ptype: 'treeviewdragdrop'
            }
        },
        title: 'Elements',
        store: this.elementStore,
        region: 'west',
        width: 230,
        margin: '1 0 1 1',
        autoScroll: true,
        rootVisible: true,
        split: true,
        tbar: {
            xtype: 'toolbar',
            items: [{
                xtype: 'button',
                text: 'Add'
            }, {
                xtype: 'button',
                text: 'Edit'
            }, {
                xtype: 'button',
                text: 'Copy'
            }, {
                xtype: 'button',
                text: 'Paste'
            }, {
                xtype: 'button',
                text: 'Delete'
            }]
        },
        listeners: {
            itemclick: function(view, record, item, index, event) {
                Ext.formbuilder.showElementForm();
                var form = Ext.formbuilder.elementForm.getForm();
                form.loadRecord(record);
                // Custom stuff...
                var after_build = Ext.getCmp('after_build');
                after_build.store.loadData(record.data['after_build'], false);
                var attributes = Ext.getCmp('attributes');
                attributes.store.loadData(record.data['attributes'], false);
            }
        }
    });
}

