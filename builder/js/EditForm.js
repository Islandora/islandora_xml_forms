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
                title: Drupal.t('Form Editor'),
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
                        text: Drupal.t('Form Properties'),
                        handler: function() {
                            var display = Ext.getCmp('xml-form-builder-display').layout;
                            display.setActiveItem(1);
                        }
                    },{
                        xtype: 'tbfill'
                    },{
                        xtype: 'button',
                        text: Drupal.t('Preview'),
                        handler: function() {
                            var display = Ext.getCmp('xml-form-builder-display').layout;
                            display.setActiveItem(0);
                        }
                    },{
                        xtype: 'tbseparator'
                    },{
                        xtype: 'button',
                        text: Drupal.t('Save')
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
                title: Drupal.t('Elements'),
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
                        text: Drupal.t('Add')
                    },
                    {
                        xtype: 'button',
                        text: Drupal.t('Edit')
                    },
                    {
                        xtype: 'button',
                        text: Drupal.t('Copy')
                    },
                    {
                        xtype: 'button',
                        text: Drupal.t('Paste')
                    },
                    {
                        xtype: 'button',
                        text: Drupal.t('Delete')
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
                title: Drupal.t('Element Form'),
                region: 'center',
                frame: true,
                margin: '1 1 1 0',
                items: [{
                    xtype: 'fieldset',
                    title: Drupal.t('Common Form Controls'),
                    collapsible: true,
                    items: [{
                        xtype: 'textfield',
                        name: 'key',
                        fieldLabel: Drupal.t('Name'),
                        width: 500
                    }, {
                        xtype: 'combobox',
                        name: 'type',
                        store: this.createTypeStorage(),
                        displayField: 'display',
                        valueField: 'value',
                        fieldLabel: Drupal.t('Type'),
                        queryMode: 'local'
                    },{
                        xtype: 'checkbox',
                        name: 'access',
                        fieldLabel: Drupal.t('Access'),
                        checked: true
                    }, {
                        xtype: 'editablegrid',
                        title: Drupal.t('After Build'),
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
                            header: Drupal.t('Function'),
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'fieldset',
                        title: Drupal.t('Ahah'),
                        name: 'ahah',
                        collapsible: true,
                        items: [{
                            xtype: 'textfield',
                            name: 'effect',
                            fieldLabel: Drupal.t('Effect')
                        },{
                            xtype: 'textfield',
                            name: 'event',
                            fieldLabel: Drupal.t('Event')
                        },{
                            xtype: 'checkbox',
                            name: 'keypress',
                            fieldLabel: Drupal.t('Keypress')
                        },{
                            xtype: 'textfield',
                            name: 'method',
                            fieldLabel: Drupal.t('Method')
                        },{
                            xtype: 'textfield',
                            name: 'path',
                            fieldLabel: Drupal.t('Path')
                        },{
                            xtype: 'textfield',
                            name: 'progress',
                            fieldLabel: Drupal.t('Progress')
                        },{
                            xtype: 'textfield',
                            name: 'wrapper',
                            fieldLabel: Drupal.t('Wrapper')
                        }]
                    }, {
                        xtype: 'editablegrid',
                        id: 'attributes',
                        title: Drupal.t('Attributes'),
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
                            header: Drupal.t('Key'),
                            sortable: true,
                            width: 200,
                            field: {
                                type: 'textfield'
                            }
                        },{
                            xtype: 'gridcolumn',
                            dataIndex: 'value',
                            header: Drupal.t('Value'),
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'textfield',
                        name: 'autocomplete_path',
                        fieldLabel: Drupal.t('Autocomplete Path')
                    }, {
                        xtype: 'textfield',
                        name: 'button_type',
                        fieldLabel: Drupal.t('Button Type')
                    }, {
                        xtype: 'checkbox',
                        name: 'collapsed',
                        fieldLabel: Drupal.t('Collapsed')
                    },  {
                        xtype: 'numberfield',
                        name: 'cols',
                        fieldLabel: Drupal.t('Cols')
                    }, {
                        xtype: 'checkbox',
                        name: 'collapsed',
                        fieldLabel: Drupal.t('Collapsed')
                    }, {
                        xtype: 'textfield',
                        name: 'default_value',
                        fieldLabel: Drupal.t('Default Value')
                    },  {
                        xtype: 'numberfield',
                        name: 'delta',
                        fieldLabel: Drupal.t('Delta')
                    }, {
                        xtype: 'textarea',
                        name: 'description',
                        fieldLabel: Drupal.t('Description'),
                        width: 500
                    }, {
                        xtype: 'checkbox',
                        name: 'disabled',
                        fieldLabel: Drupal.t('Disabled')
                    }, {
                        xtype: 'editablegrid',
                        title: Drupal.t('Element Validation'),
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: Drupal.t('Function'),
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'checkbox',
                        name: 'executes_submit_callback',
                        fieldLabel: Drupal.t('Executes Submit Callback')
                    }, {
                        xtype: 'textfield',
                        name: 'field_prefix',
                        fieldLabel: Drupal.t('Field Prefix')
                    }, {
                        xtype: 'textfield',
                        name: 'field_suffix',
                        fieldLabel: Drupal.t('Field Suffix')
                    }, {
                        xtype: 'numberfield',
                        name: 'maxlength',
                        fieldLabel: Drupal.t('Max Length')
                    }, {
                        xtype: 'combobox',
                        name: 'method',
                        fieldLabel: Drupal.t('Method'),
                        displayField: 'display',
                        valueField: 'value',
                        editable: true,
                        store: new Ext.data.Store({
                            fields: ['display', 'value'],
                            data: [{
                                display: Drupal.t('Post'), 
                                value: 'post'
                            },{
                                display: Drupal.t('Get'), 
                                value: 'get'
                            }]
                        })
                    }, {
                        xtype: 'checkbox',
                        name: 'multiple',
                        fieldLabel: Drupal.t('Multiple')
                    }, {
                        xtype: 'textfield',
                        name: 'name',
                        fieldLabel: Drupal.t('Name')
                    }, {
                        xtype: 'editablegrid',
                        title: Drupal.t('Options'),
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
                            header: Drupal.t('Key'),
                            sortable: true,
                            width: 100,
                            field: {
                                type: 'textfield'
                            }
                        },{
                            xtype: 'gridcolumn',
                            dataIndex: 'value',
                            header: Drupal.t('Value'),
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'editablegrid',
                        title: Drupal.t('Post Render'),
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: Drupal.t('Functions'),
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'textfield',
                        name: 'prefix',
                        title: Drupal.t('Prefix')
                    }, {
                        xtype: 'editablegrid',
                        title: Drupal.t('Pre Render'),
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: Drupal.t('Functions'),
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'editablegrid',
                        title: Drupal.t('Process'),
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: Drupal.t('Functions'),
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'checkbox',
                        name: 'required',
                        fieldLabel: Drupal.t('Required')
                    }, {
                        xtype: 'checkbox',
                        name: 'resizable',
                        fieldLabel: Drupal.t('Resizable')
                    }, {
                        xtype: 'textfield',
                        name: 'return_value',
                        fieldLabel: Drupal.t('Return Value')
                    }, {
                        xtype: 'numberfield',
                        name: 'rows',
                        fieldLabel: Drupal.t('Rows')
                    }, {
                        xtype: 'numberfield',
                        name: 'size',
                        fieldLabel: Drupal.t('Size')
                    }, {
                        xtype: 'textfield',
                        name: 'src',
                        fieldLabel: Drupal.t('Src')
                    }, {
                        xtype: 'editablegrid',
                        title: Drupal.t('Submit'),
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: Drupal.t('Functions'),
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'textfield',
                        name: 'suffix',
                        fieldLabel: Drupal.t('Suffix')
                    }, {
                        xtype: 'textfield',
                        name: 'theme',
                        fieldLabel: Drupal.t('Theme')
                    }, {
                        xtype: 'textfield',
                        name: 'title',
                        fieldLabel: Drupal.t('Title')
                    }, {
                        xtype: 'checkbox',
                        name: 'tree',
                        fieldLabel: Drupal.t('Tree')
                    }, {
                        xtype: 'editablegrid',
                        title: Drupal.t('Validate'),
                        height: 150,
                        collapsible: true,
                        store: this.createNamespaceStorage(),
                        modelInitTmpl: {
                            func: ''
                        },
                        columns: [{
                            xtype: 'gridcolumn',
                            dataIndex: 'func',
                            header: Drupal.t('Functions'),
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }, {
                        xtype: 'textfield',
                        name: 'value',
                        fieldLabel: Drupal.t('Value')
                    }, {
                        xtype: 'numberfield',
                        name: 'weight',
                        fieldLabel: Drupal.t('Weight')
                    }, {
                        xtype: 'editablegrid',
                        title: Drupal.t('User Data'),
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
                            header: Drupal.t('Key'),
                            sortable: true,
                            width: 150,
                            field: {
                                type: 'textfield'
                            }
                        },{
                            xtype: 'gridcolumn',
                            dataIndex: 'value',
                            header: Drupal.t('Value'),
                            sortable: true,
                            flex: 1,
                            field: {
                                type: 'textfield'
                            }
                        }]
                    }]
                }],
                buttons: [{
                    text: Drupal.t('Reset'),
                    handler: function() {
                        this.up('form').getForm().reset();
                    }
                }, {
                    text: Drupal.t('Submit'),
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
                title: Drupal.t('Properties Form'),
                region: 'center',
                margin: '1 1 1 0',
                frame: true,
                buttons: [{
                    text: 'Save',
                    handler: function() {
                        alert(Drupal.t("Saved"));
                    }
                }],
                items:  [{
                    xtype: 'fieldset',
                    title: Drupal.t('Root Element'),
                    items: [{
                        xtype: 'textfield',
                        fieldLabel: Drupal.t('Local Name'),
                        anchor: '100%',
                        allowBlank: false
                    },{
                        xtype: 'textfield',
                        fieldLabel: Drupal.t('Namespace Prefix'),
                        anchor: '100%'
                    },{
                        xtype: 'textfield',
                        fieldLabel: Drupal.t('Namespace URI'),
                        anchor: '100%'
                    }]
                },{
                    xtype: 'fieldset',
                    title: Drupal.t('Schema'),
                    items: [{
                        xtype: 'textfield',
                        fieldLabel: Drupal.t('Name'),
                        anchor: '100%'
                    },{
                        xtype: 'textfield',
                        fieldLabel: Drupal.t('URI'),
                        anchor: '100%'
                    }]
                },{
                    xtype: 'editablegrid',
                    title: Drupal.t('Namespaces'),
                    height: 300,
                    store: this.createNamespaceStorage(),
                    modelInitTmpl: {
                        prefix: '', 
                        uri: ''
                    },
                    columns: [{
                        xtype: 'gridcolumn',
                        dataIndex: 'prefix',
                        header: Drupal.t('Prefix'),
                        sortable: true,
                        width: 150,
                        field: {
                            type: 'textfield'
                        }
                    },{
                        xtype: 'gridcolumn',
                        dataIndex: 'uri',
                        header: Drupal.t('URI'),
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
            var preview = "<iframe src='" + view_url + "' width='100%' height='100%'><p>" + Drupal.t('Your browser does not support iframes.') + "</p></iframe>";
            return Ext.create('Ext.form.Panel', {
                title: Drupal.t('Preview'),
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
                    text: Drupal.t('Elements'),
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
                                text: Drupal.t('Add'),
                                scope: this,
                                handler: this.onAddClick
                            }, {
                                iconCls: 'icon-delete',
                                text: Drupal.t('Delete'),
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
