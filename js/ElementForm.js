/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * Create a Form for Manipulating Element data.
 */
Ext.formbuilder.createElementForm = function () {
    return Ext.create('Ext.form.Panel', {
        id: 'xml-form-builder-element-form',
        title: 'Element Form',
        region: 'center',
        frame: true,
        margin: '1 1 1 0',
        items: [{
            xtype: 'fieldset',
            title: 'Common Form Controls',
            collapsible: true,
            items: [{
                xtype: 'textfield',
                name: 'key',
                fieldLabel: 'Name',
                width: 500
            },{
                xtype: 'combobox',
                name: 'type',
                store: this.elementTypeStore,
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
                xtype: 'formgrid',
                title: 'After Build',
                id: 'after_build',
                height: 150,
                collapsible: true,
                store: this.createArrayStore(),
                modelInitTmpl: {
                    value: ''
                },
                columns: [{
                    xtype: 'gridcolumn',
                    dataIndex: 'value',
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
                xtype: 'formgrid',
                id: 'attributes',
                title: 'Attributes',
                height: 150,
                collapsible: true,
                store: this.createMapStore(),
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
                xtype: 'formgrid',
                title: 'Element Validation',
                height: 150,
                collapsible: true,
                store: this.createArrayStore(),
                modelInitTmpl: {
                    value: ''
                },
                columns: [{
                    xtype: 'gridcolumn',
                    dataIndex: 'value',
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
                editable: false,
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
                xtype: 'formgrid',
                title: 'Options',
                height: 150,
                collapsible: true,
                store: this.createMapStore(),
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
                xtype: 'formgrid',
                title: 'Post Render',
                height: 150,
                collapsible: true,
                store: this.createArrayStore(),
                modelInitTmpl: {
                    value: ''
                },
                columns: [{
                    xtype: 'gridcolumn',
                    dataIndex: 'value',
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
                xtype: 'formgrid',
                title: 'Pre Render',
                height: 150,
                collapsible: true,
                store: this.createArrayStore(),
                modelInitTmpl: {
                    value: ''
                },
                columns: [{
                    xtype: 'gridcolumn',
                    dataIndex: 'value',
                    header: 'Functions',
                    sortable: true,
                    flex: 1,
                    field: {
                        type: 'textfield'
                    }
                }]
            }, {
                xtype: 'formgrid',
                title: 'Process',
                height: 150,
                collapsible: true,
                store: this.createArrayStore(),
                modelInitTmpl: {
                    value: ''
                },
                columns: [{
                    xtype: 'gridcolumn',
                    dataIndex: 'value',
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
                xtype: 'formgrid',
                title: 'Submit',
                height: 150,
                collapsible: true,
                store: this.createArrayStore(),
                modelInitTmpl: {
                    value: ''
                },
                columns: [{
                    xtype: 'gridcolumn',
                    dataIndex: 'value',
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
                xtype: 'formgrid',
                title: 'Validate',
                height: 150,
                collapsible: true,
                store: this.createArrayStore(),
                modelInitTmpl: {
                    value: ''
                },
                columns: [{
                    xtype: 'gridcolumn',
                    dataIndex: 'value',
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
                xtype: 'formgrid',
                title: 'User Data',
                height: 150,
                collapsible: true,
                store: this.createMapStore(),
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
};