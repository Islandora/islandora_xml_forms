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
            this.createTypes();
            return Ext.create('Ext.panel.Panel', {
                width: 960,
                height: 520,
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
            var types = Ext.data.Types; 
            Ext.define('Namespace', {
                extend: 'Ext.data.Model',
                fields: ['prefix', 'uri']
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
                },
                listeners: {
                    itemclick: function(view, record, item, index, event) {
                        var display = Ext.getCmp('xml-form-builder-display').layout;
                        display.setActiveItem(2);
                        var form = Ext.getCmp('xml-form-builder-element-form').getForm();
                        form.loadRecord(record);
                        // Custom stuff...
                        var after_build = Ext.getCmp('after_build');
                        after_build.store.loadData(record.data['after_build'], false);
                        var attributes = Ext.getCmp('attributes');
                        attributes.store.loadData(record.data['attributes'], false);
                    }
                }
            });
        },
        createDisplayArea: function() {
            return Ext.create('Ext.panel.Panel', {
                id: 'xml-form-builder-display',
                region: 'center',
                layout: 'card',
                margin: '1 1 1 0',
                activeItem: 2,
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
                id: 'xml-form-builder-element-form',
                title: 'Element Form',
                region: 'center',
                frame: true,
                margin: '1 1 1 0',
                autoScroll: true,
                items: [{
                    xtype: 'fieldset',
                    title: 'Common Form Controls',
                    collapsible: true,
                    items: [{
                        xtype: 'textfield',
                        name: 'key',
                        fieldLabel: 'Name',
                        width: 500
                    }, {
                        xtype: 'combobox',
                        name: 'type',
                        store: this.createTypeStorage(),
                        displayField: 'display',
                        valueField: 'value',
                        fieldLabel: 'Type',
                        queryMode: 'local'
                    },{
                        xtype: 'checkbox',
                        name: 'access',
                        fieldLabel: 'Access',
                        checked: true
                    }, {
                        xtype: 'editablegrid',
                        title: 'After Build',
                        id: 'after_build',
                        height: 150,
                        collapsible: true,
                        store: new Ext.data.Store({
                            fields: ['function'],
                            proxy: {
                                type: 'memory',
                                reader: {
                                    type: 'array'
                                }
                            },
                            data: [["testing dummy data"], ['asdf']]
                        }),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'function',
                            header: 'Function',
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'fieldset',
                        title: 'Ahah',
                        name: 'ahah',
                        collapsible: true,
                        items: [{
                            xtype: 'textfield',
                            name: 'effect',
                            fieldLabel: 'Effect'
                        },{
                            xtype: 'textfield',
                            name: 'event',
                            fieldLabel: 'Event'
                        },{
                            xtype: 'checkbox',
                            name: 'keypress',
                            fieldLabel: 'Keypress'
                        },{
                            xtype: 'textfield',
                            name: 'method',
                            fieldLabel: 'Method'
                        },{
                            xtype: 'textfield',
                            name: 'path',
                            fieldLabel: 'Path'
                        },{
                            xtype: 'textfield',
                            name: 'progress',
                            fieldLabel: 'Progress'
                        },{
                            xtype: 'textfield',
                            name: 'wrapper',
                            fieldLabel: 'Wrapper'
                        }]
                    }, {
                        xtype: 'editablegrid',
                        id: 'attributes',
                        title: 'Attributes',
                        height: 150,
                        collapsible: true,
                        store: new Ext.data.Store({
                            fields: ['key', 'value'],
                            proxy: {
                                type: 'memory',
                                reader: {
                                    type: 'json'
                                }
                            },
                            data: [{key: 'test', value:'testing'}]
                        }),
                        modelInitTmpl: {
                            key: '',
                            value: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'key',
                            header: 'Key',
                            sortable: true,
                            width: 200,
                            field: {
                                type: 'textfield'
                            }
                        },{
                            xtype: 'gridcolumn',
                            dataIndex: 'value',
                            header: 'Value',
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'textfield',
                        name: 'autocomplete_path',
                        fieldLabel: 'Autocomplete Path'
                    }, {
                        xtype: 'textfield',
                        name: 'button_type',
                        fieldLabel: 'Button Type'
                    }, {
                        xtype: 'checkbox',
                        name: 'collapsed',
                        fieldLabel: 'Collapsed'
                    },  {
                        xtype: 'numberfield',
                        name: 'cols',
                        fieldLabel: 'Cols'
                    }, {
                        xtype: 'checkbox',
                        name: 'collapsed',
                        fieldLabel: 'Collapsed'
                    }, {
                        xtype: 'textfield',
                        name: 'default_value',
                        fieldLabel: 'Default Value'
                    },  {
                        xtype: 'numberfield',
                        name: 'delta',
                        fieldLabel: 'Delta'
                    }, {
                        xtype: 'textarea',
                        name: 'description',
                        fieldLabel: 'Description',
                        width: 500
                    }, {
                        xtype: 'checkbox',
                        name: 'disabled',
                        fieldLabel: 'Disabled'
                    }, {
                        xtype: 'editablegrid',
                        title: 'Element Validation',
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: 'Function',
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'checkbox',
                        name: 'executes_submit_callback',
                        fieldLabel: 'Executes Submit Callback'
                    }, {
                        xtype: 'textfield',
                        name: 'field_prefix',
                        fieldLabel: 'Field Prefix'
                    }, {
                        xtype: 'textfield',
                        name: 'field_suffix',
                        fieldLabel: 'Field Suffix'
                    }, {
                        xtype: 'numberfield',
                        name: 'maxlength',
                        fieldLabel: 'Max Length'
                    }, {
                        xtype: 'combobox',
                        name: 'method',
                        fieldLabel: 'Method',
                        displayField: 'display',
                        valueField: 'value',
                        editable: true,
                        store: new Ext.data.Store({
                            fields: ['display', 'value'],
                            data: [{
                                display: 'Post', 
                                value: 'post'
                            },{
                                display: 'Get', 
                                value: 'get'
                            }]
                        })
                    }, {
                        xtype: 'checkbox',
                        name: 'multiple',
                        fieldLabel: 'Multiple'
                    }, {
                        xtype: 'textfield',
                        name: 'name',
                        fieldLabel: 'Name'
                    }, {
                        xtype: 'editablegrid',
                        title: 'Options',
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            key: '',
                            value: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'key',
                            header: 'Key',
                            sortable: true,
                            width: 100,
                            field: {
                                type: 'textfield'
                            }
                        },{
                            xtype: 'gridcolumn',
                            dataIndex: 'value',
                            header: 'Value',
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'editablegrid',
                        title: 'Post Render',
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: 'Functions',
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'textfield',
                        name: 'prefix',
                        title: 'Prefix'
                    }, {
                        xtype: 'editablegrid',
                        title: 'Pre Render',
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: 'Functions',
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'editablegrid',
                        title: 'Process',
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: 'Functions',
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'checkbox',
                        name: 'required',
                        fieldLabel: 'Required'
                    }, {
                        xtype: 'checkbox',
                        name: 'resizable',
                        fieldLabel: 'Resizable'
                    }, {
                        xtype: 'textfield',
                        name: 'return_value',
                        fieldLabel: 'Return Value'
                    }, {
                        xtype: 'numberfield',
                        name: 'rows',
                        fieldLabel: 'Rows'
                    }, {
                        xtype: 'numberfield',
                        name: 'size',
                        fieldLabel: 'Size'
                    }, {
                        xtype: 'textfield',
                        name: 'src',
                        fieldLabel: 'Src'
                    }, {
                        xtype: 'editablegrid',
                        title: 'Submit',
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: 'Functions',
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'textfield',
                        name: 'suffix',
                        fieldLabel: 'Suffix'
                    }, {
                        xtype: 'textfield',
                        name: 'theme',
                        fieldLabel: 'Theme'
                    }, {
                        xtype: 'textfield',
                        name: 'title',
                        fieldLabel: 'Title'
                    }, {
                        xtype: 'checkbox',
                        name: 'tree',
                        fieldLabel: 'Tree'
                    }, {
                        xtype: 'editablegrid',
                        title: 'Validate',
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: 'Functions',
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'textfield',
                        name: 'value',
                        fieldLabel: 'Value'
                    }, {
                        xtype: 'numberfield',
                        name: 'weight',
                        fieldLabel: 'Weight'
                    }, {
                        xtype: 'editablegrid',
                        title: 'User Data',
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            key: '',
                            value: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'key',
                            header: 'Key',
                            sortable: true,
                            width: 150,
                            field: {
                                type: 'textfield'
                            }
                        },{
                            xtype: 'gridcolumn',
                            dataIndex: 'value',
                            header: 'Value',
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }]
                }],
                buttons: [{
                    text: 'Reset',
                    handler: function() {
                        this.up('form').getForm().reset();
                    }
                }, {
                    text: 'Submit',
                    formBind: true, //only enabled once the form is valid
                    disabled: true,
                    handler: function() {
                        var form = this.up('form').getForm();
                        if (form.isValid()) {
                            var record = form.getRecord();
                            record.beginEdit();
                            var values = form.getValues();
                            for(var i in values) {
                                record.set(i, values[i]);
                            }
                            record.endEdit();
                        }
                    }
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
                    xtype: 'editablegrid',
                    title: 'Namespaces',
                    height: 300,
                    store: this.createNamespaceStorage(),
                    modelInitTmpl: {
                        prefix: '', 
                        uri: ''
                    },
                    columns: [{
                        xtype: 'gridcolumn',
                        dataIndex: 'prefix',
                        header: 'Prefix',
                        sortable: true,
                        width: 150,
                        field: {
                            type: 'textfield'
                        }
                    },{
                        xtype: 'gridcolumn',
                        dataIndex: 'uri',
                        header: 'URI',
                        sortable: true,
                        flex: 1,
                        field: {
                            type: 'textfield'
                        }
                    }]
                }]
            });
        },
        createTypeStorage: function() {
            Ext.define('Type', {
                extend: 'Ext.data.Model',
                fields: ['display', 'value']
            });
            return Ext.create('Ext.data.Store', {
                storeId: 'TypeStore',
                model: 'Type',
                proxy: {
                    type: 'memory',
                    reader: {
                        type: 'json'
                    }
                },
                data: [{
                    display: 'Textfield',
                    value: 'textfield',
                    display: 'Fieldset',
                    value: 'fieldset'
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
                    data: Ext.app.FormDefinition.elements,
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
        },
        defineEditiableGrid: function() {
            Ext.define('Ext.form.Grid', {
                extend: 'Ext.form.field.Base',
                mixins: {
                    gridPanel: 'Ext.grid.Panel'
                },
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
                                scope: this,s
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
            Ext.define('Editable.Grid', {
                extend: 'Ext.grid.Panel',
                alias: 'widget.editablegrid',
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
        },
        createTypes: function() {
            this.defineEditiableGrid();
        }
        
    }
    return that;
})();

// Create App
Ext.onReady(function() {
    var app = Ext.app.create();
});