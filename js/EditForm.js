/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
Ext.ns('Ext.app');

Ext.app = (function() {
    var that = {
        create: function () {
            return Ext.create('Ext.panel.Panel', {
                width: 960,
                height: 800,
                title: 'Form Editor',
                layout: 'border',
                renderTo: 'xml-form-builder-editor',
                items: [this.createTree(), this.createForm() ],
                defaults: {
                  margin: '1 0 1 0',
                  frame: true
                },
                tbar: {
                    xtype: 'toolbar',
                    items: [
                    {
                        xtype: 'button',
                        text: 'Form Properties'
                    },
                    {
                        xtype: 'tbfill'
                    },
                    {
                        xtype: 'button',
                        text: 'Save'
                    },
                    {
                        xtype: 'tbseparator'
                    },
                    {
                        xtype: 'button',
                        text: 'Save & Preview'
                    }
                    ]
                }
            });
        },
        createTree: function () {
            return Ext.create('Ext.tree.Panel', {
                viewConfig: {
                    plugins: {
                        ptype: 'treeviewdragdrop'
                    }
                },
                title: 'Elements',
                store: this.createElementStore(),
                region: 'west',
                width: 230,
                margin: '1 0 1 1',
                autoScroll: true,
                rootVisible: true,
                split: true,
                tbar: {
                    xtype: 'toolbar',
                    items: [
                    {
                        xtype: 'button',
                        text: 'Add'
                    },
                    {
                        xtype: 'button',
                        text: 'Edit'
                    },
                    {
                        xtype: 'button',
                        text: 'Copy'
                    },
                    {
                        xtype: 'button',
                        text: 'Paste'
                    },
                    {
                        xtype: 'button',
                        text: 'Delete'
                    }
                    ]
                }
            });
        },
        createForm: function() {
            return Ext.create('Ext.form.Panel', {
                title: 'Element',
                region: 'center',
                margin: '1 1 1 0',
                items: [{
                    xtype: 'textfield',
                    name: 'name',
                    fieldLabel: 'Name',
                    allowBlank: false
                }]
            });
        },
        createElementStore: function () {
            return Ext.create('Ext.data.TreeStore', {
                model: 'Element',
                root: {
                    text: 'Elements',
                    root: 'elements',
                    id: 'id'
                }
            });
        }
    }
    return that;
})();

//
Ext.onReady(function() {
    var app = Ext.app.create();
});