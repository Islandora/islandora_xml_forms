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
                items: [this.createTree(), this.createForm() ]
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
                width: 300,
                autoScroll: true,
                rootVisible: true
            });
        },
        createForm: function() {
            return Ext.create('Ext.form.Panel', {
                title: 'Element',
                region: 'center',
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