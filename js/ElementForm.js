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
                id: 'key',
                name: 'key',
                fieldLabel: 'Key',
                width: 500,
                listeners: {
                    render: function() {
                        Ext.create('Ext.tip.ToolTip', {
                            target: 'key',
                            anchor: 'right',
                            html: 'Description Coming...'
                        });
                    }
                }
            },{
                xtype: 'combobox',
                id: 'type',
                name: 'type',
                store: this.elementTypeStore,
                displayField: 'display',
                valueField: 'value',
                fieldLabel: 'Type',
                queryMode: 'local',
                listeners: {
                    render: function() {
                        Ext.create('Ext.tip.ToolTip', {
                            target: 'key',
                            anchor: 'right',
                            html: '<h3><a name="type" id="type"></a>#type</h3>'+
                        '<p><strong>Used by</strong>: All</p>' +
                        '<p><strong>Description</strong>: Used to determine the type of form element.</p>'
                        });
                    }
                }
            },{
                xtype: 'checkbox',
                id: 'access',
                name: 'access',
                fieldLabel: 'Access',
                checked: true,
                listeners: {
                    render: function() {
                        Ext.create('Ext.tip.ToolTip', {
                            target: 'access',
                            anchor: 'right',
                            html:  '<h3><a name="access" id="access"></a>#access</h3>' +
                        '<p><strong>Used by</strong>: All elements and forms</p>' +
                        '<p><strong>Description</strong>: Whether the element is accessible or not; when FALSE, the element is not rendered and the user submitted value is not taken into consideration.</p>' +
                        '<p><strong>Values</strong>: TRUE or FALSE.</p>'
                        });
                    }
                }
            }, {
                xtype: 'formgrid',
                title: 'After Build',
                id: 'after_build',
                name: 'after_build',
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
                id: 'ahah',
                name: 'ahah',
                collapsible: true,
                items: [{
                    xtype: 'textfield',
                    id: 'effect',
                    name: 'effect',
                    fieldLabel: 'Effect'
                },{
                    xtype: 'textfield',
                    name: 'event',
                    fieldLabel: 'Event'
                },{
                    xtype: 'checkbox',
                    id: 'keypress',
                    name: 'keypress',
                    fieldLabel: 'Keypress'
                },{
                    xtype: 'textfield',
                    id: 'method',
                    name: 'method',
                    fieldLabel: 'Method'
                },{
                    xtype: 'textfield',
                    id: 'path',
                    name: 'path',
                    fieldLabel: 'Path'
                },{
                    xtype: 'textfield',
                    id: 'progress',
                    name: 'progress',
                    fieldLabel: 'Progress'
                },{
                    xtype: 'textfield',
                    id: 'wrapper',
                    name: 'wrapper',
                    fieldLabel: 'Wrapper'
                }],
                listeners: {
                    render: function() {
                        Ext.create('Ext.tip.ToolTip', {
                            tile: 'After Build',
                            target: 'after_build',
                            anchor: 'right',
                            html: '<h3><a name="after_build" id="after_build"></a>#after_build</h3>'+
                        '<p><strong>Used by</strong>: All elements and forms</p>' +
                        '<p><strong>Description</strong>: An array of function names which will be called after the form or element is built.</p>'
                        });
                    }
                }
            }, {
                xtype: 'formgrid',
                id: 'attributes',
                name: 'attributes',
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
                id: 'autocomplete_path',
                name: 'autocomplete_path',
                fieldLabel: 'Autocomplete Path'
            }, {
                xtype: 'textfield',
                id: 'button_type',
                name: 'button_type',
                fieldLabel: 'Button Type'
            }, {
                xtype: 'checkbox',
                id: 'collapsed',
                name: 'collapsed',
                fieldLabel: 'Collapsed'
            },  {
                xtype: 'numberfield',
                id: 'cols',
                name: 'cols',
                fieldLabel: 'Cols'
            }, {
                xtype: 'checkbox',
                id: 'collapsed',
                name: 'collapsed',
                fieldLabel: 'Collapsed'
            }, {
                xtype: 'textfield',
                id: 'default_value',
                name: 'default_value',
                fieldLabel: 'Default Value'
            },  {
                xtype: 'numberfield',
                id: 'delta',
                name: 'delta',
                fieldLabel: 'Delta'
            }, {
                xtype: 'textarea',
                id: 'description',
                name: 'description',
                fieldLabel: 'Description',
                width: 500
            }, {
                xtype: 'checkbox',
                id: 'disabled',
                name: 'disabled',
                fieldLabel: 'Disabled'
            }, {
                xtype: 'formgrid',
                id: 'element_validation',
                name: 'element_validation',
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
                id: 'executes_submit_callback',
                name: 'executes_submit_callback',
                fieldLabel: 'Executes Submit Callback'
            }, {
                xtype: 'textfield',
                id: 'field_prefix',
                name: 'field_prefix',
                fieldLabel: 'Field Prefix'
            }, {
                xtype: 'textfield',
                id: 'field_suffix',
                name: 'field_suffix',
                fieldLabel: 'Field Suffix'
            }, {
                xtype: 'numberfield',
                id: 'maxlength',
                name: 'maxlength',
                fieldLabel: 'Max Length'
            }, {
                xtype: 'combobox',
                id: 'method',
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
                id: 'multiple',
                name: 'multiple',
                fieldLabel: 'Multiple'
            }, {
                xtype: 'textfield',
                id: 'name',
                name: 'name',
                fieldLabel: 'Name'
            }, {
                xtype: 'formgrid',
                id: 'options',
                name: 'options',
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
                id: 'post_render',
                name: 'post_render',
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
                id: 'prefix',
                name: 'prefix',
                title: 'Prefix'
            }, {
                xtype: 'formgrid',
                title: 'Pre Render',
                id: 'pre_render',
                name: 'pre_render',
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
                id: 'process',
                name: 'process',
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
                id: 'required',
                name: 'required',
                fieldLabel: 'Required'
            }, {
                xtype: 'checkbox',
                id: 'resizable',
                name: 'resizable',
                fieldLabel: 'Resizable'
            }, {
                xtype: 'textfield',
                id: 'return_value',
                name: 'return_value',
                fieldLabel: 'Return Value'
            }, {
                xtype: 'numberfield',
                id: 'rows',
                name: 'rows',
                fieldLabel: 'Rows'
            }, {
                xtype: 'numberfield',
                id: 'size',
                name: 'size',
                fieldLabel: 'Size'
            }, {
                xtype: 'textfield',
                id: 'src',
                name: 'src',
                fieldLabel: 'Src'
            }, {
                xtype: 'formgrid',
                title: 'Submit',
                id: 'submit',
                name: 'submit',
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
                id: 'suffix',
                name: 'suffix',
                fieldLabel: 'Suffix'
            }, {
                xtype: 'textfield',
                id: 'theme',
                name: 'theme',
                fieldLabel: 'Theme'
            }, {
                xtype: 'textfield',
                id: 'title',
                name: 'title',
                fieldLabel: 'Title'
            }, {
                xtype: 'checkbox',
                id: 'tree',
                name: 'tree',
                fieldLabel: 'Tree'
            }, {
                xtype: 'formgrid',
                title: 'Validate',
                id: 'validate',
                name: 'validate',
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
                id: 'value',
                name: 'value',
                fieldLabel: 'Value'
            }, {
                xtype: 'numberfield',
                id: 'weight',
                name: 'weight',
                fieldLabel: 'Weight'
            }, {
                xtype: 'formgrid',
                title: 'User Data',
                id: 'user_data',
                name: 'user_data',
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