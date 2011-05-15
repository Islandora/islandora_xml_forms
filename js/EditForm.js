/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
Ext.ns('Ext.app');

Ext.app = (function() {
    var url = window.location.pathname;
    var element = null;
    var that = {
        /* Create the App object and render it. */
        create: function () {
            this.createModels();
            return Ext.create('Ext.panel.Panel', {
                width: 960,
                height: 800,
                title: 'Form Editor',
                layout: 'border',
                renderTo: 'xml-form-builder-editor',
                items: [this.createTree(), this.createDisplayArea() ],
                defaults: {
                    margin: '1 0 1 0',
                    frame: true
                },
                tbar: {
                    xtype: 'toolbar',
                    items: [{
                        xtype: 'button',
                        text: 'Form Properties',
                        handler: function() {
                            var display = Ext.getCmp('xml-form-builder-display').layout;
                            display.setActiveItem(1);
                        }
                    },{
                        xtype: 'tbfill'
                    },{
                        xtype: 'button',
                        text: 'Preview',
                        handler: function() {
                            var display = Ext.getCmp('xml-form-builder-display').layout;
                            display.setActiveItem(0);
                        }
                    },{
                        xtype: 'tbseparator'
                    },{
                        xtype: 'button',
                        text: 'Save'
                    }]
                }
            });
        },
        /* Create all the data models required to build a Form. */
        createModels: function() {
            /*Ext.define('Form', {
                extend: 'Ext.data.Model',
                fields: ['id', 'name'],
                proxy: {
                    type: 'ajax',
                    url: url + '/get/json',
                    reader: 'json',
                    writer: 'json'
                }
            });
            Ext.define('Properties', {
                extend: 'Ext.data.Model',
                fields: ['id', 'root_name', 'root_prefix'],
                associations: [
                {
                    type: 'hasMany',
                    model: 'Namespace',
                    primaryKey: 'id',
                    foreignKey: 'properties_id',
                    autoLoad: true,
                    autoSync: true,
                    associationKey: 'children' // read child data from child_groups
                },
                ]
            });
            Ext.define('Namespace', {
                extend: 'Ext.data.Model',
                fields: ['id', 'prefix', 'uri', 'properties_id'],
                belongsTo: {
                    model: 'Properties', 
                    name: 'properties'
                }
            });
            Ext.define('Elements', {
                extend: 'Ext.data.Model',
                fields: ['id', 'form_id'],
                associations: [{
                    type: 'hasMany',
                    model: 'Element',
                    primaryKey: 'id',
                    foreignKey: 'parent_id',
                    autoLoad: true,
                    associationKey: 'children' // read child data from child_groups
                }, {
                    type: 'belongsTo',
                    model: 'Form',
                    primaryKey: 'id',
                    foreignKey: 'form_id',
                }
                ]
            });*/
            Ext.define('Element', {
                extend: 'Ext.data.Model',
                fields: ['id', 'parent_id', 'name', 'text'],
                associations: [{
                    type: 'hasMany',
                    model: 'Element',
                    primaryKey: 'id',
                    foreignKey: 'parent_id',
                    associationKey: 'children'
                }, {
                    type: 'belongsTo',
                    model: 'Element',
                    primaryKey: 'id',
                    foreignKey: 'parent_id',
                    associatedKey: 'parent'
                }]
            });
            Ext.define('Namespace', {
                extend: 'Ext.data.Model',
                fields: ['prefix', 'uri']
            });
        //            element = new Element({
        //                id: 0,
        //                parent_id: null,
        //                name: "root"
        //            });
        //var children = element.children();
        //children.add(new Element({
        //                name: 'child'
        //  /          }));
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
        createDisplayArea: function() {
            return Ext.create('Ext.panel.Panel', {
                id: 'xml-form-builder-display',
                region: 'center',
                layout: 'card',
                margin: '1 1 1 0',
                activeItem: 1,
                unstyled: true,
                defaults: {
                    bodyStyle: 'padding:15px'
                },
                items: [
                this.createFormPreview(),
                this.createPropertiesForm(),
                this.createElementForm()
                ]
            });
        },
        createElementForm: function() {
            return Ext.create('Ext.form.Panel', {
                title: 'Element Form',
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
        createPropertiesForm: function() {
            return Ext.create('Ext.form.Panel', {
                title: 'Properties Form',
                region: 'center',
                margin: '1 1 1 0',
                frame: true,
                buttons: [{
                    text: 'Save',
                    handler: function() {
                        alert("Saved");
                    }
                }],
                items:  [{
                    xtype: 'fieldset',
                    title: 'Root Element',
                    items: [{
                        xtype: 'textfield',
                        fieldLabel: 'Local Name',
                        anchor: '100%',
                        allowBlank: false
                    },{
                        xtype: 'textfield',
                        fieldLabel: 'Namespace Prefix',
                        anchor: '100%'
                    },{
                        xtype: 'textfield',
                        fieldLabel: 'Namespace URI',
                        anchor: '100%'
                    }]
                },{
                    xtype: 'fieldset',
                    title: 'Schema',
                    items: [{
                        xtype: 'textfield',
                        fieldLabel: 'Name',
                        anchor: '100%'
                    },{
                        xtype: 'textfield',
                        fieldLabel: 'URI',
                        anchor: '100%'
                    }]
                },{
                    xtype: 'grid',
                    title: 'Namespaces',
                    height: 300,
                    //width: 454,
                    store: this.createNamespaceStorage(),
                    columns: [{
                        xtype: 'gridcolumn',
                        dataIndex: 'string',
                        header: 'Prefix',
                        sortable: true,
                        width: 150
                    },{
                        xtype: 'gridcolumn',
                        header: 'URI',
                        sortable: true,
                        flex: 1
                    }]
                }]
            });
        },
        createNamespaceStorage: function() {
            return Ext.create('Ext.data.Store', {
                storeId: 'NamespaceStore',
                model: 'Namespace',
                proxy: {
                    type: 'memory',
                    reader: {
                        type: 'json'
                    }
                }
            });
        },
        createFormPreview: function() { // Use an iframe...
            var view_url = url.replace(/\/edit/i, '/view');
            var preview = "<iframe src='" + view_url + "' width='100%' height='100%'><p>Your browser does not support iframes.</p></iframe>";
            return Ext.create('Ext.form.Panel', {
                title: 'Preview',
                html: preview
            });
        },
        createElementStore: function () {
            var data = [ {
                text: 'child',
                children: [{
                    text: 'sub child'
                }]
            }];/*,
            {
                id: 'elements/0/0',
                parent_id: 'elements/0',
                name: 'sub-child 0',
                text: 'sub-child 0',
                leaf: true
            },{
                id: 'elements/0/1',
                parent_id: 'elements/0',
                name: 'sub-child 1',
                text: 'sub-child 1',
                leaf: true
            }];*/
            return Ext.create('Ext.data.TreeStore', {
                storeId: 'ElementStore',
                model: 'Element',
                proxy: {
                    type: 'memory',
                    data: data,
                    reader: {
                        type: 'json'
                    }
                },
                root: {
                    text: 'Elements',
                    id: 'elements',
                    expanded: true
                }
            });
        }
    }
    return that;
})();

// Create App
Ext.onReady(function() {
    var app = Ext.app.create();
});