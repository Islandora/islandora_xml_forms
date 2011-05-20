/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

/**
 * 
 */
Ext.formbuilder.createPropertiesForm = function() {
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
                id: 'name',
                name: 'name',
                fieldLabel: 'Local Name',
                anchor: '100%',
                allowBlank: false
            },{
                xtype: 'textfield',
                name: 'prefix',
                fieldLabel: 'Namespace Prefix',
                anchor: '100%'
            },{
                xtype: 'textfield',
                id: 'uri',
                name: 'uri',
                fieldLabel: 'Namespace URI',
                anchor: '100%'
            }]
        },{
            xtype: 'fieldset',
            title: 'Schema',
            items: [{
                xtype: 'textfield',
                id: 'schema',
                name: 'schema',
                fieldLabel: 'Name',
                anchor: '100%'
            },{
                xtype: 'textfield',
                fieldLabel: 'URI',
                anchor: '100%'
            }]
        },{
            xtype: 'formgrid',
            title: 'Namespaces',
            id: 'namespaces',
            height: 300,
            store: new Ext.data.Store({
                fields: ['prefix', 'uri'],
                proxy: {
                    type: 'memory',
                    reader: {
                        type: 'json'
                    }
                },
                data: [{
                    prefix: 'test prefix', 
                    uri: 'test uri'
                }]
            }),
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
};